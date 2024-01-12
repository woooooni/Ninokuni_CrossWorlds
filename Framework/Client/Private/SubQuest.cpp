#include "stdafx.h"
#include "SubQuest.h"

#include "SubQuest_FindCat.h"
#include "SubQuest_Windmill.h"
#include "SubQuest_Wanted.h"

#include "GameInstance.h"

CSubQuest::CSubQuest()
{
}

HRESULT CSubQuest::Initialize()
{
	m_vecSQ.push_back(CSubQuest_FindCat::Create());
	m_vecSQ.push_back(CSubQuest_Windmill::Create());
	m_vecSQ.push_back(CSubQuest_Wanted::Create());

	return S_OK;
}

void CSubQuest::Tick(const _float& fTimeDelta)
{
	for (_int i = 0; i < m_vecSQ.size(); ++i)
	{
		if (m_vecSQ[i]->Get_IsRunningQuest() && !m_vecSQ[i]->Get_IsClearQuest())
			m_vecSQ[i]->Tick(fTimeDelta);
	}
}

void CSubQuest::LateTick(const _float& fTimeDelta)
{
	for (_int i = 0; i < m_vecSQ.size(); ++i)
	{
		if (m_vecSQ[i]->Get_IsRunningQuest() && !m_vecSQ[i]->Get_IsClearQuest())
			m_vecSQ[i]->LateTick(fTimeDelta);
	}
}

void CSubQuest::Set_SubQuestRunning(SUBQUEST_NAME eSQName, _bool bIsRunning)
{
	m_vecSQ[eSQName]->Set_IsRunningQuest(bIsRunning);
}

CSubQuest* CSubQuest::Create()
{
	CSubQuest* pInstance = new CSubQuest();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuest::Free()
{
	__super::Free();

	for (_uint i = 0; i < m_vecSQ.size(); ++i)
	{
		Safe_Release(m_vecSQ[i]);
	}
}
