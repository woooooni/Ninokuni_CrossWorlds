#include "stdafx.h"
#include "IceBearManNode_Attack2.h"

#include "BehaviorTree.h"
#include "Model.h"

CIceBearManNode_Attack2::CIceBearManNode_Attack2()
{
}

HRESULT CIceBearManNode_Attack2::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CIceBearManNode_Attack2::Start()
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Attack02"));
	dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CIceBearManNode_Attack2::Tick(const _float& fTimeDelta)
{
	// (현재 액션이 취하는 애니메이션, 동작이 끝나고 기다릴 시간, timeDelta)
	return __super::Atk_BehaviorTick(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Attack02"), 1.f, fTimeDelta);
}

CIceBearManNode_Attack2* CIceBearManNode_Attack2::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CIceBearManNode_Attack2* pInstance = new CIceBearManNode_Attack2();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CIceBearManNode_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBearManNode_Attack2::Free()
{
	__super::Free();
}
