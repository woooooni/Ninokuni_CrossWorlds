#include "stdafx.h"
#include "QuestNode_Base.h"
#include "..\Public\QuestNode_Base.h"

CQuestNode_Base::CQuestNode_Base()
{
}

HRESULT CQuestNode_Base::Initialize()
{
	return S_OK;
}

void CQuestNode_Base::Start()
{
}

CBTNode::NODE_STATE CQuestNode_Base::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

void CQuestNode_Base::LateTick(const _float& fTimeDelta)
{
}

void CQuestNode_Base::Free()
{
	__super::Free();

	Safe_Release(m_pQuestDestSpot);
}

