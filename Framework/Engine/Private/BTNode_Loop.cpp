#include "BTNode_Loop.h"

CBTNode_Loop::CBTNode_Loop()
{
}

HRESULT CBTNode_Loop::Initialize_Prototype(function<_bool()> conditionFunc)
{
	m_conditionFunc = conditionFunc;

	return S_OK;
}

void CBTNode_Loop::Start()
{
}

CBTNode::NODE_STATE CBTNode_Loop::Tick(const _float& fTimeDelta)
{
	if (m_conditionFunc() == true)
	{
		for (auto iter : m_ltChildNode)
		{
			return iter->Tick(fTimeDelta);
		}
	}
}

CBTNode_Loop* CBTNode_Loop::Create(function<_bool()> conditionFunc)
{
	CBTNode_Loop* pInstance = new CBTNode_Loop();

	if (FAILED(pInstance->Initialize_Prototype(conditionFunc)))
	{
		MSG_BOX("Fail Create : CBTNode_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBTNode_Loop::Free()
{
	__super::Free();
}
