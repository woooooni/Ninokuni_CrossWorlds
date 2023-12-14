#include "BTNode_Sequence.h"

CBTNode_Sequence::CBTNode_Sequence()
{
}

HRESULT CBTNode_Sequence::Initialize_Prototype()
{
	return S_OK;
}

CBTNode::NODE_STATE CBTNode_Sequence::Tick(const _float& fTimeDelta)
{
	_bool bIsRunning = false;

	for (auto iter : m_ltChildNode)
	{
		switch (iter->Tick(fTimeDelta))
		{
		case NODE_STATE::NODE_RUNNING:
			bIsRunning = true;
			break;
		case NODE_STATE::NODE_FAIL:
			return NODE_STATE::NODE_FAIL;
		}
	}

	return bIsRunning ? NODE_STATE::NODE_RUNNING : NODE_STATE::NODE_SUCCESS;
}

CBTNode_Sequence* CBTNode_Sequence::Create()
{
	CBTNode_Sequence* pInstance = new CBTNode_Sequence();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CBTNode_Sequence");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBTNode_Sequence::Free()
{
	__super::Free();
}
