#include "stdafx.h"
#include "Equipment_Weapon.h"

CEquipment_Weapon::CEquipment_Weapon()
{
}

CEquipment_Weapon::CEquipment_Weapon(const CEquipment_Weapon& rhs)
{
}

HRESULT CEquipment_Weapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEquipment_Weapon::Initialize(void* pArg)
{
	return S_OK;
}

void CEquipment_Weapon::Tick(_float fTimeDelta)
{
}

void CEquipment_Weapon::Free()
{
	__super::Free();
}
