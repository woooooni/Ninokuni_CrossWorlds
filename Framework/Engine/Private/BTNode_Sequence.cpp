#include "BTNode_Sequence.h"

#include "BehaviorTree.h"

CBTNode_Sequence::CBTNode_Sequence()
{
}

HRESULT CBTNode_Sequence::Initialize_Prototype(CBehaviorTree* pBT)
{
    m_pBT = pBT;

	return S_OK;
}

void CBTNode_Sequence::Start()
{
}

CBTNode::NODE_STATE CBTNode_Sequence::Tick(const _float& fTimeDelta)
{
    for (auto iter : m_ltChildNode)
    {
        if (!iter->Get_IsStart())
        {
            iter->Start();
            iter->Set_IsStart(true);
        }
    
        switch (iter->Tick(fTimeDelta))
        {
        case NODE_STATE::NODE_RUNNING:
            return NODE_STATE::NODE_RUNNING;
        case NODE_STATE::NODE_SUCCESS:
            continue;
        case NODE_STATE::NODE_FAIL:
            for (auto iter : m_ltChildNode)
            {
                iter->Init_Success();
            }
            return NODE_STATE::NODE_FAIL;
        }
    }

    for (auto iter : m_ltChildNode)
    {
        iter->Init_Success();
        iter->Set_IsStart(false);
    }

    return NODE_STATE::NODE_SUCCESS;
}

CBTNode_Sequence* CBTNode_Sequence::Create(CBehaviorTree* pBT)
{
	CBTNode_Sequence* pInstance = new CBTNode_Sequence();

	if (FAILED(pInstance->Initialize_Prototype(pBT)))
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
