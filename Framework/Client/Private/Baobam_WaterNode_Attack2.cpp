#include "stdafx.h"
#include "Baobam_WaterNode_Attack2.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CBaobam_WaterNode_Attack2::CBaobam_WaterNode_Attack2()
{
}

HRESULT CBaobam_WaterNode_Attack2::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_WaterNode_Attack2::Start()
{
	m_tBTNodeDesc.pOwnerTransform->LookAt(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_Attack02"));
}

CBTNode::NODE_STATE CBaobam_WaterNode_Attack2::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && !m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;

	return NODE_STATE::NODE_RUNNING;
}

CBaobam_WaterNode_Attack2* CBaobam_WaterNode_Attack2::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CBaobam_WaterNode_Attack2* pInstance = new CBaobam_WaterNode_Attack2();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_WaterNode_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_WaterNode_Attack2::Free()
{
	__super::Free();
}
