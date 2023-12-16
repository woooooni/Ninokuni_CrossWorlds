#include "stdafx.h"
#include "Baobam_WaterNode_Chase.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CBaobam_WaterNode_Chase::CBaobam_WaterNode_Chase()
{
}

HRESULT CBaobam_WaterNode_Chase::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_WaterNode_Chase::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_BattleRun"));
}

CBTNode::NODE_STATE CBaobam_WaterNode_Chase::Tick(const _float& fTimeDelta)
{
	m_tBTNodeDesc.pOwnerTransform->LookAt(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerTransform->Move(m_tBTNodeDesc.pOwnerTransform->Get_Look(), 1.f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CBaobam_WaterNode_Chase* CBaobam_WaterNode_Chase::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CBaobam_WaterNode_Chase* pInstance = new CBaobam_WaterNode_Chase();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_WaterNode_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_WaterNode_Chase::Free()
{
	__super::Free();
}
