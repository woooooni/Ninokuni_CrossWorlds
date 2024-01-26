#include "stdafx.h"
#include "GameItem_Consumpsion.h"

CGameItem_Consumpsion::CGameItem_Consumpsion()
{
}

CGameItem_Consumpsion::CGameItem_Consumpsion(const CGameItem_Consumpsion& rhs)
{
}

HRESULT CGameItem_Consumpsion::Initialize(void* pArg)
{
	ITEMDESC_ETC tConsumeDesc = *(ITEMDESC_ETC*)pArg;
	
	m_tArmorDesc.eCode = tConsumeDesc.eCode;
	m_tArmorDesc.strName = tConsumeDesc.strName;
	m_tArmorDesc.strContent = tConsumeDesc.strContent;
	m_tArmorDesc.eMainCategory = tConsumeDesc.eMainCategory;
	m_tArmorDesc.eArmorCategory = tConsumeDesc.eArmorCategory;
	m_tArmorDesc.fDef = tConsumeDesc.fDef;

	return S_OK;
}

void CGameItem_Consumpsion::Tick(_float fTimeDelta)
{
}

CGameItem_Consumpsion* CGameItem_Consumpsion::Create(void* pArg)
{
	CGameItem_Consumpsion* pInstance = new CGameItem_Consumpsion();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Create : CGameItem_Consumpsion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameItem_Consumpsion::Free()
{
	__super::Free();
}
