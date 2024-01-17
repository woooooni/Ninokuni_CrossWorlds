#include "stdafx.h"
#include "ClownNode_Attack2.h"

#include "BehaviorTree.h"
#include "Model.h"

CClownNode_Attack2::CClownNode_Attack2()
{
}

HRESULT CClownNode_Attack2::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClownNode_Attack2::Start()
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Clown.ao|Clown_Attack02"));
	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CClownNode_Attack2::Tick(const _float& fTimeDelta)
{
	return __super::Atk_BehaviorTick(1.f, fTimeDelta);
}

CClownNode_Attack2* CClownNode_Attack2::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
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
