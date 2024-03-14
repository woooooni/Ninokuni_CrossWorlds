#include "stdafx.h"
#include "Criminal_MonsterNode_Attack2.h"

#include "BehaviorTree.h"
#include "Model.h"

CCriminal_MonsterNode_Attack2::CCriminal_MonsterNode_Attack2()
{
}

HRESULT CCriminal_MonsterNode_Attack2::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CCriminal_MonsterNode_Attack2::Start()
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|Attack2"));
	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CCriminal_MonsterNode_Attack2::Tick(const _float& fTimeDelta)
{
	// (현재 액션이 취하는 애니메이션, 동작이 끝나고 기다릴 시간, timeDelta)
	return __super::Atk_BehaviorTick(1.f, fTimeDelta);
}

CCriminal_MonsterNode_Attack2* CCriminal_MonsterNode_Attack2::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CCriminal_MonsterNode_Attack2* pInstance = new CCriminal_MonsterNode_Attack2();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CCriminal_MonsterNode_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCriminal_MonsterNode_Attack2::Free()
{
	__super::Free();
}
