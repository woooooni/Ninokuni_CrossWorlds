#include "stdafx.h"
#include "NpcWeapon_Halberd.h"

#include "GameInstance.h"
#include "Collision_Manager.h"

CNpcWeapon_Halberd::CNpcWeapon_Halberd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CWeapon(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_WEAPON)
{

}

CNpcWeapon_Halberd::CNpcWeapon_Halberd(const CNpcWeapon_Halberd& rhs)
	: CWeapon(rhs)
{

}

HRESULT CNpcWeapon_Halberd::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpcWeapon_Halberd::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CNpcWeapon_Halberd::Tick(_float fTimeDelta)
{

}

void CNpcWeapon_Halberd::LateTick(_float fTimeDelta)
{
	/* 무기의 오너가 해당 무기를 사용하기위한 소켓 행렬을 받아와서 무기의 Set_SocketWorld()를 해주어야 합니다. */
	/* Set_SocketWorld()는 모델의 소켓 월드행렬을 반환합니다. (해당 행렬에 무기의 월드행렬을 곱하면, 월드상에서의 무기 위치가 됩니다.)*/

	__super::LateTick(fTimeDelta);
}

HRESULT CNpcWeapon_Halberd::Render()
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

void CNpcWeapon_Halberd::Collision_Enter(const COLLISION_INFO& tInfo)
{

}

void CNpcWeapon_Halberd::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CNpcWeapon_Halberd::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CNpcWeapon_Halberd::Ready_Trails()
{
	return S_OK;
}

HRESULT CNpcWeapon_Halberd::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;
	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_NpcWeapon_Halberd"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNpcWeapon_Halberd::Ready_Colliders()
{
	return S_OK;
}

HRESULT CNpcWeapon_Halberd::Bind_ShaderResources()
{
	return S_OK;
}


CNpcWeapon_Halberd* CNpcWeapon_Halberd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CNpcWeapon_Halberd* pInstance = new CNpcWeapon_Halberd(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNpcWeapon_Halberd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNpcWeapon_Halberd::Clone(void* pArg)
{
	CNpcWeapon_Halberd* pInstance = new CNpcWeapon_Halberd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNpcWeapon_Halberd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcWeapon_Halberd::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);

}

