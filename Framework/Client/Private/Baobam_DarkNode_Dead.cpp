#include "stdafx.h"
#include "Baobam_DarkNode_Dead.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CBaobam_DarkNode_Dead::CBaobam_DarkNode_Dead()
{
}

HRESULT CBaobam_DarkNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_DarkNode_Dead::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Darkness.ao|BaoBam_Death"));
}

CBTNode::NODE_STATE CBaobam_DarkNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Finish())
	{
		m_tBTNodeDesc.pOwner->Set_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CBaobam_DarkNode_Dead* CBaobam_DarkNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CBaobam_DarkNode_Dead* pInstance = new CBaobam_DarkNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_DarkNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_DarkNode_Dead::Free()
{
	__super::Free();
}
