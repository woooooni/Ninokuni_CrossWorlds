#include "stdafx.h"
#include "StelliaNode_Dead.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

CStelliaNode_Dead::CStelliaNode_Dead()
{
}

HRESULT CStelliaNode_Dead::Initialize_Prototype(CBehaviorTree* pBT)
{
	m_pBT = pBT;

	return S_OK;
}

CBTNode::NODE_STATE CStelliaNode_Dead::Tick(const _float& fTimeDelta)
{
	m_fTemp += fTimeDelta;

	//if (m_fTemp >= 4.f)
	//	Safe_Release(m_pStellia);

	BLACKBOARD& hashBlackBoard = m_pBT->GetBlackBoard();
	const auto& target = hashBlackBoard.find(TEXT("Test2"));

	// cout << target->second << endl;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_Dead* CStelliaNode_Dead::Create(CBehaviorTree* pBT)
{
	CStelliaNode_Dead* pInstance = new CStelliaNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_Dead::Free()
{
	__super::Free();
}
