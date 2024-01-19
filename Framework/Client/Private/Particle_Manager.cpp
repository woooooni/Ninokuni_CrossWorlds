#include "stdafx.h"
#include "Particle_Manager.h"

#include "GameInstance.h"
#include "PipeLine.h"
#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"

#include "GameObject.h"
#include "Transform.h"
#include "Particle.h"

IMPLEMENT_SINGLETON(CParticle_Manager)

CParticle_Manager::CParticle_Manager()
{

}

HRESULT CParticle_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strParticlePath)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	
	if (FAILED(Ready_Proto_Particles(strParticlePath)))
		return E_FAIL;

	return S_OK;
}

void CParticle_Manager::Tick(_float fTimeDelta)
{

}

HRESULT CParticle_Manager::AddLevel_Particle(_uint iLevelIndex, const wstring& strParticleName, _matrix WorldMatrix, _float3 vLocalPos, _float3 vLocalScale, _float3 vLocalRotation, CGameObject* pOwner, CParticle** ppOut)
{
	// strParticleName
	CGameObject* pGameObject = GI->Clone_GameObject(L"Prototype_" + strParticleName, LAYER_TYPE::LAYER_EFFECT);
	if (nullptr == pGameObject)
	{
		MSG_BOX("Particle_Clone_Failde");
		return E_FAIL;
	}

	CParticle* pParticle = dynamic_cast<CParticle*>(pGameObject);
	if (nullptr == pParticle)
	{
		if (nullptr != pGameObject)
			Safe_Release(pGameObject);

		MSG_BOX("Particle_Casting_Failde");
		return E_FAIL;
	}

	// WorldMatrix
	CTransform* pTransform = pParticle->Get_Component<CTransform>(L"Com_Transform");
	if (pTransform == nullptr)
		return E_FAIL;
	pTransform->Set_WorldMatrix(WorldMatrix);

	// Scale / Rotation
	Matrix matScale = matScale.CreateScale(vLocalScale);
	Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(vLocalRotation.x), XMConvertToRadians(vLocalRotation.y), XMConvertToRadians(vLocalRotation.z)));
	Matrix matResult = matScale * matRotation * pTransform->Get_WorldFloat4x4();
	pTransform->Set_WorldMatrix(matResult);

	// Position
	_vector vCurrentPosition = pTransform->Get_Position();

	_vector vFinalPosition = vCurrentPosition;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_RIGHT) * vLocalPos.x;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_UP) * vLocalPos.y;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_LOOK) * vLocalPos.z;
	pParticle->Set_Position_Particle(_float3(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition)));

	// pOwner
	if (pOwner != nullptr)
	{
		pParticle->Set_Owner(pOwner);
		pParticle->Set_LoacalTransformInfo(vLocalPos, vLocalScale, vLocalRotation);
	}

	// ppOut
	if (ppOut != nullptr)
		*ppOut = pParticle;

	if (FAILED(GI->Add_GameObject(iLevelIndex, LAYER_TYPE::LAYER_EFFECT, pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Manager::Generate_Particle(const wstring& strParticleName, _matrix WorldMatrix, _float3 vLocalPos, _float3 vLocalScale, _float3 vLocalRotation, CGameObject* pOwner, class CParticle** ppOut, _bool bDelet)
{
	// strParticleName
	CGameObject* pGameObject = GI->Clone_GameObject(L"Prototype_" + strParticleName, LAYER_TYPE::LAYER_EFFECT);
	if (nullptr == pGameObject)
	{
		MSG_BOX("Particle_Clone_Failde");
		return E_FAIL;
	}

	CParticle* pParticle = dynamic_cast<CParticle*>(pGameObject);
	if (nullptr == pParticle)
	{
		if (nullptr != pGameObject)
			Safe_Release(pGameObject);

		MSG_BOX("Particle_Casting_Failde");
		return E_FAIL;
	}

	// WorldMatrix
	CTransform* pTransform = pParticle->Get_Component<CTransform>(L"Com_Transform");
	if (pTransform == nullptr)
		return E_FAIL;
	pTransform->Set_WorldMatrix(WorldMatrix);

	// Scale / Rotation
	Matrix matScale = matScale.CreateScale(vLocalScale);
	Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(vLocalRotation.x), XMConvertToRadians(vLocalRotation.y), XMConvertToRadians(vLocalRotation.z)));
	Matrix matResult = matScale * matRotation * pTransform->Get_WorldFloat4x4();
	pTransform->Set_WorldMatrix(matResult);

	// Position
	_vector vCurrentPosition = pTransform->Get_Position();

	_vector vFinalPosition = vCurrentPosition;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_RIGHT) * vLocalPos.x;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_UP)    * vLocalPos.y;
	vFinalPosition += pTransform->Get_State(CTransform::STATE_LOOK)  * vLocalPos.z;
	pParticle->Set_Position_Particle(_float3(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition)));

	// pOwner
	if (pOwner != nullptr)
	{
		pParticle->Set_Owner(pOwner);
		pParticle->Set_LoacalTransformInfo(vLocalPos, vLocalScale, vLocalRotation);
	}
		
	// ppOut
	if (ppOut != nullptr)
		*ppOut = pParticle;

	// bDelet
	pParticle->Set_DeleteParticle(bDelet);

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Manager::Generate_Particle_To_Position(const wstring& strParticleName, _matrix WorldMatrix, _float3 vLocalPos, _float3 vLocalScale, _float3 vLocalRotation, CGameObject* pOwner, CParticle** ppOut, _bool bDelet)
{
	// strDecalName
	CGameObject* pGameObject = GI->Clone_GameObject(L"Prototype_" + strParticleName, LAYER_TYPE::LAYER_EFFECT);
	if (nullptr == pGameObject)
	{
		MSG_BOX("Decal_Clone_Failde");
		return E_FAIL;
	}

	CParticle* pParticle = dynamic_cast<CParticle*>(pGameObject);
	if (nullptr == pParticle)
	{
		if (nullptr != pGameObject)
			Safe_Release(pGameObject);

		MSG_BOX("Decal_Casting_Failde");
		return E_FAIL;
	}

	// WorldMatrix
	CTransform* pTransform = pParticle->Get_Component<CTransform>(L"Com_Transform");
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
	vFinalPosition.m128_f32[0] += vLocalPos.x;
	vFinalPosition.m128_f32[1] += vLocalPos.y;
	vFinalPosition.m128_f32[2] += vLocalPos.z;
	pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));

	// pOwner
	if (pOwner != nullptr)
		pParticle->Set_Owner(pOwner);

	// ppOut
	if (ppOut != nullptr)
		*ppOut = pParticle;

	// bDelet
	pParticle->Set_DeleteParticle(bDelet);

	Vec4 vOffsetPos = Vec4(vLocalPos.x, vLocalPos.y, vLocalPos.z, 0.0f);
	pParticle->Set_OffsetPosition(vOffsetPos);

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT, pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Manager::Tick_Generate_Particle(_float* fTimeAcc, _float fCreateTime, _float fTimeDelta, const wstring& strParticleName, CGameObject* pOwner, _float3 vLocalPos, _float3 vLocalScale, _float3 vLocalRotation, _bool bOwnerSet)
{
	*fTimeAcc += fTimeDelta;
	if (*fTimeAcc >= fCreateTime)
	{
		*fTimeAcc = 0.f;

		CTransform* pTransform = pOwner->Get_Component<CTransform>(L"Com_Transform");
		if (pTransform == nullptr)
			return S_OK;

		if(!bOwnerSet)
			Generate_Particle(strParticleName, pTransform->Get_WorldMatrix(), vLocalPos, vLocalScale, vLocalRotation);
		else
			Generate_Particle(strParticleName, pTransform->Get_WorldMatrix(), vLocalPos, vLocalScale, vLocalRotation, pOwner);
	}

	return S_OK;
}

