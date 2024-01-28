#include "stdafx.h"
#include "MainQuest_WitchForest.h"

#include "BTNode_Select.h"

CMainQuest_WitchForest::CMainQuest_WitchForest()
{
}

HRESULT CMainQuest_WitchForest::Initialize()
{
	__super::Initialize();

	m_pRootNode = CBTNode_Select::Create();

	return S_OK;
}

_bool CMainQuest_WitchForest::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_WitchForest::LateTick(const _float& fTimeDelta)
{
}

CMainQuest_WitchForest* CMainQuest_WitchForest::Create()
{
	CMainQuest_WitchForest* pInstance = new CMainQuest_WitchForest();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_WitchForest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_WitchForest::Free()
{
	__super::Free();
}

