#include "stdafx.h"
#include "StelliaNode_Charge.h"

CStelliaNode_Charge::CStelliaNode_Charge()
{
}

HRESULT CStelliaNode_Charge::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CStelliaNode_Charge::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06"));
}

CBTNode::NODE_STATE CStelliaNode_Charge::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	cout << "charge" << endl;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_Charge* CStelliaNode_Charge::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CStelliaNode_Charge* pInstance = new CStelliaNode_Charge();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_Charge::Free()
{
	__super::Free();
}
