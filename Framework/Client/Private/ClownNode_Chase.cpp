#include "stdafx.h"
#include "ClownNode_Chase.h"

#include "BehaviorTree.h"
#include "Model.h"

CClownNode_Chase::CClownNode_Chase()
{
}

HRESULT CClownNode_Chase::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClownNode_Chase::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Clown.ao|Clown_BattleRun"));
}

CBTNode::NODE_STATE CClownNode_Chase::Tick(const _float& fTimeDelta)
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerTransform->Move(m_tBTNodeDesc.pOwnerTransform->Get_Look(), 3.5f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CClownNode_Chase* CClownNode_Chase::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CClownNode_Chase* pInstance = new CClownNode_Chase();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClownNode_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClownNode_Chase::Free()
{
	__super::Free();
}
