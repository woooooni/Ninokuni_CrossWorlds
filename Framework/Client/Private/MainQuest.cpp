#include "stdafx.h"
#include "MainQuest.h"

#include "MainQuest01.h"

#include "GameInstance.h"

CMainQuest::CMainQuest()
{
}

HRESULT CMainQuest::Initialize()
{
	m_vecMQ.push_back(CMainQuest01::Create());

	return S_OK;
}

void CMainQuest::Tick(const _float& fTimeDelta)
{
	if (m_iLevel < m_vecMQ.size())
	{
		if (m_vecMQ[m_iLevel]->Tick(fTimeDelta))
		{
			m_iLevel += 1;
		}
	}
}

void CMainQuest::LateTick(const _float& fTimeDelta)
{
	if (m_iLevel < m_vecMQ.size())
	{
		m_vecMQ[m_iLevel]->LateTick(fTimeDelta);
	}
}

CMainQuest* CMainQuest::Create()
{
	CMainQuest* pInstance = new CMainQuest();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest::Free()
{
	__super::Free();

	for (_uint i = 0; i < m_vecMQ.size(); ++i)
	{
		Safe_Release(m_vecMQ[i]);
	}
}
