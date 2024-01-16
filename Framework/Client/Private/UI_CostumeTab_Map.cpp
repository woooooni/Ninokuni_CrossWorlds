#include "stdafx.h"
#include "UI_CostumeTab_Map.h"
#include "GameInstance.h"

#include "Game_Manager.h"
#include "Player.h"
#include <filesystem>
#include "FileUtils.h"

CUI_CostumeTab_Map::CUI_CostumeTab_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"UI_CostumeTab_Map", OBJ_TYPE::OBJ_PROP)
{
}

CUI_CostumeTab_Map::CUI_CostumeTab_Map(const CUI_CostumeTab_Map& rhs)
	: CGameObject(rhs)
{

}

void CUI_CostumeTab_Map::Set_Active(_bool bActive)
{
	if (true == bActive)
		m_pTransformCom->LookAt_ForLandObject(m_vCamPosition);

	m_bActive = bActive;
}

HRESULT CUI_CostumeTab_Map::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	if (FAILED(Load_CostumeProps()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_CostumeTab_Map::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CPlayer* pPlayer = nullptr;
	pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pPlayer)
		return E_FAIL;
	CCharacter* pCharacter = nullptr;
	pCharacter = pPlayer->Get_Character();
	if (nullptr == pCharacter)
		return E_FAIL;

	_float3 vCamPos, vLook, vUp;

	switch (pCharacter->Get_CharacterType())
	{
	case CHARACTER_TYPE::SWORD_MAN:
		vCamPos = _float3(0.f, 0.9f, -3.f);
		vLook = _float3(0.f, 0.9, 0.f);
		vUp = _float3(0.f, 1.f, 0.f);
		break;

	case CHARACTER_TYPE::ENGINEER:
		vCamPos = _float3(0.f, 0.7f, -2.3f);
		vLook = _float3(0.f, 0.7, 0.f);
		vUp = _float3(0.f, 1.f, 0.f);
		break;

	case CHARACTER_TYPE::DESTROYER:
		vCamPos = _float3(0.f, 0.9f, -3.f);
		vLook = _float3(0.f, 0.9, 0.f);
		vUp = _float3(0.f, 1.f, 0.f);
		break;
	}

	m_vCamMatrix = XMMatrixLookAtLH(XMLoadFloat3(&vCamPos), XMLoadFloat3(&vLook), XMLoadFloat3(&vUp));
	m_vCamPosition = XMVectorSet(vCamPos.x, vCamPos.y, vCamPos.z, 1.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(m_vCamMatrix)); // 카메라 행렬을 전치시킴

	m_pTransformCom->Set_Scale(_float3(1.2f, 1.f, 1.2f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-0.3f, 0.f, 0.f, 1.f));
	m_pTransformCom->LookAt_ForLandObject(m_vCamPosition);

	return S_OK;
}

void CUI_CostumeTab_Map::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}

}

void CUI_CostumeTab_Map::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::LateTick(fTimeDelta);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_UI, this);
	}
}

HRESULT CUI_CostumeTab_Map::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_vCamPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint		iPassIndex = 0;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}


HRESULT CUI_CostumeTab_Map::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"),
		(CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"),
		(CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_CostumeTab_Props"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_CostumeTab_Map::Load_CostumeProps()
{
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_CostumeTab_Props", CModel::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/Map/CustomRoom/", L"CustomRoom")))
		return E_FAIL;

	return S_OK;
}

CUI_CostumeTab_Map* CUI_CostumeTab_Map::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_CostumeTab_Map* pInstance = new CUI_CostumeTab_Map(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CUI_CostumeTab_Map");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CUI_CostumeTab_Map::Clone(void* pArg)
{
	CUI_CostumeTab_Map* pInstance = new CUI_CostumeTab_Map(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_CostumeTab_Map");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CostumeTab_Map::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);

//	for (auto& iter : m_CostumeProps)
//	{
//		Safe_Release(iter);
//	}
}

