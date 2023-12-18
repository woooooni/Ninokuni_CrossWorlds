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
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Water.ao|BaoBam_Attack02"));
	dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CBaobam_WaterNode_Attack2::Tick(const _float& fTimeDelta)
{
	// (현재 액션이 취하는 애니메이션, 동작이 끝나고 기다릴 시간, timeDelta)
	return __super::Atk_BehaviorTick(TEXT("SKM_Baobam_Water.ao|BaoBam_Attack02"), 1.f, fTimeDelta);
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
