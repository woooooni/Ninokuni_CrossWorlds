#include "stdafx.h"
#include "ClownNode_Dead.h"

#include "BehaviorTree.h"


#include "Model.h"

CClownNode_Dead::CClownNode_Dead()
{
}

HRESULT CClownNode_Dead::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClownNode_Dead::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Clown.ao|Clown_Death"));
}

CBTNode::NODE_STATE CClownNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
	{
		m_tBTMonsterDesc.pOwner->Reserve_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CClownNode_Dead* CClownNode_Dead::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CClownNode_Dead* pInstance = new CClownNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClownNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClownNode_Dead::Free()
{
	__super::Free();
}
