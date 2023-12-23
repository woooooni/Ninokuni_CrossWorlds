#include "stdafx.h"
#include "GameItem.h"

CGameItem::CGameItem()
{
}

CGameItem::CGameItem(const CGameItem& rhs)
{
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
}

void CGameItem::Free()
{
	__super::Free();
}
