#include "stdafx.h"
#include "HumanChild01BT.h"

#include "GameInstance.h"

#include "HumanChild01.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_Sequence.h"

#include "Npc_RoamingNode.h"
#include "Npc_SpecialIdleNode.h"

#include "Game_Manager.h"
#include "Player.h"

CHumanChild01BT::CHumanChild01BT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBehaviorTree(pDevice, pContext)
{
}

CHumanChild01BT::CHumanChild01BT(const CHumanChild01BT& rhs)
	: CBehaviorTree(rhs)
{
}

HRESULT CHumanChild01BT::Initialize_Prototype(CGameObject* pObject)
{
	m_tBTNodeDesc.pOwner = pObject;
	m_tBTNodeDesc.pOwnerModel = pObject->Get_Component<CModel>(L"Com_Model");
	m_tBTNodeDesc.pOwnerTransform = pObject->Get_Component<CTransform>(L"Com_Transform");

	m_tBTNodeDesc.pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_tBTNodeDesc.pTarget != nullptr)
	{
		// m_tBTNodeDesc.pTarget = GI->Find_GameObjects(LEVEL_TEST, LAYER_CHARACTER);
		m_tBTNodeDesc.pTargetModel = m_tBTNodeDesc.pTarget->Get_Component<CModel>(L"Com_Model");
		m_tBTNodeDesc.pTargetTransform = m_tBTNodeDesc.pTarget->Get_Component<CTransform>(L"Com_Transform");
	}

	m_pNpc = dynamic_cast<CGameNpc*>(pObject);
	m_pRootNode = CBTNode_Select::Create(this);

	/* 상위 Sequence 관련 */
	CBTNode_Sequence* pSeq_Roaming = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_SpecialIdle = CBTNode_Sequence::Create(this);

	/* Roaming 관련 */
	CNpc_RoamingNode*	pRoamingNode = CNpc_RoamingNode::Create(&m_tBTNodeDesc, this, dynamic_cast<CGameNpc*>(m_tBTNodeDesc.pOwner)->Get_RoamingArea());

	///* Condition 관련*/
	//CBTNode_Condition* pCon_IsRoaming = CBTNode_Condition::Create(bind(&CHumanChild01BT::IsRoaming, this));

	//m_pRootNode->Add_ChildNode(pSeq_Roaming);
	//pSeq_Roaming->Add_ChildNode(pCon_IsRoaming);
	//pSeq_Roaming->Add_ChildNode(pRoamingNode);

	return S_OK;
}

HRESULT CHumanChild01BT::Initialize(void* pArg)
{
	return S_OK;
}

void CHumanChild01BT::Tick(const _float& fTimeDelta)
{
	m_pRootNode->Tick(fTimeDelta);
}

void CHumanChild01BT::LateTick(const _float& fTimeDelta)
{
}

void CHumanChild01BT::Init_NodeStart()
{
	m_pRootNode->Init_Start();
}

_bool CHumanChild01BT::IsOneWay()
{
	//if (m_pNpc->Get_Bools(CHumanChild01::NPC_BOOLTYPE::NPC_ROAMING))
	//	return true;
	//
	return false;
}

_bool CHumanChild01BT::IsTwoWay()
{
	return false;
}

CHumanChild01BT* CHumanChild01BT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject)
{
	CHumanChild01BT* pInstance = new CHumanChild01BT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pObject)))
	{
		MSG_BOX("Fail Create : CHumanChild01BT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CHumanChild01BT::Clone(void* pArg)
{
	CHumanChild01BT* pInstance = new CHumanChild01BT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanChild01BT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanChild01BT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}
