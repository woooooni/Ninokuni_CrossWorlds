#include "BTNode_Leaf.h"

CBTNode_Leaf::CBTNode_Leaf()
{
}

HRESULT CBTNode_Leaf::Initialize_Prototype(CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pBT);

	return S_OK;
}

void CBTNode_Leaf::Start()
{
}

CBTNode::NODE_STATE CBTNode_Leaf::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

void CBTNode_Leaf::Free()
{
	__super::Free();
}
