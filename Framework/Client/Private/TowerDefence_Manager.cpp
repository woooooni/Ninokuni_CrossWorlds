#include "stdafx.h"
#include "GameInstance.h"
#include "TowerDefence_Manager.h"
#include "Utils.h"

IMPLEMENT_SINGLETON(CTowerDefence_Manager)

CTowerDefence_Manager::CTowerDefence_Manager()
{

}

HRESULT CTowerDefence_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (true == m_bReserved)
		return S_OK;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);



	m_bReserved = true;


	return S_OK;
}

void CTowerDefence_Manager::Tick(_float fTimeDelta)
{
	switch (m_eCurrentPhase)
	{
	case DEFENCE_PREPARE:
		Tick_Defence_Prepare(fTimeDelta);
		break;
	case DEFENCE_PROGRESS:
		Tick_Defence_Progress(fTimeDelta);
		break;
	case DEFENCE_FINISH:
		Tick_Defence_Finish(fTimeDelta);
		break;

	default:
		break;
	}
}

void CTowerDefence_Manager::LateTick(_float fTimeDelta)
{
	switch (m_eCurrentPhase)
	{
	case DEFENCE_PREPARE:
		LateTick_Defence_Prepare(fTimeDelta);
		break;
	case DEFENCE_PROGRESS:
		LateTick_Defence_Progress(fTimeDelta);
		break;
	case DEFENCE_FINISH:
		LateTick_Defence_Finish(fTimeDelta);
		break;

	default:
		break;
	}
}

void CTowerDefence_Manager::Prepare_Defence()
{
	m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_PREPARE;
}

void CTowerDefence_Manager::Start_Defence()
{
	m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_PROGRESS;
}

void CTowerDefence_Manager::Finish_Defence()
{
	m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_FINISH;
}

void CTowerDefence_Manager::End_Defence()
{
	m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_END;
}

void CTowerDefence_Manager::Tick_Defence_Prepare(_float fTimeDelta)
{
}

void CTowerDefence_Manager::Tick_Defence_Progress(_float fTimeDelta)
{
}

void CTowerDefence_Manager::Tick_Defence_Finish(_float fTimeDelta)
{
}

void CTowerDefence_Manager::LateTick_Defence_Prepare(_float fTimeDelta)
{
}

void CTowerDefence_Manager::LateTick_Defence_Progress(_float fTimeDelta)
{
}

void CTowerDefence_Manager::LateTick_Defence_Finish(_float fTimeDelta)
{
}



void CTowerDefence_Manager::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}



