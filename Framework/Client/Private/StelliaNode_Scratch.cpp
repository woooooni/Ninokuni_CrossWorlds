#include "stdafx.h"
#include "StelliaNode_Scratch.h"

CStelliaNode_Scratch::CStelliaNode_Scratch()
{
}

HRESULT CStelliaNode_Scratch::Initialize_Prototype(CBehaviorTree* pBT)
{
	m_pBT = pBT;

	return S_OK;
}

CBTNode::NODE_STATE CStelliaNode_Scratch::Tick(const _float& fTimeDelta)
{
	m_fTemp += fTimeDelta;

	if (m_fTemp >= 4.f)
	{
		m_fTemp = 4.f - m_fTemp;
		return NODE_STATE::NODE_SUCCESS;
	}

	// cout << "스크래치 진행 중" << endl;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_Scratch* CStelliaNode_Scratch::Create(CBehaviorTree* pBT)
{
	CStelliaNode_Scratch* pInstance = new CStelliaNode_Scratch();

	if (FAILED(pInstance->Initialize_Prototype(pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_Scratch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_Scratch::Free()
{
	__super::Free();
}
