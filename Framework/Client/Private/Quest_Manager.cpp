#include "stdafx.h"
#include "Quest_Manager.h"

#include "MainQuest.h"
#include "SubQuest.h"

IMPLEMENT_SINGLETON(CQuest_Manager)

CQuest_Manager::CQuest_Manager()
{

}

HRESULT CQuest_Manager::Reserve_Manager()
{
	m_pMainQuest = CMainQuest::Create();
	m_pSubQuest = CSubQuest::Create();

	return S_OK;
}

void CQuest_Manager::Tick(_float fTimeDelta)
{
	if (m_bIsRunning)
	{
		m_pMainQuest->Tick(fTimeDelta);
		m_pSubQuest->Tick(fTimeDelta);
	}
}

void CQuest_Manager::LateTick(_float fTimeDelta)
{
	if (m_bIsRunning)
	{
		m_pMainQuest->LateTick(fTimeDelta);
		m_pSubQuest->LateTick(fTimeDelta);
	}
}

void CQuest_Manager::Set_SubQuestRunning(CSubQuest::SUBQUEST_NAME eSQName, _bool bIsRunning)
{
	m_pSubQuest->Set_SubQuestRunning(eSQName, bIsRunning);
}

void CQuest_Manager::Free()
{
	Safe_Release(m_pMainQuest);
	Safe_Release(m_pSubQuest);
}
