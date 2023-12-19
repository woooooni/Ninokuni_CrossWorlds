#include "stdafx.h"
#include "Tool_Effect.h"
#include "imgui.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "filesystem"
#include "FileUtils.h"
#include "Utils.h"

CTool_Effect::CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{
}

HRESULT CTool_Effect::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_Effect::Tick(_float fTimeDelta)
{
	ImGui::Begin("Effect_Tool");

	// 생성/ 삭제
	if (ImGui::Button("EffectCreate"))
		Create_Effect();
	ImGui::SameLine();
	if (ImGui::Button("EffectDelete")) 
	{
		if (m_pEffect != nullptr) 
		{
			m_pEffect->Set_Dead(true);
			m_pEffect = nullptr;
		}
	}
	ImGui::SameLine();

	// 확인(적용)
	if (ImGui::Button("TransformSelect"))
		Store_TransformEffect();
	ImGui::SameLine();
	if (ImGui::Button("EffectInfoSelect"))
		Store_InfoEffect();
	ImGui::NewLine();

	// 바이너리
	if (ImGui::CollapsingHeader("EffectBinary"))
	{
		// 바이너리 fbx
		if (ImGui::TreeNode("MeshBinary"))
		{
			if (ImGui::Button("LoadMeshBinary"))
			{

			}

			ImGui::InputText("##MeshBinary", m_cLoadFbxName, IM_ARRAYSIZE(m_cLoadFbxName));

			ImGui::TreePop();
		}

		// 바이너리 effect
		if (ImGui::TreeNode("EffectBinary"))
		{
			if (ImGui::Button("SaveEffectBinary"))
			{

			}
			ImGui::SameLine();
			if (ImGui::Button("LoadEffectBinary"))
			{

			}

			ImGui::InputText("##EffectBinary", m_cLoadFbxName, IM_ARRAYSIZE(m_cLoadFbxName));

			ImGui::TreePop();
		}
	}

	// 트랜스폼
	if (ImGui::CollapsingHeader("EffectTransform"))
	{
		ImGui::Text("Position");
		ImGui::InputFloat3("##Position", m_fPosition);

		ImGui::Text("Rotation");
		ImGui::InputFloat3("##Rotation", m_fRotation);

		ImGui::Text("Scale");
		ImGui::InputFloat3("##Scale", m_fScale);
	}

	// 기본 정보
	if (ImGui::CollapsingHeader("EffectBasicInfo"))
	{

	}

	// 텍스처 지정
	if (ImGui::CollapsingHeader("EffectTexture"))
	{
		// 이펙트 폴더 내의 폴더별로 추가하기
		// 이미지 프로토 타입으로 로드 설정 (선택해서 사용할 수 있게)
		ImGui::Text("DiffuseFolderName :");
		ImGui::SameLine();
		if(ImGui::Combo("##DiffuseFolderName", &m_iDiffuseFolderIndex, m_cFolderName, IM_ARRAYSIZE(m_cFolderName)))
			Store_InfoEffect();

		ImGui::Text("DiffuseTextureIndex");
		if (ImGui::InputInt("##DiffuseTextureIndex", &(_int)m_tEffectInfo.iTextureIndexDiffuse))
			Store_InfoEffect();
		ImGui::NewLine();

		ImGui::Text("AlphaFolderName :");
		ImGui::SameLine();
		if (ImGui::Combo("##AlphaFolderName", &m_iAlphaFolderIndex, m_cFolderName, IM_ARRAYSIZE(m_cFolderName)))
			Store_InfoEffect();

		ImGui::Text("AlphaTextureIndex");
		if (ImGui::InputInt("##AlphaTextureIndex", &(_int)m_tEffectInfo.iTextureIndexAlpha))
			Store_InfoEffect();
		ImGui::NewLine();
	}

	// 쉐이더 값 셋팅
	if (ImGui::CollapsingHeader("EffectShader"))
	{
		// 쉐이더 패스
		ImGui::Text("ShaderPass");
		if (ImGui::InputInt("##ShaderPass", &(_int)m_tEffectInfo.iShaderPass))
			Store_InfoEffect();
		ImGui::NewLine();
	}

	ImGui::End();
}

void CTool_Effect::Create_Effect()
{
	// 생성
	if (m_pEffect != nullptr)
		return;

	m_pEffect = GI->Clone_GameObject(TEXT("Prototype_TempMeshEffect"), LAYER_TYPE::LAYER_EFFECT);
	GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_EFFECT, m_pEffect);

	Load_InfoEffect();
}

