#include "stdafx.h"
#include "NpcNode_Base.h"

CNpcNode_Base::CNpcNode_Base()
{
}

HRESULT CNpcNode_Base::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CNpcNode_Base::Start()
{
}

CBTNode::NODE_STATE CNpcNode_Base::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

_bool CNpcNode_Base::Behavior_Wait(_float fDestWaitTime, const _float& fTimeDelta)
{
	m_fSourWaitTime += fTimeDelta;

	if (m_fSourWaitTime > fDestWaitTime)
	{
		m_fSourWaitTime = fDestWaitTime - m_fSourWaitTime;
		return true;
	}

	return false;
}

void CNpcNode_Base::Free()
{
}
