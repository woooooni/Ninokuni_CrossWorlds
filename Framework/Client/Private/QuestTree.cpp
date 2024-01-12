#include "stdafx.h"
#include "QuestTree.h"

#include "BTNode_Select.h"

CQuestTree::CQuestTree()
{
}

HRESULT CQuestTree::Initialize()
{
	return S_OK;
}

_bool CQuestTree::Tick(const _float& fTimeDelta)
{
	return false;
}

void CQuestTree::LateTick(const _float& fTimeDelta)
{

}

void CQuestTree::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
	Safe_Release(m_pCurNode);
}