void CTool_Effect::Store_TransformEffect()
{
	if (m_pEffect == nullptr)
		return;

	CTransform* pTransform = m_pEffect->Get_Component<CTransform>(L"Com_Transform");

	pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fPosition[0], m_fPosition[1], m_fPosition[2], 1.f));
	pTransform->FixRotation(m_fRotation[0], m_fRotation[1], m_fRotation[2]);
	pTransform->Set_Scale(_float3(m_fScale[0], m_fScale[1], m_fScale[2]));
}

void CTool_Effect::Load_InfoEffect()
{
	if (m_pEffect == nullptr)
		return;

	CTransform* pTransform = m_pEffect->Get_Component<CTransform>(L"Com_Transform");

	_vector vPosition = pTransform->Get_State(CTransform::STATE_POSITION);
	m_fPosition[0] = XMVectorGetX(vPosition);
	m_fPosition[1] = XMVectorGetY(vPosition);
	m_fPosition[2] = XMVectorGetZ(vPosition);

	_vector vRotation = pTransform->Get_WorldRotation();
	m_fRotation[0] = XMVectorGetX(vRotation);
	m_fRotation[1] = XMVectorGetY(vRotation);
	m_fRotation[2] = XMVectorGetZ(vRotation);

	_float3 fScale = pTransform->Get_Scale();
	m_fScale[0] = fScale.x;
	m_fScale[1] = fScale.y;
	m_fScale[2] = fScale.z;

	m_tEffectInfo = static_cast<CEffect*>(m_pEffect)->Get_EffectDesc();


}

void CTool_Effect::Store_InfoEffect()
{
	if (m_pEffect == nullptr)
		return;

	m_tEffectInfo.strDiffuseTetextureName = Select_FolderName(m_iDiffuseFolderIndex);
	m_tEffectInfo.strAlphaTexturName      = Select_FolderName(m_iAlphaFolderIndex);

	static_cast<CEffect*>(m_pEffect)->Set_EffectDesc(m_tEffectInfo);
}

void CTool_Effect::Save_Effect(const char* pFileName)
{
}

void CTool_Effect::Load_Effect(const char* pFileName)
{
}

wstring CTool_Effect::Select_FolderName(_uint iFolderIndex)  
{
	wstring ProtoName;

	switch (iFolderIndex)
	{
	case 0:
		ProtoName = L"Prototype_Component_Texture_Effect_Aura";
		break;
	case 1:
		ProtoName = L"Prototype_Component_Texture_Effect_Crack";
		break;
	case 2:
		ProtoName = L"Prototype_Component_Texture_Effect_Decal";
		break;
	case 3:
		ProtoName = L"Prototype_Component_Texture_Effect_Fire";
		break;
	case 4:
		ProtoName = L"Prototype_Component_Texture_Effect_Flare";
		break;
	case 5:
		ProtoName = L"Prototype_Component_Texture_Effect_Glow";
		break;
	case 6:
		ProtoName = L"Prototype_Component_Texture_Effect_Hit";
		break;
	case 7:
		ProtoName = L"Prototype_Component_Texture_Effect_Ice";
		break;
	case 8:
		ProtoName = L"Prototype_Component_Texture_Effect_Image";
		break;
	case 9:
		ProtoName = L"Prototype_Component_Texture_Effect_Mask";
		break;
	case 10:
		ProtoName = L"Prototype_Component_Texture_Effect_MeshTrail";
		break;
	case 11:
		ProtoName = L"Prototype_Component_Texture_Effect_Noise";
		break;
	case 12:
		ProtoName = L"Prototype_Component_Texture_Effect_Object";
		break;
	case 13:
		ProtoName = L"Prototype_Component_Texture_Effect_Ring";
		break;
	case 14:
		ProtoName = L"Prototype_Component_Texture_Effect_Slash";
		break;
	case 15:
		ProtoName = L"Prototype_Component_Texture_Effect_Smoke";
		break;
	case 16:
		ProtoName = L"Prototype_Component_Texture_Effect_SubUV";
		break;
	case 17:
		ProtoName = L"Prototype_Component_Texture_Effect_SwordTrail";
		break;
	case 18:
		ProtoName = L"Prototype_Component_Texture_Effect_UI";
		break;
	case 19:
		ProtoName = L"Prototype_Component_Texture_Effect_Water";
		break;
	case 20:
		ProtoName = L"Prototype_Component_Texture_Effect_Wind";
		break;
	case 21:
		ProtoName = L"";
		break;
	}

	return ProtoName;
}

CTool_Effect* CTool_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Effect* pInstance = new CTool_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Effect");
		return nullptr;
	}

	return pInstance;
}

void CTool_Effect::Free()
{
	__super::Free();
}
