#include "stdafx.h"
#include "Baobam_DarkNode_Chase.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CBaobam_DarkNode_Chase::CBaobam_DarkNode_Chase()
{
}

HRESULT CBaobam_DarkNode_Chase::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_DarkNode_Chase::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Darkness.ao|BaoBam_BattleRun"));
}

CBTNode::NODE_STATE CBaobam_DarkNode_Chase::Tick(const _float& fTimeDelta)
{
	m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTNodeDesc.pTargetTransform->Get_Position());
	m_tBTNodeDesc.pOwnerTransform->Move(m_tBTNodeDesc.pOwnerTransform->Get_Look(), 2.f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CBaobam_DarkNode_Chase* CBaobam_DarkNode_Chase::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CBaobam_DarkNode_Chase* pInstance = new CBaobam_DarkNode_Chase();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_DarkNode_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_DarkNode_Chase::Free()
{
	__super::Free();
}
