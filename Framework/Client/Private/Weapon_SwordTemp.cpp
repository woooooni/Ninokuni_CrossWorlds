#include "stdafx.h"
#include "..\Public\Weapon_SwordTemp.h"

#include "GameInstance.h"
#include "Collision_Manager.h"

CWeapon_SwordTemp::CWeapon_SwordTemp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CWeapon(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_WEAPON)
{

}

CWeapon_SwordTemp::CWeapon_SwordTemp(const CWeapon_SwordTemp& rhs)
	: CWeapon(rhs)
{

}

HRESULT CWeapon_SwordTemp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon_SwordTemp::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CWeapon_SwordTemp::Tick(_float fTimeDelta)
{
	
}

void CWeapon_SwordTemp::LateTick(_float fTimeDelta)
{
	/* 무기의 오너가 해당 무기를 사용하기위한 소켓 행렬을 받아와서 무기의 Set_SocketWorld()를 해주어야 합니다. */
	/* Set_SocketWorld()는 모델의 소켓 월드행렬을 반환합니다. (해당 행렬에 무기의 월드행렬을 곱하면, 월드상에서의 무기 위치가 됩니다.)*/
	
	__super::LateTick(fTimeDelta);
}

HRESULT CWeapon_SwordTemp::Render()
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

void CWeapon_SwordTemp::Collision_Enter(const COLLISION_INFO& tInfo)
{

}

void CWeapon_SwordTemp::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CWeapon_SwordTemp::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CWeapon_SwordTemp::Ready_Trails()
{
	return S_OK;
}

HRESULT CWeapon_SwordTemp::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_TempSword"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_SwordTemp::Ready_Colliders()
{
	return S_OK;
}

HRESULT CWeapon_SwordTemp::Bind_ShaderResources()
{
	return S_OK;
}


CWeapon_SwordTemp* CWeapon_SwordTemp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CWeapon_SwordTemp* pInstance = new CWeapon_SwordTemp(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_SwordTemp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_SwordTemp::Clone(void* pArg)
{
	CWeapon_SwordTemp* pInstance = new CWeapon_SwordTemp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_SwordTemp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_SwordTemp::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);

}
