#include "stdafx.h"
#include "Criminal_MonsterNode_Chase.h"

#include "BehaviorTree.h"
#include "Model.h"

CCriminal_MonsterNode_Chase::CCriminal_MonsterNode_Chase()
{
}

HRESULT CCriminal_MonsterNode_Chase::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CCriminal_MonsterNode_Chase::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|Run"));
}

CBTNode::NODE_STATE CCriminal_MonsterNode_Chase::Tick(const _float& fTimeDelta)
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
	m_tBTMonsterDesc.pOwnerTransform->Move(m_tBTMonsterDesc.pOwnerTransform->Get_Look(), 4.f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CCriminal_MonsterNode_Chase* CCriminal_MonsterNode_Chase::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CCriminal_MonsterNode_Chase* pInstance = new CCriminal_MonsterNode_Chase();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CCriminal_MonsterNode_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCriminal_MonsterNode_Chase::Free()
{
	__super::Free();
}
