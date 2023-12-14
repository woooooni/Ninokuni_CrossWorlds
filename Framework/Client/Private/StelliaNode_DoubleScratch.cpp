#include "stdafx.h"
#include "StelliaNode_DoubleScratch.h"

CStelliaNode_DoubleScratch::CStelliaNode_DoubleScratch()
{
}

HRESULT CStelliaNode_DoubleScratch::Initialize_Prototype(CBehaviorTree* pBT)
{
	m_pBT = pBT;

	return S_OK;
}

CBTNode::NODE_STATE CStelliaNode_DoubleScratch::Tick(const _float& fTimeDelta)
{
	m_fTemp += fTimeDelta;

	if (m_fTemp >= 4.f)
	{
		m_fTemp = 4.f - m_fTemp;
		return NODE_STATE::NODE_SUCCESS;
	}

	// cout << "더블 스크래치 진행 중" << endl;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_DoubleScratch* CStelliaNode_DoubleScratch::Create(CBehaviorTree* pBT)
{
	CStelliaNode_DoubleScratch* pInstance = new CStelliaNode_DoubleScratch();

	if (FAILED(pInstance->Initialize_Prototype(pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_DoubleScratch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_DoubleScratch::Free()
{
	__super::Free();
}
