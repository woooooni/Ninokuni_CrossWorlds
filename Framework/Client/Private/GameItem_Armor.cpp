#include "stdafx.h"
#include "GameItem_Armor.h"

CGameItem_Armor::CGameItem_Armor()
{
}

CGameItem_Armor::CGameItem_Armor(const CGameItem_Armor& rhs)
{
}

HRESULT CGameItem_Armor::Initialize(void* pArg)
{
	ITEMDESC_ARMOR tArmorDesc = *(ITEMDESC_ARMOR*)pArg;

	m_tArmorDesc.eCode = tArmorDesc.eCode;
	m_tArmorDesc.strName = tArmorDesc.strName;
	m_tArmorDesc.strContent = tArmorDesc.strContent;
	m_tArmorDesc.eMainCategory = tArmorDesc.eMainCategory;
	m_tArmorDesc.eArmorCategory = tArmorDesc.eArmorCategory;
	m_tArmorDesc.fDef = tArmorDesc.fDef;

	return S_OK;
}

void CGameItem_Armor::Tick(_float fTimeDelta)
{
}

CGameItem_Armor* CGameItem_Armor::Create(void* pArg)
{
	CGameItem_Armor* pInstance = new CGameItem_Armor();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Create : CGameItem_Armor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameItem_Armor::Free()
{
	__super::Free();
}
