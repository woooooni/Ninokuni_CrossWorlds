#include "stdafx.h"
#include "ClownNode_Dead.h"

#include "BehaviorTree.h"


#include "Model.h"

CClownNode_Dead::CClownNode_Dead()
{
}

HRESULT CClownNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClownNode_Dead::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Clown.ao|Clown_Death"));
}

CBTNode::NODE_STATE CClownNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Finish())
	{
		// 여기서 객체 지우는 코드.
		m_tBTNodeDesc.pOwner->Set_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CClownNode_Dead* CClownNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CClownNode_Dead* pInstance = new CClownNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClownNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClownNode_Dead::Free()
{
	__super::Free();
}
