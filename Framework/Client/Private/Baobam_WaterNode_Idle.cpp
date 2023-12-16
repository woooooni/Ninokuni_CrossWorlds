#include "stdafx.h"
#include "Baobam_WaterNode_Idle.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CBaobam_WaterNode_Idle::CBaobam_WaterNode_Idle()
{
}

HRESULT CBaobam_WaterNode_Idle::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_WaterNode_Idle::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_BattleStand"));
}

CBTNode::NODE_STATE CBaobam_WaterNode_Idle::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

CBaobam_WaterNode_Idle* CBaobam_WaterNode_Idle::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CBaobam_WaterNode_Idle* pInstance = new CBaobam_WaterNode_Idle();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_WaterNode_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_WaterNode_Idle::Free()
{
	__super::Free();
}
