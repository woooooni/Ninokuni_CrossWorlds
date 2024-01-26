#include "stdafx.h"
#include "GameItem.h"

CGameItem::CGameItem()
{
}

CGameItem::CGameItem(const CGameItem& rhs)
{
}

wstring CGameItem::Get_Content(const ITEM_CODE& eCode)
{

	return wstring();
}

wstring CGameItem::Get_Name(const ITEM_CODE& eCode)
{
	return wstring();
}

HRESULT CGameItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameItem::Initialize(void* pArg)
{
	return S_OK;
}

void CGameItem::Tick(_float fTimeDelta)
{
	if (false == m_bIsIn)
		return;
}

void CGameItem::Free()
{
	__super::Free();
}
