#include "stdafx.h"
#include "Npc_RoamingNode.h"

#include "BehaviorTree.h"

#include "Model.h"

CNpc_RoamingNode::CNpc_RoamingNode()
{
}

HRESULT CNpc_RoamingNode::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_vecRoamingArea = *vecRoamingArea;

	return S_OK;
}

void CNpc_RoamingNode::Start()
{
	m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Walk"));
	m_bIsRoaming = true;
}

CBTNode::NODE_STATE CNpc_RoamingNode::Tick(const _float& fTimeDelta)
{
	return CBTNode::NODE_STATE();
}

CNpc_RoamingNode* CNpc_RoamingNode::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea)
{
	return nullptr;
}

void CNpc_RoamingNode::Free()
{
}