HRESULT CParticle_Manager::Tick_Generate_Particle_To_Matrix(_float* fTimeAcc, _float fCreateTime, _float fTimeDelta, const wstring& strParticleName, _matrix WorldMatrix, _float3 vLocalPos, _float3 vLocalScale, _float3 vLocalRotation, CGameObject* pOwner)
{
	*fTimeAcc += fTimeDelta;
	if (*fTimeAcc >= fCreateTime)
	{
		*fTimeAcc = 0.f;

		if (nullptr == pOwner)
			Generate_Particle(strParticleName, WorldMatrix, vLocalPos, vLocalScale, vLocalRotation);
		else
			Generate_Particle(strParticleName, WorldMatrix, vLocalPos, vLocalScale, vLocalRotation, pOwner);
	}

	return S_OK;
}

HRESULT CParticle_Manager::Ready_Proto_Particles(const wstring& strParticlePath)
{
	for (auto& p : std::filesystem::directory_iterator(strParticlePath))
	{
		if (p.is_directory())
			Ready_Proto_Particles(p.path());

		wstring strFullPath = CUtils::PathToWString(p.path().wstring());

		_tchar strDriveName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strFileName[MAX_PATH];
		_tchar strExt[MAX_PATH];

		_wsplitpath_s(strFullPath.c_str(), strDriveName, MAX_PATH, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		if (0 == lstrcmp(TEXT(".Particle"), strExt))
		{
			size_t length = _tcslen(strFileName);
			wchar_t* wcharFileName = new wchar_t[length + 1];
			wcscpy_s(wcharFileName, length + 1, strFileName);
			wcharFileName[length] = L'\0';

			wstring strParticleName(wcharFileName);

#pragma region Basic_Load
			CParticle::PARTICLE_DESC tParticleDesc = {};

			shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
			File->Open(strFullPath, FileMode::Read);

			// 파티클 타입
			_uint iParticleType = 0;
			File->Read<_uint>(iParticleType);
			tParticleDesc.eParticleType = (CParticle::PARTICLEPROJTYPE)iParticleType;

			// 정렬 설정
			File->Read<_bool>(tParticleDesc.bParticleSortZ);

			// 반복 여부
			File->Read<_bool>(tParticleDesc.bParticleLoop);

			// 파티클 개수
			File->Read<_uint>(tParticleDesc.iNumEffectMaxCount);
			File->Read<_uint>(tParticleDesc.iNumEffectCount);

			// 분포 범위
			File->Read<_float3>(tParticleDesc.fRange);
			File->Read<_float2>(tParticleDesc.fRangeDistance);

#pragma region 크기
			File->Read<_bool>(tParticleDesc.bScaleSameRate);
			File->Read<_float2>(tParticleDesc.fScaleStart);

			File->Read<_bool>(tParticleDesc.bScaleChange);
			File->Read<_float2>(tParticleDesc.fScaleChangeStartDelay);

			File->Read<_bool>(tParticleDesc.bScaleChangeRandom);
			File->Read<_float2>(tParticleDesc.fScaleChangeTime);

			File->Read<_bool>(tParticleDesc.bScaleAdd);
			File->Read<_bool>(tParticleDesc.bScaleLoop);
			File->Read<_bool>(tParticleDesc.bScaleLoopStart);

			File->Read<_float2>(tParticleDesc.fScaleMin);
			File->Read<_float2>(tParticleDesc.fScaleMax);
			File->Read<_float2>(tParticleDesc.fScaleSpeed);
#pragma endregion

#pragma region 이동
			File->Read<_float2>(tParticleDesc.fVelocitySpeed);

			File->Read<_float3>(tParticleDesc.vVelocityMinStart);
			File->Read<_float3>(tParticleDesc.vVelocityMaxStart);

			File->Read<_bool>(tParticleDesc.bVelocityChange);
			File->Read<_float2>(tParticleDesc.fVelocityChangeStartDelay);

			File->Read<_bool>(tParticleDesc.bVelocityChangeRandom);
			File->Read<_float2>(tParticleDesc.fVelocityChangeTime);

			File->Read<_bool>(tParticleDesc.bVelocityLoop);
			File->Read<_uint>(tParticleDesc.iVelocityCountCur);
			File->Read<_uint>(tParticleDesc.iVelocityCountMax);

			File->Read<_uint>(tParticleDesc.iVelocityUse);
			if (tParticleDesc.iVelocityUse > 0)
			{
				tParticleDesc.pVelocityMin = new _float3[tParticleDesc.iVelocityUse];
				tParticleDesc.pVelocityMax = new _float3[tParticleDesc.iVelocityUse];
				tParticleDesc.pVelocityTime = new _float2[tParticleDesc.iVelocityUse];
				if (tParticleDesc.pVelocityMin != nullptr)
				{
					for (size_t i = 0; i < tParticleDesc.iVelocityUse; ++i)
						File->Read<_float3>(tParticleDesc.pVelocityMin[i]);
				}

				if (tParticleDesc.pVelocityMax != nullptr)
				{
					for (size_t i = 0; i < tParticleDesc.iVelocityUse; ++i)
						File->Read<_float3>(tParticleDesc.pVelocityMax[i]);
				}

				if (tParticleDesc.pVelocityTime != nullptr)
				{
					for (size_t i = 0; i < tParticleDesc.iVelocityUse; ++i)
						File->Read<_float2>(tParticleDesc.pVelocityTime[i]);
				}
			}
#pragma endregion

#pragma region 회전
			File->Read<_bool>(tParticleDesc.bBillboard);

			File->Read<_bool>(tParticleDesc.bRandomAxis);
			File->Read<_vector>(tParticleDesc.vAxis);

			File->Read<_bool>(tParticleDesc.bRandomAngle);
			File->Read<_float>(tParticleDesc.fAngle);

			File->Read<_bool>(tParticleDesc.bRotationChange);
			File->Read<_float2>(tParticleDesc.fRotationChangeStartDelay);

			File->Read<_float2>(tParticleDesc.fRotationSpeed);

			File->Read<_bool>(tParticleDesc.bRotationChangeRandom);
			File->Read<_float2>(tParticleDesc.fRotationChangeTime);

			File->Read<_bool>(tParticleDesc.bRotationAdd);
#pragma endregion

			// 지속 시간
			File->Read<_float2>(tParticleDesc.fLifeTime);

			// 박스 범위
			File->Read<_bool>(tParticleDesc.bUseBox);
			File->Read<_float3>(tParticleDesc.fBoxMin);
			File->Read<_float3>(tParticleDesc.fBoxMax);

#pragma region 텍스처
			// 문자열
			tParticleDesc.strDiffuseTetextureName = CUtils::ToWString(File->Read<string>());
			tParticleDesc.strDiffuseTetexturePath = CUtils::ToWString(File->Read<string>());
			tParticleDesc.strAlphaTexturName = CUtils::ToWString(File->Read<string>());
			tParticleDesc.strAlphaTexturPath = CUtils::ToWString(File->Read<string>());

			File->Read<_bool>(tParticleDesc.bRandomStartIndex);
			File->Read<_float2>(tParticleDesc.fUVIndex);
			File->Read<_float2>(tParticleDesc.fUVMaxCount);

			File->Read<_uint>(tParticleDesc.iTextureIndexDiffuse);
			File->Read<_uint>(tParticleDesc.iTextureIndexAlpha);
#pragma endregion

#pragma region 애니메이션
			File->Read<_bool>(tParticleDesc.bAnimation);
			File->Read<_bool>(tParticleDesc.bAnimationLoop);
			File->Read<_float2>(tParticleDesc.fAnimationSpeed);
#pragma endregion

#pragma region 알파
			File->Read<_float2>(tParticleDesc.fStartAlpha);

			File->Read<_bool>(tParticleDesc.bFadeCreate);
			File->Read<_bool>(tParticleDesc.bFadeDelete);
			File->Read<_float2>(tParticleDesc.fFadeSpeed);

			File->Read<_bool>(tParticleDesc.bFadeChange);
			File->Read<_bool>(tParticleDesc.bFadeIn);
			File->Read<_float2>(tParticleDesc.fFadeChangeStartDelay);
#pragma endregion

#pragma region 색상
			File->Read<_bool>(tParticleDesc.bColorRandom);
			File->Read<_float4>(tParticleDesc.vColorS);

			File->Read<_bool>(tParticleDesc.bColorChange);

			File->Read<_bool>(tParticleDesc.bColorChangeRandom);
			File->Read<_float2>(tParticleDesc.fColorChangeRandomTime);

			File->Read<_bool>(tParticleDesc.bColorLoop);
			File->Read<_float2>(tParticleDesc.fColorChangeStartDelay);

			File->Read<_float2>(tParticleDesc.fColorChangeStartM);
			File->Read<_float4>(tParticleDesc.fColorM);

			File->Read<_float2>(tParticleDesc.fColorChangeStartF);
			File->Read<_float4>(tParticleDesc.fColorF);

			File->Read<_float2>(tParticleDesc.fColorDuration);
#pragma endregion

#pragma region 블러
			File->Read<_bool>(tParticleDesc.bBloomPowerRandom);
			File->Read<_float4>(tParticleDesc.fBloomPower);
			File->Read<_bool>(tParticleDesc.bBlurPowerRandom);
			File->Read<_float>(tParticleDesc.fBlurPower);
#pragma endregion

#pragma region 기타 정보
			File->Read<_uint>(tParticleDesc.iShaderPass);
			File->Read<_float>(tParticleDesc.fAlpha_Discard);
			File->Read<_float3>(tParticleDesc.fBlack_Discard);
#pragma endregion

#pragma endregion

#pragma region Rigidbody_Load
			wstring strRigidPath = strDriveName;
			strRigidPath += strFolderName;
			strRigidPath += strFileName;
			strRigidPath += L".Rigidbody";

			CParticle::PARTICLE_RIGIDBODY_DESC tRigidbodyDesc = {};
			if (true == filesystem::exists(strRigidPath)) // 해당 파일이 존재
			{
				shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
				File->Open(strRigidPath, FileMode::Read);

				File->Read<_bool>(tRigidbodyDesc.bRigidbody);
				File->Read<_bool>(tRigidbodyDesc.bGravity);
				File->Read<_bool>(tRigidbodyDesc.bStopZero);
				File->Read<_bool>(tRigidbodyDesc.bStopStartY);
				File->Read<_bool>(tRigidbodyDesc.bGroundSlide);

				File->Read<_bool>(tRigidbodyDesc.bStartJump);
				File->Read<Vec4>(tRigidbodyDesc.vStartMinVelocity);
				File->Read<Vec4>(tRigidbodyDesc.vStartMaxVelocity);

				File->Read<Vec4>(tRigidbodyDesc.vMaxVelocity);
				File->Read<_float>(tRigidbodyDesc.fMass);
				File->Read<_float>(tRigidbodyDesc.fFricCoeff);
			}
#pragma endregion

			if (FAILED(GI->Add_Prototype(L"Prototype_" + strParticleName,
				CParticle::Create(m_pDevice, m_pContext, strParticleName, &tParticleDesc, &tRigidbodyDesc), LAYER_TYPE::LAYER_EFFECT)))
				return E_FAIL;

			Safe_Delete(wcharFileName);
		}
	}

	return S_OK;
}

void CParticle_Manager::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
