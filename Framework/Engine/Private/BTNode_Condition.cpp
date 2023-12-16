#include "BTNode_Condition.h"

CBTNode_Condition::CBTNode_Condition()
{
}

HRESULT CBTNode_Condition::Initialize_Prototype(function<_bool()> conditionFunc, CBTNode* pPairNode, CBTNode* pNextNode)
{
	m_conditionFunc = conditionFunc;

	m_pPairNode = pPairNode;
	m_pNextNode = pNextNode;

	return S_OK;
}

void CBTNode_Condition::Start()
{
}

CBTNode::NODE_STATE CBTNode_Condition::Tick(const _float& fTimeDelta)
{
	if (m_conditionFunc() == true)
	{
		m_pNextNode->Set_IsStart(false);
		return  NODE_STATE::NODE_SUCCESS;
	}
	else
	{
		if(m_pPairNode != nullptr)
			m_pPairNode->Set_IsStart(false);
		return NODE_STATE::NODE_FAIL;
	}
}

CBTNode_Condition* CBTNode_Condition::Create(function<_bool()> conditionFunc, CBTNode* pPairNode, CBTNode* pNextNode)
{
	CBTNode_Condition* pInstance = new CBTNode_Condition();

	if (FAILED(pInstance->Initialize_Prototype(conditionFunc, pPairNode, pNextNode)))
	{
		MSG_BOX("Fail Create : CBTNode_Condition");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBTNode_Condition::Free()
{
	__super::Free();
}
