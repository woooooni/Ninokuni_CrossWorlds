#include "BTNode_Select.h"

CBTNode_Select::CBTNode_Select()
{
}

HRESULT CBTNode_Select::Initialize_Prototype(CBehaviorTree* pBT)
{
	m_pBT = pBT;

	return S_OK;
}

void CBTNode_Select::Start()
{
}

CBTNode::NODE_STATE CBTNode_Select::Tick(const _float& fTimeDelta)
{
	for (auto iter : m_ltChildNode)
	{
		switch (iter->Tick(fTimeDelta))
		{
		case NODE_STATE::NODE_RUNNING:
			return NODE_STATE::NODE_RUNNING;
		case NODE_STATE::NODE_SUCCESS:
			return NODE_STATE::NODE_SUCCESS;
		}
	}

	return NODE_STATE::NODE_FAIL;
}

CBTNode_Select* CBTNode_Select::Create(CBehaviorTree* pBT)
{
	CBTNode_Select* pInstance = new CBTNode_Select();

	if (FAILED(pInstance->Initialize_Prototype(pBT)))
	{
		MSG_BOX("Fail Create : CBTNode_Select");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBTNode_Select::Free()
{
	__super::Free();
}
