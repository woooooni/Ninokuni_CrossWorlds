#include "stdafx.h"
#include "..\Public\Weapon.h"

#include "GameInstance.h"

CWeapon::CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObejctTag, _uint iObjectType)
	: CGameObject(pDevice, pContext, strObejctTag, iObjectType)
{

}



HRESULT CWeapon::Set_Owner(CGameObject* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	if (nullptr != m_pOwner)
		Safe_Release(m_pOwner);

	Safe_AddRef(m_pOwner);

	return S_OK;
}

HRESULT CWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	//if (FAILED(Ready_Components()))
	//	return E_FAIL;

	return S_OK;
}

void CWeapon::Tick(_float fTimeDelta)
{
}

void CWeapon::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom || nullptr == m_pModelCom)
		return;

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pModelCom->LateTick(fTimeDelta);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif // DEBUG

}

HRESULT CWeapon::Render()
{
	if (FAILED(__super::Render() || nullptr == m_pModelCom))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint		iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CWeapon::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon::Bind_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	Matrix matWorld = m_matSocketWorld * m_pTransformCom->Get_WorldMatrix();

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &matWorld.Transpose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	return S_OK;
}

void CWeapon::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pOwner);
}
