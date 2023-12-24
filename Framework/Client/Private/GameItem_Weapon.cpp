#include "stdafx.h"
#include "GameItem_Weapon.h"

CGameItem_Weapon::CGameItem_Weapon()
{
}

CGameItem_Weapon::CGameItem_Weapon(const CGameItem_Weapon& rhs)
{
}

HRESULT CGameItem_Weapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameItem_Weapon::Initialize(void* pArg)
{
	ITEMDESC_WEAPON tWeaponDesc = *(ITEMDESC_WEAPON*)pArg;

	m_tWeaponDesc.eCode = tWeaponDesc.eCode;
	m_tWeaponDesc.strName = tWeaponDesc.strName;
	m_tWeaponDesc.strContent = tWeaponDesc.strContent;
	m_tWeaponDesc.eMainCategory = tWeaponDesc.eMainCategory;
	m_tWeaponDesc.eWeaponCategory = tWeaponDesc.eWeaponCategory;

	return S_OK;
}

void CGameItem_Weapon::Tick(_float fTimeDelta)
{
}

CGameItem_Weapon* CGameItem_Weapon::Create()
{
	CGameItem_Weapon* pInstance = new CGameItem_Weapon();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGameItem_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameItem* CGameItem_Weapon::Clone(void* pArg)
{
	CGameItem_Weapon* pInstance = new CGameItem_Weapon();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGameItem_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameItem_Weapon::Free()
{
	__super::Free();
}
