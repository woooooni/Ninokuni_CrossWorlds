#include "stdafx.h"
#include "StelliaNode_Chase.h"

CStelliaNode_Chase::CStelliaNode_Chase()
{
}

HRESULT CStelliaNode_Chase::Initialize_Prototype(CBehaviorTree* pBT)
{
	m_pBT = pBT;

	return S_OK;
}

CBTNode::NODE_STATE CStelliaNode_Chase::Tick(const _float& fTimeDelta)
{
	m_fTemp += fTimeDelta;

	if (m_fTemp >= 4.f)
	{
		m_fTemp = 4.f - m_fTemp;
		return NODE_STATE::NODE_SUCCESS;
	}

	cout << "Ãß°Ý Áß" << endl;

	return NODE_STATE::NODE_RUNNING;
}

CStelliaNode_Chase* CStelliaNode_Chase::Create(CBehaviorTree* pBT)
{
	CStelliaNode_Chase* pInstance = new CStelliaNode_Chase();

	if (FAILED(pInstance->Initialize_Prototype(pBT)))
	{
		MSG_BOX("Fail Create : CStelliaNode_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaNode_Chase::Free()
{
	__super::Free();
}
