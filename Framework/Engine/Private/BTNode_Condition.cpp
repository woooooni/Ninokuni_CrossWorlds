#include "BTNode_Condition.h"

CBTNode_Condition::CBTNode_Condition()
{
}

HRESULT CBTNode_Condition::Initialize_Prototype(function<_bool()> conditionFunc)
{
	m_conditionFunc = conditionFunc;

	return S_OK;
}

CBTNode::NODE_STATE CBTNode_Condition::Tick(const _float& fTimeDelta)
{
	if (m_conditionFunc() == true)
		return  NODE_STATE::NODE_SUCCESS;
	else
		return NODE_STATE::NODE_FAIL;
}

CBTNode_Condition* CBTNode_Condition::Create(function<_bool()> conditionFunc)
{
	CBTNode_Condition* pInstance = new CBTNode_Condition();

	if (FAILED(pInstance->Initialize_Prototype(conditionFunc)))
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
