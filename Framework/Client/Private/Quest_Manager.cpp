#include "stdafx.h"
#include "Quest_Manager.h"

#include "MainQuest.h"

IMPLEMENT_SINGLETON(CQuest_Manager)

CQuest_Manager::CQuest_Manager()
{

}

HRESULT CQuest_Manager::Reserve_Manager()
{
	m_pMainQuest = CMainQuest::Create();

	return S_OK;
}

void CQuest_Manager::Tick(_float fTimeDelta)
{
	if (m_bIsRunning)
	{
		m_pMainQuest->Tick(fTimeDelta);
	}
}

void CQuest_Manager::LateTick(_float fTimeDelta)
{
	if (m_bIsRunning)
	{
		m_pMainQuest->LateTick(fTimeDelta);
	}
}

void CQuest_Manager::Free()
{
	Safe_Release(m_pMainQuest);
}
