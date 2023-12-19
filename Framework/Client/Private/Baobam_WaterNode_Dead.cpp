#include "stdafx.h"
#include "Baobam_WaterNode_Dead.h"

#include "BehaviorTree.h"
#include "Model.h"

CBaobam_WaterNode_Dead::CBaobam_WaterNode_Dead()
{
}

HRESULT CBaobam_WaterNode_Dead::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_WaterNode_Dead::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_Death"));
}

CBTNode::NODE_STATE CBaobam_WaterNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pOwnerModel->Is_Finish())
	{
		// 여기서 객체 지우는 코드.
		// m_tBTNodeDesc.pOwner->Set_Dead(true);
		m_tBTNodeDesc.pOwner->Reserve_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CBaobam_WaterNode_Dead* CBaobam_WaterNode_Dead::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CBaobam_WaterNode_Dead* pInstance = new CBaobam_WaterNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_WaterNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_WaterNode_Dead::Free()
{
	__super::Free();
}
