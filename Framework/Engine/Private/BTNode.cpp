#include "BTNode.h"

#include "BehaviorTree.h"

CBTNode::CBTNode()
{
}

HRESULT CBTNode::Initialize_Prototype(CBehaviorTree* pBT)
{

	return S_OK;
}

CBTNode::NODE_STATE CBTNode::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_FAIL;
}

void CBTNode::Add_ChildNode(CBTNode* pNode)
{
	m_ltChildNode.push_back(pNode);
}

void CBTNode::Free()
{
	__super::Free();

	for (auto& iter : m_ltChildNode)
		Safe_Release(iter);
}
