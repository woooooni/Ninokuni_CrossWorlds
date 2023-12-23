#include "stdafx.h"
#include "GameItem_Equipment.h"

CGameItem_Equipment::CGameItem_Equipment()
{
}

CGameItem_Equipment::CGameItem_Equipment(const CGameItem_Equipment& rhs)
{
}

HRESULT CGameItem_Equipment::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameItem_Equipment::Initialize(void* pArg)
{
	return S_OK;
}

void CGameItem_Equipment::Tick(_float fTimeDelta)
{
}

void CGameItem_Equipment::Free()
{
	__super::Free();
}
