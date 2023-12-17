#include "stdafx.h"
#include "Shadow_ThiefNode_Skill1.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CShadow_ThiefNode_Skill1::CShadow_ThiefNode_Skill1()
{
}

HRESULT CShadow_ThiefNode_Skill1::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Skill1::Start()
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Skill01"));
	dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CShadow_ThiefNode_Skill1::Tick(const _float& fTimeDelta)
{
	// (현재 액션이 취하는 애니메이션, 동작이 끝나고 기다릴 시간, timeDelta)
	return __super::UnLoop_BehaviorTick(TEXT("SKM_ShadowThief.ao|ShadowThief_Skill01"), 2.f, fTimeDelta);
}

CShadow_ThiefNode_Skill1* CShadow_ThiefNode_Skill1::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CShadow_ThiefNode_Skill1* pInstance = new CShadow_ThiefNode_Skill1();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Skill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Skill1::Free()
{
	__super::Free();
}
