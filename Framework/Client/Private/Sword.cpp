#include "stdafx.h"
#include "Sword.h"

#include "GameInstance.h"
#include "Collision_Manager.h"
#include "Weapon_Manager.h"

CSword::CSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CWeapon(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_WEAPON)
{

}


HRESULT CSword::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSword::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSword::LateTick(_float fTimeDelta)
{
	/* 무기의 오너가 해당 무기를 사용하기위한 소켓 행렬을 받아와서 무기의 Set_SocketWorld()를 해주어야 합니다. */
	/* Set_SocketWorld()는 모델의 소켓 월드행렬을 반환합니다. (해당 행렬에 무기의 월드행렬을 곱하면, 월드상에서의 무기 위치가 됩니다.)*/

	__super::LateTick(fTimeDelta);
}

HRESULT CSword::Render()
{
	if (FAILED(__super::Render() || nullptr == m_pModelCom))
		return E_FAIL;


	return S_OK;
}

void CSword::Collision_Enter(const COLLISION_INFO& tInfo)
{

}

void CSword::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CSword::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CSword::Ready_Components()
{
	/* Com_Model */
	

	return S_OK;
}

HRESULT CSword::Ready_Colliders()
{
	return S_OK;
}

HRESULT CSword::Bind_ShaderResources()
{
	return S_OK;
}


CSword* CSword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CSword* pInstance = new CSword(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CSword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSword::Clone(void* pArg)
{
	return nullptr;
}


void CSword::Free()
{
	__super::Free();
}
