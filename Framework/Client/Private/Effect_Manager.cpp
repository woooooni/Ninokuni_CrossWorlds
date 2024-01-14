#include "stdafx.h"
#include "Effect_Manager.h"

#include "GameInstance.h"
#include "PipeLine.h"
#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"

#include "GameObject.h"
#include "Transform.h"

#include "Effect.h"
#include "Decal.h"
#include "VfxHeaderGroup.h"

IMPLEMENT_SINGLETON(CEffect_Manager)

CEffect_Manager::CEffect_Manager()
{

}

HRESULT CEffect_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strEffectMeshPath, const wstring& strEffectPath, const wstring& strDecalPath)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (FAILED(GI->Ready_Model_Data_FromPath(LEVEL_STATIC, CModel::TYPE_NONANIM, strEffectMeshPath)))
		return E_FAIL;

	if (FAILED(Ready_Proto_Effects(strEffectPath)))
		return E_FAIL;

	if (FAILED(Ready_Proto_Decal(strDecalPath)))
		return E_FAIL;

	if (FAILED(Ready_Proto_Vfx()))
		return E_FAIL;
	
	return S_OK;
}

void CEffect_Manager::Tick(_float fTimeDelta)
{
}

HRESULT CEffect_Manager::Generate_Effect(const wstring& strEffectName, _matrix WorldMatrix, _float3 vLocalPos, _float3 vLocalScale, _float3 vLocalRotation, CGameObject* pOwner, CEffect** ppOut, _bool bDelet)
{
	// strEffectName
	CGameObject* pGameObject = GI->Clone_GameObject(L"Prototype_" + strEffectName, LAYER_EFFECT);
	if (nullptr == pGameObject)
	{
		MSG_BOX("Effect_Clone_Failde");
		return E_FAIL;
	}

	CEffect* pEffect = dynamic_cast<CEffect*>(pGameObject);
	if (nullptr == pEffect)
	{
		if (nullptr != pGameObject)
			Safe_Release(pGameObject);

		MSG_BOX("Effect_Casting_Failde");
		return E_FAIL;
	}

	// WorldMatrix
	CTransform* pTransform = pEffect->Get_Component<CTransform>(L"Com_Transform");
	if (pTransform == nullptr)
		return E_FAIL;
	pTransform->Set_WorldMatrix(WorldMatrix); // 부모 또는 자신의 행렬 셋팅

	// Scale / Rotation
	Matrix matScale    = matScale.CreateScale(vLocalScale);
	Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(vLocalRotation.x), XMConvertToRadians(vLocalRotation.y), XMConvertToRadians(vLocalRotation.z)));//matRotation.CreateRotationZ(::XMConvertToRadians(25.0f));
	Matrix matResult = matScale * matRotation * pTransform->Get_WorldFloat4x4();
	pTransform->Set_WorldMatrix(matResult);

	// Position
	_vector vCurrentPosition = pTransform->Get_Position();

	_vector vFinalPosition = vCurrentPosition;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_RIGHT) * vLocalPos.x;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_UP)    * vLocalPos.y;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_LOOK)  * vLocalPos.z;
	pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));

	// pOwner
	if (pOwner != nullptr)
	{
		pEffect->Set_Owner(pOwner);
		pEffect->Set_LoacalTransformInfo(vLocalPos, vLocalScale, vLocalRotation);
	}

	// ppOut
	if (ppOut != nullptr)
		*ppOut = pEffect;

	// bDelet
	pEffect->Set_DeleteEffect(bDelet);

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Manager::Generate_Decal(const wstring& strDecalName, _matrix WorldMatrix, _float3 vLocalPos, _float3 vLocalScale, _float3 vLocalRotation, CGameObject* pOwner, CDecal** ppOut, _bool bDelet)
{
	// strDecalName
	CGameObject* pGameObject = GI->Clone_GameObject(L"Prototype_" + strDecalName, LAYER_TYPE::LAYER_EFFECT);
	if (nullptr == pGameObject)
	{
		MSG_BOX("Decal_Clone_Failde");
		return E_FAIL;
	}

	CDecal* pDecal = dynamic_cast<CDecal*>(pGameObject);
	if (nullptr == pDecal)
	{
		if (nullptr != pGameObject)
			Safe_Release(pGameObject);

		MSG_BOX("Decal_Casting_Failde");
		return E_FAIL;
	}

	// WorldMatrix
	CTransform* pTransform = pDecal->Get_Component<CTransform>(L"Com_Transform");
	if (pTransform == nullptr)
		return E_FAIL;
	pTransform->Set_WorldMatrix(WorldMatrix); // 부모 또는 자신의 행렬 셋팅

    // Scale / Rotation
	Matrix matScale = matScale.CreateScale(vLocalScale);
	Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(vLocalRotation.x), XMConvertToRadians(vLocalRotation.y), XMConvertToRadians(vLocalRotation.z)));
	Matrix matResult = matScale * matRotation * pTransform->Get_WorldFloat4x4();
	pTransform->Set_WorldMatrix(matResult);
	
	// Position
	_vector vFinalPosition = pTransform->Get_Position();
	vFinalPosition += pTransform->Get_State(CTransform::STATE_RIGHT) * vLocalPos.x;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_UP)    * vLocalPos.y;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_LOOK)  * vLocalPos.z;
	pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));

	// pOwner
	if (pOwner != nullptr)
		pDecal->Set_Owner(pOwner);

	// ppOut
	if (ppOut != nullptr)
		*ppOut = pDecal;

	// bDelet
	pDecal->Set_DeleteDecal(bDelet);

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Manager::Generate_Vfx(const wstring& strVfxName, _matrix WorldMatrix, CGameObject* pOwner, class CVfx** ppOut)
{
	// strVfxName
	CGameObject* pGameObject = GI->Clone_GameObject(L"Prototype_" + strVfxName, LAYER_TYPE::LAYER_EFFECT);
	if (nullptr == pGameObject)
	{
		MSG_BOX("Vfx_Clone_Failde");
		return E_FAIL;
	}

	CVfx* pVfx = dynamic_cast<CVfx*>(pGameObject);
	if (nullptr == pVfx)
	{
		if (nullptr != pGameObject)
			Safe_Release(pGameObject);

		MSG_BOX("Vfx_Casting_Failde");
		return E_FAIL;
	}

	// WorldMatrix
	pVfx->Set_WorldMatrix(WorldMatrix);

	// pOwner
	if (pOwner != nullptr)
		pVfx->Set_Owner(pOwner);

	// ppOut
	if (ppOut != nullptr)
		*ppOut = pVfx;

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Manager::Ready_Proto_Effects(const wstring& strEffectPath)
{
	for (auto& p : std::filesystem::directory_iterator(strEffectPath))
	{
		if (p.is_directory())
			Ready_Proto_Effects(p.path());

		wstring strFullPath = CUtils::PathToWString(p.path().wstring());
		_tchar strFileName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strExt[MAX_PATH];

		_wsplitpath_s(strFullPath.c_str(), nullptr, 0, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		if (0 == lstrcmp(TEXT(".json"), strExt))
		{
#pragma region Load
			CEffect::EFFECT_DESC EffectInfo = {};

			Json json = GI->Json_Load(strFullPath);
			for (const auto& item : json["EffectInfo"])
			{
				// 이펙트 타입
				_int iType = item["Type"];
				EffectInfo.eType = (CEffect::EFFECT_TYPE)iType;

				// 중력 
				EffectInfo.bGravity = item["Gravity"];

				// 위치		
				EffectInfo.fRange.x = item["Range"]["x"];
				EffectInfo.fRange.y = item["Range"]["y"];
				EffectInfo.fRange.z = item["Range"]["z"];

				EffectInfo.fRangeDistance.x = item["RangeDistance"]["x"];
				EffectInfo.fRangeDistance.y = item["RangeDistance"]["y"];

#pragma region 크기		  
				EffectInfo.bScaleSameRate = item["ScaleSameRate"];

				EffectInfo.fScaleStartMin.x = item["ScaleStartMin"]["x"];
				EffectInfo.fScaleStartMin.y = item["ScaleStartMin"]["y"];
				EffectInfo.fScaleStartMin.z = item["ScaleStartMin"]["z"];

				EffectInfo.fScaleStartMax.x = item["ScaleStartMax"]["x"];
				EffectInfo.fScaleStartMax.y = item["ScaleStartMax"]["y"];
				EffectInfo.fScaleStartMax.z = item["ScaleStartMax"]["z"];

				EffectInfo.bScaleChange = item["ScaleChange"];

				EffectInfo.fScaleChangeStartDelay.x = item["ScaleChangeStartDelay"]["x"];
				EffectInfo.fScaleChangeStartDelay.y = item["ScaleChangeStartDelay"]["y"];

				EffectInfo.bScaleChangeRandom = item["ScaleChangeRandom"];

				EffectInfo.fScaleChangeTime.x = item["ScaleChangeTime"]["x"];
				EffectInfo.fScaleChangeTime.y = item["ScaleChangeTime"]["y"];

				EffectInfo.bScaleAdd = item["ScaleAdd"];
				EffectInfo.bScaleLoop = item["ScaleLoop"];
				EffectInfo.bScaleLoopStart = item["ScaleLoopStart"];

				EffectInfo.fScaleSizeMin.x = item["ScaleSizeMin"]["x"];
				EffectInfo.fScaleSizeMin.y = item["ScaleSizeMin"]["y"];
				EffectInfo.fScaleSizeMin.z = item["ScaleSizeMin"]["z"];

				EffectInfo.fScaleSizeMax.x = item["ScaleSizeMax"]["x"];
				EffectInfo.fScaleSizeMax.y = item["ScaleSizeMax"]["y"];
				EffectInfo.fScaleSizeMax.z = item["ScaleSizeMax"]["z"];

				EffectInfo.fScaleSpeed.x = item["ScaleSpeed"]["x"];
				EffectInfo.fScaleSpeed.y = item["ScaleSpeed"]["y"];

				EffectInfo.fScaleDirSpeed.x = item["ScaleDirSpeed"]["x"];
				EffectInfo.fScaleDirSpeed.y = item["ScaleDirSpeed"]["y"];
				EffectInfo.fScaleDirSpeed.z = item["ScaleDirSpeed"]["z"];
#pragma endregion

#pragma region 이동
				EffectInfo.fVelocitySpeed.x = item["VelocitySpeed"]["x"];
				EffectInfo.fVelocitySpeed.y = item["VelocitySpeed"]["y"];

				EffectInfo.vVelocityMinStart.x = item["VelocityMinStart"]["x"];
				EffectInfo.vVelocityMinStart.y = item["VelocityMinStart"]["y"];
				EffectInfo.vVelocityMinStart.z = item["VelocityMinStart"]["z"];

				EffectInfo.vVelocityMaxStart.x = item["VelocityMaxStart"]["x"];
				EffectInfo.vVelocityMaxStart.y = item["VelocityMaxStart"]["y"];
				EffectInfo.vVelocityMaxStart.z = item["VelocityMaxStart"]["z"];

				EffectInfo.bVelocityChange = item["VelocityChange"];

				EffectInfo.fVelocityChangeStartDelay.x = item["VelocityChangeStartDelay"]["x"];
				EffectInfo.fVelocityChangeStartDelay.y = item["VelocityChangeStartDelay"]["y"];

				EffectInfo.fVelocityChangeTime.x = item["VelocityChangeTime"]["x"];
				EffectInfo.fVelocityChangeTime.y = item["VelocityChangeTime"]["y"];
#pragma endregion

#pragma region 회전
				EffectInfo.bBillboard = item["Billboard"];

				EffectInfo.bRandomAxis = item["RandomAxis"];

				EffectInfo.fAxis.x = item["Axis"]["x"];
				EffectInfo.fAxis.y = item["Axis"]["y"];
				EffectInfo.fAxis.z = item["Axis"]["z"];

				EffectInfo.bRandomAngle = item["RandomAngle"];

				EffectInfo.fAngle = item["Angle"];

				EffectInfo.bRotationChange = item["RotationChange"];

				EffectInfo.fRotationChangeStartDelay.x = item["RotationChangeStartDelay"]["x"];
				EffectInfo.fRotationChangeStartDelay.y = item["RotationChangeStartDelay"]["y"];

				EffectInfo.fRotationSpeed.x = item["RotationSpeed"]["x"];
				EffectInfo.fRotationSpeed.y = item["RotationSpeed"]["y"];

				EffectInfo.fRotationDir.x = item["RotationDir"]["x"];
				EffectInfo.fRotationDir.y = item["RotationDir"]["y"];
				EffectInfo.fRotationDir.z = item["RotationDir"]["z"];

				EffectInfo.bRotationChangeRandom = item["RotationChangeRandom"];

				EffectInfo.fRotationChangeTime.x = item["RotationChangeTime"]["x"];
				EffectInfo.fRotationChangeTime.y = item["RotationChangeTime"]["y"];
#pragma endregion

				// 지속 시간
				EffectInfo.fLifeTime.x = item["LifeTime"]["x"];
				EffectInfo.fLifeTime.y = item["LifeTime"]["y"];

#pragma region 모델 && 텍스처
				EffectInfo.strModelName            = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(item["ModelName"]));
				EffectInfo.strDiffuseTetextureName = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(item["DiffuseTetextureName"]));
				EffectInfo.strAlphaTexturName      = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(item["AlphaTexturName"]));

				EffectInfo.iTextureIndexDiffuse = item["TextureIndexDiffuse"];
				EffectInfo.iTextureIndexAlpha = item["TextureIndexAlpha"];

				EffectInfo.bRandomStartIndex = item["RandomStartIndex"];

				EffectInfo.fUVIndex.x = item["UVIndex"]["x"];
				EffectInfo.fUVIndex.y = item["UVIndex"]["y"];

				EffectInfo.fMaxCount.x = item["MaxCount"]["x"];
				EffectInfo.fMaxCount.y = item["MaxCount"]["y"];

				EffectInfo.bUVFlowChange = item["UVFlowChange"];

				EffectInfo.iUVFlowLoop = item["UVFlowLoop"];

				EffectInfo.fUVFlowDir.x = item["UVFlowDir"]["x"];
				EffectInfo.fUVFlowDir.y = item["UVFlowDir"]["y"];

				EffectInfo.fUVFlowSpeed.x = item["UVFlowSpeed"]["x"];
				EffectInfo.fUVFlowSpeed.y = item["UVFlowSpeed"]["y"];
#pragma endregion

#pragma region 애니메이션
				EffectInfo.bAnimation = item["Animation"];

				EffectInfo.bAnimationLoop = item["AnimationLoop"];

				EffectInfo.bIncrement = item["Increment"];

				EffectInfo.fAnimationSpeed.x = item["AnimationSpeed"]["x"];
				EffectInfo.fAnimationSpeed.y = item["AnimationSpeed"]["y"];
#pragma endregion

#pragma region 알파
				EffectInfo.fAlphaStart.x = item["AlphaStart"]["x"];
				EffectInfo.fAlphaStart.y = item["AlphaStart"]["y"];

				EffectInfo.bAlphaCreate = item["AlphaCreate"];

				EffectInfo.bAlphaDelete = item["AlphaDelete"];

				EffectInfo.fAlphaSpeed.x = item["AlphaSpeed"]["x"];
				EffectInfo.fAlphaSpeed.y = item["AlphaSpeed"]["y"];

				EffectInfo.bAlphaChange = item["AlphaChange"];

				EffectInfo.bAlphaIn = item["AlphaIn"];

				EffectInfo.fAlphaChangeStartDelay.x = item["AlphaChangeStartDelay"]["x"];
				EffectInfo.fAlphaChangeStartDelay.y = item["AlphaChangeStartDelay"]["y"];
#pragma endregion

#pragma region 색상
				EffectInfo.bColorRandom = item["ColorRandom"];

				EffectInfo.fColorS.x = item["ColorS"]["x"];
				EffectInfo.fColorS.y = item["ColorS"]["y"];
				EffectInfo.fColorS.z = item["ColorS"]["z"];
				EffectInfo.fColorS.w = item["ColorS"]["w"];

				EffectInfo.bColorChange = item["ColorChange"];

				EffectInfo.bColorChangeRandom = item["ColorChangeRandom"];

				EffectInfo.fColorChangeRandomTime.x = item["ColorChangeRandomTime"]["x"];
				EffectInfo.fColorChangeRandomTime.y = item["ColorChangeRandomTime"]["y"];

				EffectInfo.bColorLoop = item["ColorChangeRandom"];

				EffectInfo.fColorChangeStartDelay.x = item["ColorChangeStartDelay"]["x"];
				EffectInfo.fColorChangeStartDelay.y = item["ColorChangeStartDelay"]["y"];

				EffectInfo.fColorChangeStartM.x = item["ColorChangeStartM"]["x"];
				EffectInfo.fColorChangeStartM.y = item["ColorChangeStartM"]["y"];

				EffectInfo.fColorM.x = item["ColorM"]["x"];
				EffectInfo.fColorM.y = item["ColorM"]["y"];
				EffectInfo.fColorM.z = item["ColorM"]["z"];
				EffectInfo.fColorM.w = item["ColorM"]["w"];

				EffectInfo.fColorChangeStartF.x = item["ColorChangeStartF"]["x"];
				EffectInfo.fColorChangeStartF.y = item["ColorChangeStartF"]["y"];

				EffectInfo.fColorF.x = item["ColorF"]["x"];
				EffectInfo.fColorF.y = item["ColorF"]["y"];
				EffectInfo.fColorF.z = item["ColorF"]["z"];
				EffectInfo.fColorF.w = item["ColorF"]["w"];

				EffectInfo.fColorDuration.x = item["ColorDuration"]["x"];
				EffectInfo.fColorDuration.y = item["ColorDuration"]["y"];
#pragma endregion

#pragma region 블러
				EffectInfo.bBloomPowerRandom = item["BloomPowerRandom"];

				EffectInfo.fBloomPower.x = item["BloomPower"]["x"];
				EffectInfo.fBloomPower.y = item["BloomPower"]["y"];
				EffectInfo.fBloomPower.z = item["BloomPower"]["z"];
				EffectInfo.fBloomPower.w = item["BloomPower"]["w"];

				EffectInfo.bBlurPowerRandom = item["BlurPowerRandom"];

				EffectInfo.fBlurPower = item["BlurPower"];
#pragma endregion

#pragma region 기타 정보
				EffectInfo.iShaderPass = item["ShaderPass"];

				EffectInfo.fAlpha_Discard = item["Alpha_Discard"];

				EffectInfo.fBlack_Discard.x = item["Black_Discard"]["x"];
				EffectInfo.fBlack_Discard.y = item["Black_Discard"]["y"];
				EffectInfo.fBlack_Discard.z = item["Black_Discard"]["z"];
#pragma endregion
			}
#pragma endregion

			if (FAILED(GI->Add_Prototype(wstring(L"Prototype_") + strFileName, 
				CEffect::Create(m_pDevice, m_pContext, strFileName, &EffectInfo), LAYER_TYPE::LAYER_EFFECT)))
				return E_FAIL;
			
			CGameObject* pObject = GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_EFFECT, wstring(L"Prototype_") + strFileName);
			if (pObject == nullptr)
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CEffect_Manager::Ready_Proto_Decal(const wstring& strDecalPath)
{
	for (auto& p : std::filesystem::directory_iterator(strDecalPath))
	{
		if (p.is_directory())
			Ready_Proto_Effects(p.path());

		wstring strFullPath = CUtils::PathToWString(p.path().wstring());
		_tchar strFileName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strExt[MAX_PATH];

		_wsplitpath_s(strFullPath.c_str(), nullptr, 0, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		if (0 == lstrcmp(TEXT(".json"), strExt))
		{
#pragma region Load
			CDecal::DECAL_DESC DecalInfo = {};

			Json json = GI->Json_Load(strFullPath);
			for (const auto& item : json["DecalInfo"])
			{
				DecalInfo.fScale.x = item["Scale"]["x"];
				DecalInfo.fScale.y = item["Scale"]["y"];
				DecalInfo.fScale.z = item["Scale"]["z"];

				DecalInfo.fLifeTime = item["LifeTime"];

				DecalInfo.iTextureIndexDiffuse = item["TextureIndexDiffuse"];

				DecalInfo.iShaderPass = item["ShaderPass"];
				DecalInfo.fAlpha_Discard = item["Alpha_Discard"];
				DecalInfo.fBlack_Discard.x = item["Black_Discard"]["x"];
				DecalInfo.fBlack_Discard.y = item["Black_Discard"]["y"];
				DecalInfo.fBlack_Discard.z = item["Black_Discard"]["z"];

				DecalInfo.fBloomPower.x = item["BloomPower"]["x"];
				DecalInfo.fBloomPower.y = item["BloomPower"]["y"];
				DecalInfo.fBloomPower.z = item["BloomPower"]["z"];
				DecalInfo.fBlurPower = item["BlurPower"];

				DecalInfo.fColorAdd_01_Alpha = item["ColorAdd_01_Alpha"];
				DecalInfo.fColorAdd_01.x = item["ColorAdd_01"]["x"];
				DecalInfo.fColorAdd_01.y = item["ColorAdd_01"]["y"];
				DecalInfo.fColorAdd_01.z = item["ColorAdd_01"]["z"];
				DecalInfo.fColorAdd_02.x = item["ColorAdd_02"]["x"];
				DecalInfo.fColorAdd_02.y = item["ColorAdd_02"]["y"];
				DecalInfo.fColorAdd_02.z = item["ColorAdd_02"]["z"];

				DecalInfo.fAlphaRemove = item["AlphaRemove"];
				DecalInfo.bAlphaCreate = item["AlphaCreate"];
				DecalInfo.bAlphaDelete = item["AlphaDelete"];
				DecalInfo.fAlphaSpeed = item["AlphaSpeed"];
			}
#pragma endregion

			if (FAILED(GI->Add_Prototype(wstring(L"Prototype_") + strFileName,
				CDecal::Create(m_pDevice, m_pContext, strFileName, &DecalInfo), LAYER_TYPE::LAYER_EFFECT)))
				return E_FAIL;

			CGameObject* pObject = GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_EFFECT, wstring(L"Prototype_") + strFileName);
			if (pObject == nullptr)
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CEffect_Manager::Ready_Proto_Vfx()
{
	if (FAILED(Ready_Proto_Vfx_UI()))
		return E_FAIL;

	if (FAILED(Ready_Proto_Vfx_Player()))
		return E_FAIL;

	if (FAILED(Ready_Proto_Vfx_Monster()))
		return E_FAIL;

	// Prototype_Vfx_QuestPoint
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_QuestPoint"),
		CVfx_QuestPoint::Create(m_pDevice, m_pContext, TEXT("QuestPoint")), LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	// Prototype_Vfx_PortalPoint
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_PortalPoint"),
		CVfx_PortalPoint::Create(m_pDevice, m_pContext, TEXT("PortalPoint")), LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Manager::Ready_Proto_Vfx_UI()
{
	// Prototype_Vfx_MouseClick
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_MouseClick"),
		CVfx_UI_MouseClick::Create(m_pDevice, m_pContext, TEXT("Particles_MouseClick")), LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	// Prototype_Vfx_TargetPoint(For Monster)
//	if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_TargetMonsterPoint"),
//		CVfx_TargetPoint::Create(m_pDevice, m_pContext, TEXT("Decal_TargetPoint")), LAYER_TYPE::LAYER_EFFECT)))
//		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Manager::Ready_Proto_Vfx_Player()
{
	// SwordMan Skill
	{
		// Prototype_Vfx_SwordMan_Skill_PerfectBlade
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_SwordMan_Skill_PerfectBlade"),
			CVfx_SwordMan_Skill_PerfectBlade::Create(m_pDevice, m_pContext, TEXT("SwordMan_Skill_PerfectBlade")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_SwordMan_Skill_SipohoningLunge
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_SwordMan_Skill_SipohoningLunge"),
			CVfx_SwordMan_Skill_SipohoningLunge::Create(m_pDevice, m_pContext, TEXT("SwordMan_Skill_SipohoningLunge")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_SwordMan_Skill_SpinningAssault
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_SwordMan_Skill_SpinningAssault"),
			CVfx_SwordMan_Skill_SpinningAssault::Create(m_pDevice, m_pContext, TEXT("SwordMan_Skill_SpinningAssault")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_SwordMan_Skill_MegaSlash
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_SwordMan_Skill_MegaSlash"),
			CVfx_SwordMan_Skill_MegaSlash::Create(m_pDevice, m_pContext, TEXT("SwordMan_Skill_MegaSlash")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_SwordMan_Skill_SwordTempest
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_SwordMan_Skill_SwordTempest"),
			CVfx_SwordMan_Skill_SwordTempest::Create(m_pDevice, m_pContext, TEXT("SwordMan_Skill_SwordTempest")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_SwordMan_Skill_FrozenStorm
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_SwordMan_Skill_FrozenStorm"),
			CVfx_SwordMan_Skill_FrozenStorm::Create(m_pDevice, m_pContext, TEXT("SwordMan_Skill_FrozenStorm")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_SwordMan_Skill_AcaneBarrier
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_SwordMan_Skill_AcaneBarrier"),
			CVfx_SwordMan_Skill_AcaneBarrier::Create(m_pDevice, m_pContext, TEXT("SwordMan_Skill_AcaneBarrier")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;
	}
	
	// Engineer Skill
	{
		// Prototype_Vfx_Engineer_Skill_BurstCall
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Engineer_Skill_BurstCall"),
			CVfx_Engineer_Skill_BurstCall::Create(m_pDevice, m_pContext, TEXT("Engineer_Skill_BurstCall")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Engineer_Skill_ElementalBlast
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Engineer_Skill_ElementalBlast"),
			CVfx_Engineer_Skill_ElementalBlast::Create(m_pDevice, m_pContext, TEXT("Engineer_Skill_ElementalBlast")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Engineer_Skill_ExplosionShot
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Engineer_Skill_ExplosionShot"),
			CVfx_Engineer_Skill_ExplosionShot::Create(m_pDevice, m_pContext, TEXT("Engineer_Skill_ExplosionShot")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Engineer_Skill_Destruction
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Engineer_Skill_Destruction"),
			CVfx_Engineer_Skill_Destruction::Create(m_pDevice, m_pContext, TEXT("Engineer_Skill_Destruction")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Engineer_Skill_HealingTree
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Engineer_Skill_HealingTree"),
			CVfx_Engineer_Skill_HealingTree::Create(m_pDevice, m_pContext, TEXT("Engineer_Skill_HealingTree")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Engineer_Skill_TimeLab
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Engineer_Skill_TimeLab"),
			CVfx_Engineer_Skill_TimeLab::Create(m_pDevice, m_pContext, TEXT("Engineer_Skill_TimeLab")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Engineer_Skill_FlashHeal
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Engineer_Skill_FlashHeal"),
			CVfx_Engineer_Skill_FlashHeal::Create(m_pDevice, m_pContext, TEXT("Engineer_Skill_FlashHeal")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;
	}
	
	// Destroyer Skill
	{
		// Prototype_Vfx_Destroyer_Skill_WheelWind
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Destroyer_Skill_WheelWind"),
			CVfx_Destroyer_Skill_WheelWind::Create(m_pDevice, m_pContext, TEXT("Destroyer_Skill_WheelWind")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Destroyer_Skill_BrutalStrike
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Destroyer_Skill_BrutalStrike"),
			CVfx_Destroyer_Skill_BrutalStrike::Create(m_pDevice, m_pContext, TEXT("Destroyer_Skill_BrutalStrike")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Destroyer_Skill_LeafSlam
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Destroyer_Skill_LeafSlam"),
			CVfx_Destroyer_Skill_LeafSlam::Create(m_pDevice, m_pContext, TEXT("Destroyer_Skill_LeafSlam")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Destroyer_Skill_HyperStrike
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Destroyer_Skill_HyperStrike"),
			CVfx_Destroyer_Skill_HyperStrike::Create(m_pDevice, m_pContext, TEXT("Destroyer_Skill_HyperStrike")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Destroyer_Skill_BattleCry
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Destroyer_Skill_BattleCry"),
			CVfx_Destroyer_Skill_BattleCry::Create(m_pDevice, m_pContext, TEXT("Destroyer_Skill_BattleCry")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Destroyer_Skill_IgnorePain
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Destroyer_Skill_IgnorePain"),
			CVfx_Destroyer_Skill_IgnorePain::Create(m_pDevice, m_pContext, TEXT("Destroyer_Skill_IgnorePain")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Destroyer_Skill_FrengeCharge
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Destroyer_Skill_FrengeCharge"),
			CVfx_Destroyer_Skill_FrengeCharge::Create(m_pDevice, m_pContext, TEXT("Destroyer_Skill_FrengeCharge")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Manager::Ready_Proto_Vfx_Monster()
{
	// Glanix
	{
		// Prototype_Vfx_Glanix_Skill_HandDown
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Glanix_Skill_HandDown"),
			CVfx_Glanix_Skill_HandDown::Create(m_pDevice, m_pContext, TEXT("Glanix_Skill_HandDown")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Glanix_Skill_JumpDown
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Glanix_Skill_JumpDown"),
			CVfx_Glanix_Skill_JumpDown::Create(m_pDevice, m_pContext, TEXT("Glanix_Skill_JumpDown")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Glanix_Skill_FootDown
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Glanix_Skill_FootDown"),
			CVfx_Glanix_Skill_FootDown::Create(m_pDevice, m_pContext, TEXT("Glanix_Skill_FootDown")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Glanix_Skill_TwoHandSwing
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Glanix_Skill_TwoHandSwing"),
			CVfx_Glanix_Skill_TwoHandSwing::Create(m_pDevice, m_pContext, TEXT("Glanix_Skill_TwoHandSwing")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Glanix_Skill_FourHandSwing
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Glanix_Skill_FourHandSwing"),
			CVfx_Glanix_Skill_FourHandSwing::Create(m_pDevice, m_pContext, TEXT("Glanix_Skill_FourHandSwing")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Glanix_Skill_SwingDown
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Glanix_Skill_SwingDown"),
			CVfx_Glanix_Skill_SwingDown::Create(m_pDevice, m_pContext, TEXT("Glanix_Skill_SwingDown")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Glanix_Skill_SwingDownDown
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Glanix_Skill_SwingDownDown"),
			CVfx_Glanix_Skill_SwingDownDown::Create(m_pDevice, m_pContext, TEXT("Glanix_Skill_SwingDownDown")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Glanix_Skill_Rush
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Glanix_Skill_Rush"),
			CVfx_Glanix_Skill_Rush::Create(m_pDevice, m_pContext, TEXT("Glanix_Skill_Rush")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;

		// Prototype_Vfx_Glanix_Skill_IceThrow
		if (FAILED(GI->Add_Prototype(TEXT("Prototype_Vfx_Glanix_Skill_IceThrow"),
			CVfx_Glanix_Skill_IceThrow::Create(m_pDevice, m_pContext, TEXT("Glanix_Skill_IceThrow")), LAYER_TYPE::LAYER_EFFECT)))
			return E_FAIL;
	}

	return S_OK;
}

void CEffect_Manager::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
