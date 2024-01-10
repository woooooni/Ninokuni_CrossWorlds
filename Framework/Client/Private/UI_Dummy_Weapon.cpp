#include "stdafx.h"
#include "UI_Dummy_Weapon.h"

#include "GameInstance.h"
#include "Weapon_Manager.h"

CUI_Dummy_Weapon::CUI_Dummy_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObejctTag)
	: CGameObject(pDevice, pContext, strObejctTag, OBJ_TYPE::OBJ_WEAPON)
{
}


HRESULT CUI_Dummy_Weapon::Set_Owner(CGameObject* pOwner, CHARACTER_TYPE eType)
{
	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_TEST || GI->Get_CurrentLevel() == LEVELID::LEVEL_TOOL)
		return S_OK;

	if (nullptr == pOwner)
		return E_FAIL;

	if (nullptr != m_pOwner)
		Safe_Release(m_pOwner);

	Safe_AddRef(m_pOwner);
	m_eCurCharacter = eType;

	return S_OK;
}

HRESULT CUI_Dummy_Weapon::Initialize(void* pArg)
{
	if(FAILED(Ready_Components()))
		return E_FAIL;

	_float3 vCamPos, vLook, vUp;

	switch (m_eCurCharacter)
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
	}

	m_vCamMatrix = XMMatrixLookAtLH(XMLoadFloat3(&vCamPos), XMLoadFloat3(&vLook), XMLoadFloat3(&vUp));
	m_vCamPosition = XMVectorSet(vCamPos.x, vCamPos.y, vCamPos.z, 1.f);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(m_vCamMatrix)); // 카메라 행렬을 전치시킴

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-0.3f, 0.f, 0.f, 1.f));
	m_pTransformCom->LookAt_ForLandObject(m_vCamPosition);

	m_bActive = true;

	return S_OK;
}

void CUI_Dummy_Weapon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_Dummy_Weapon::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pRendererCom || nullptr == m_pModelCom)
			return;

		m_pModelCom->LateTick(fTimeDelta);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_UI, this);
	}
}

HRESULT CUI_Dummy_Weapon::Render()
{
	if (FAILED(__super::Render() || nullptr == m_pModelCom))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Dummy_Weapon::Ready_Components()
{
	// Transform Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// Renderer Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// Shader Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Dummy_Weapon::Bind_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_vCamPosition, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_matSocketWorld.Transpose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	return S_OK;
}

CUI_Dummy_Weapon* CUI_Dummy_Weapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CUI_Dummy_Weapon* pInstance = new CUI_Dummy_Weapon(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CUI_Dummy_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dummy_Weapon::Clone(void* pArg)
{
	return nullptr;
}

void CUI_Dummy_Weapon::Free()
{
	__super::Free();

	Safe_Release(m_pOwner);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
