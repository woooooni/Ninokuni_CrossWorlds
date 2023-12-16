#include "stdafx.h"
#include "StelliaNode_ThirdLaser.h"

CStelliaNode_ThirdLaser::CStelliaNode_ThirdLaser()
{
}

HRESULT CStelliaNode_ThirdLaser::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CStelliaNode_ThirdLaser::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill03"));
}

CBTNode::NODE_STATE CStelliaNode_ThirdLaser::Tick(const _float& fTimeDelta)
{
	if (m_bIsSucces)
		return NODE_STATE::NODE_SUCCESS;

	cout << "thirdlaser" << endl;

	if (m_tBTNodeDesc.pOwnerModel->Is_Finish() && m_tBTNodeDesc.pOwnerModel->Is_Tween())
		m_bIsSucces = true;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_ThirdLaser* CStelliaNode_ThirdLaser::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CStelliaNode_ThirdLaser* pInstance = new CStelliaNode_ThirdLaser();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_ThirdLaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_ThirdLaser::Free()
{
	__super::Free();
}
