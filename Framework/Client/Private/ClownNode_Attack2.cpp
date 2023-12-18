#include "stdafx.h"
#include "ClownNode_Attack2.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CClownNode_Attack2::CClownNode_Attack2()
{
}

HRESULT CClownNode_Attack2::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClownNode_Attack2::Start()
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Clown.ao|Clown_Attack02"));
	dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CClownNode_Attack2::Tick(const _float& fTimeDelta)
{
	return __super::Atk_BehaviorTick(TEXT("SKM_Clown.ao|Clown_Attack02"), 1.f, fTimeDelta);
}

CClownNode_Attack2* CClownNode_Attack2::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CClownNode_Attack2* pInstance = new CClownNode_Attack2();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClownNode_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClownNode_Attack2::Free()
{
	__super::Free();
}
