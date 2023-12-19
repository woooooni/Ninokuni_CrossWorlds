#include "stdafx.h"
#include "Baobam_DarkNode_Attack2.h"

#include "BehaviorTree.h"
#include "Model.h"

CBaobam_DarkNode_Attack2::CBaobam_DarkNode_Attack2()
{
}

HRESULT CBaobam_DarkNode_Attack2::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_DarkNode_Attack2::Start()
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Darkness.ao|BaoBam_Attack02"));
	dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CBaobam_DarkNode_Attack2::Tick(const _float& fTimeDelta)
{
	return __super::Atk_BehaviorTick(TEXT("SKM_Baobam_Darkness.ao|BaoBam_Attack02"), 1.f, fTimeDelta);
}

CBaobam_DarkNode_Attack2* CBaobam_DarkNode_Attack2::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CBaobam_DarkNode_Attack2* pInstance = new CBaobam_DarkNode_Attack2();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_DarkNode_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_DarkNode_Attack2::Free()
{
	__super::Free();
}
