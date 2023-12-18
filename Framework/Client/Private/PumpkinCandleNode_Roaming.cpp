#include "stdafx.h"
#include "PumpkinCandleNode_Roaming.h"

#include "BehaviorTree.h"


#include "Model.h"

CPumpkinCandleNode_Roaming::CPumpkinCandleNode_Roaming()
{
}

HRESULT CPumpkinCandleNode_Roaming::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_vecRoamingArea = *vecRoamingArea;

	return S_OK;
}

void CPumpkinCandleNode_Roaming::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_BattleWalk"));
	m_bIsRoaming = true;
}

CBTNode::NODE_STATE CPumpkinCandleNode_Roaming::Tick(const _float& fTimeDelta)
{
	_float4 vPos;
	_float4 vDestPos;

	XMStoreFloat4(&vPos, m_tBTNodeDesc.pOwnerTransform->Get_Position());
	XMStoreFloat4(&vDestPos, m_vecRoamingArea[m_iRoamingIndex]);

	if (m_bIsRoaming)
	{
		m_tBTNodeDesc.pOwnerTransform->LookAt_ForLandObject(m_vecRoamingArea[m_iRoamingIndex]);
		m_tBTNodeDesc.pOwnerTransform->Move(m_tBTNodeDesc.pOwnerTransform->Get_Look(), 1.f, fTimeDelta);

		if (vPos.x >= vDestPos.x - 0.1f && vPos.x <= vDestPos.x + 0.1f &&
			vPos.z >= vDestPos.z - 0.1f && vPos.z <= vDestPos.z + 0.1f)
		{
			m_iRoamingIndex += 1;
			if (m_iRoamingIndex == m_vecRoamingArea.size())
				m_iRoamingIndex = 0;

			m_bIsRoaming = false;
		}
	}
	else
		return NODE_STATE::NODE_SUCCESS;

	return NODE_STATE::NODE_RUNNING;
}

CPumpkinCandleNode_Roaming* CPumpkinCandleNode_Roaming::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea)
{
	CPumpkinCandleNode_Roaming* pInstance = new CPumpkinCandleNode_Roaming();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT, vecRoamingArea)))
	{
		MSG_BOX("Fail Create : CPumpkinCandleNode_Roaming");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPumpkinCandleNode_Roaming::Free()
{
	__super::Free();
}
