#include "BTNode.h"

#include "BehaviorTree.h"

CBTNode::CBTNode()
{
}

HRESULT CBTNode::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	m_tBTNodeDesc = *pDesc;
	m_pBT = pBT;
	return S_OK;
}

CBTNode::NODE_STATE CBTNode::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_FAIL;
}

void CBTNode::Init_Start()
{
	if (m_pBT != nullptr)
	{
		if (this != m_pBT->Get_CurNode())
			m_bIsStart = false;

		for (auto iter : m_ltChildNode)
			iter->Init_Start();
	}
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
