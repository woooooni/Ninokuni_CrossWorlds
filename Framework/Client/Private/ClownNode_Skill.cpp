#include "stdafx.h"
#include "ClownNode_Skill.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CClownNode_Skill::CClownNode_Skill()
{
}

HRESULT CClownNode_Skill::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClownNode_Skill::Start()
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Clown.ao|Clown_Skill01"));
	dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CClownNode_Skill::Tick(const _float& fTimeDelta)
{
	return __super::Atk_BehaviorTick(TEXT("SKM_Clown.ao|Clown_Skill01"), 1.f, fTimeDelta);
}

CClownNode_Skill* CClownNode_Skill::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CClownNode_Skill* pInstance = new CClownNode_Skill();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClownNode_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClownNode_Skill::Free()
{
	__super::Free();
}
