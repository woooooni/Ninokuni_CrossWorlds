#include "stdafx.h"
#include "IceBearManBT.h"

#include "GameInstance.h"

#include "IceBearMan.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_RandomSelect.h"
#include "BTNode_Sequence.h"
#include "BTNode_Loop.h"

#include "IceBearManNode_Dead.h"

#include "IceBearManNode_Stun.h"
#include "IceBearManNode_Hit.h"

#include "IceBearManNode_Chase.h"

#include "IceBearManNode_Attack1.h"
#include "IceBearManNode_Attack2.h"

#include "IceBearManNode_Roaming.h"
#include "IceBearManNode_Idle.h"


CIceBearManBT::CIceBearManBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBehaviorTree(pDevice, pContext)
{
}

CIceBearManBT::CIceBearManBT(const CIceBearManBT& rhs)
	: CBehaviorTree(rhs)
{
}

HRESULT CIceBearManBT::Initialize_Prototype(CGameObject* pObject)
{
	m_tBTNodeDesc.pOwner = pObject;
	m_tBTNodeDesc.pOwnerModel = pObject->Get_Component<CModel>(L"Com_Model");
	m_tBTNodeDesc.pOwnerTransform = pObject->Get_Component<CTransform>(L"Com_Transform");

	for (auto iter : GI->Find_GameObjects(LEVEL_TEST, LAYER_CHARACTER))
	{
		m_tBTNodeDesc.pTarget = iter;
	}

	if (m_tBTNodeDesc.pTarget != nullptr)
	{
		// m_tBTNodeDesc.pTarget = GI->Find_GameObjects(LEVEL_TEST, LAYER_CHARACTER);
		m_tBTNodeDesc.pTargetModel = m_tBTNodeDesc.pTarget->Get_Component<CModel>(L"Com_Model");
		m_tBTNodeDesc.pTargetTransform = m_tBTNodeDesc.pTarget->Get_Component<CTransform>(L"Com_Transform");
	}

	m_pIceBearMan = dynamic_cast<CIceBearMan*>(pObject);
	m_pRootNode = CBTNode_Select::Create(this);

	/* ���� Sequence ���� */
	CBTNode_Sequence* pSeq_Dead = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Hit = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Combat = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Chase = CBTNode_Sequence::Create(this);
	//CBTNode_Sequence* pSeq_Return = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Idle = CBTNode_Sequence::Create(this);

	/* Dead ���� */
	CIceBearManNode_Dead* pDeadNode = CIceBearManNode_Dead::Create(&m_tBTNodeDesc, this);

	/* Hit ���� */
	CBTNode_Select* pSel_Hit = CBTNode_Select::Create(this);
	CIceBearManNode_Stun* pStunNode = CIceBearManNode_Stun::Create(&m_tBTNodeDesc, this);
	CIceBearManNode_Hit* pHitNode = CIceBearManNode_Hit::Create(&m_tBTNodeDesc, this);

	/* Combat ���� */
	CBTNode_Sequence* pSeq_Pattern = CBTNode_Sequence::Create(this);
	CIceBearManNode_Attack1* pAtk1Node = CIceBearManNode_Attack1::Create(&m_tBTNodeDesc, this);
	CIceBearManNode_Attack2* pAtk2Node = CIceBearManNode_Attack2::Create(&m_tBTNodeDesc, this);

	/* Chase ���� */
	CIceBearManNode_Chase* pChaseNode = CIceBearManNode_Chase::Create(&m_tBTNodeDesc, this);

	/* Return ���� */
	//CIceBearManNode_Return* pReturnNode = CIceBearManNode_Return::Create(&m_tBTNodeDesc, this);

	/* Idel ���� */
	vector<wstring> vecAnimationName;
	vecAnimationName.push_back(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Idle02"));
	vecAnimationName.push_back(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Idle01"));
	vecAnimationName.push_back(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Idle02"));
	CIceBearManNode_Idle* pIdleNode = CIceBearManNode_Idle::Create(&m_tBTNodeDesc, this, vecAnimationName);
	CIceBearManNode_Roaming* pRoamingNode = CIceBearManNode_Roaming::Create(&m_tBTNodeDesc, this, dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_RoamingArea());

	/* Condition ����*/
	/* function<_bool()>�� �޴� CBTNode_Condition::Create �Լ������� ��� �Լ��� ����ϰ� �ֱ� ������ �߰����� ó���� �ʿ� */
	CBTNode_Condition* pCon_IsDead = CBTNode_Condition::Create(bind(&CIceBearManBT::IsZeroHp, this), pDeadNode, pAtk1Node);
	CBTNode_Condition* pCon_IsHitAnim = CBTNode_Condition::Create(bind(&CIceBearManBT::IsHitAnim, this), pHitNode, pChaseNode);
	CBTNode_Condition* pCon_IsCombat = CBTNode_Condition::Create(bind(&CIceBearManBT::IsAtkRound, this), nullptr, pChaseNode);
	CBTNode_Condition* pCon_IsChase = CBTNode_Condition::Create(bind(&CIceBearManBT::IsChase, this), pChaseNode, pRoamingNode);
	//CBTNode_Condition* pCon_IsReturn = CBTNode_Condition::Create(bind(&CIceBearManBT::IsReturn, this), pReturnNode, pIdleNode);


	m_pRootNode->Add_ChildNode(pSeq_Dead);
	pSeq_Dead->Add_ChildNode(pCon_IsDead);
	pSeq_Dead->Add_ChildNode(pDeadNode);

	m_pRootNode->Add_ChildNode(pSeq_Hit);
	pSeq_Hit->Add_ChildNode(pCon_IsHitAnim);
	pSeq_Hit->Add_ChildNode(pSel_Hit);
	pSel_Hit->Add_ChildNode(pStunNode);
	pSel_Hit->Add_ChildNode(pHitNode);

	m_pRootNode->Add_ChildNode(pSeq_Combat);
	pSeq_Combat->Add_ChildNode(pCon_IsCombat);
	pSeq_Combat->Add_ChildNode(pSeq_Pattern);
	pSeq_Pattern->Add_ChildNode(pAtk1Node);
	pSeq_Pattern->Add_ChildNode(pAtk2Node);

	m_pRootNode->Add_ChildNode(pSeq_Chase);
	pSeq_Chase->Add_ChildNode(pCon_IsChase);
	pSeq_Chase->Add_ChildNode(pChaseNode);

	//m_pRootNode->Add_ChildNode(pSeq_Return);
	//pSeq_Return->Add_ChildNode(pCon_IsReturn);
	//pSeq_Return->Add_ChildNode(pReturnNode);

	m_pRootNode->Add_ChildNode(pSeq_Idle);
	pSeq_Idle->Add_ChildNode(pRoamingNode);
	pSeq_Idle->Add_ChildNode(pIdleNode);

	return S_OK;
}

HRESULT CIceBearManBT::Initialize(void* pArg)
{
	return S_OK;
}

void CIceBearManBT::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pTarget != nullptr)
		m_pRootNode->Tick(fTimeDelta);
}

void CIceBearManBT::LateTick(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::NUM_1))
	{
		m_pRootNode->Init_Start();
		m_pIceBearMan->Set_StunTime(3.f);
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Stun"));
		m_pIceBearMan->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN, true);
		m_pIceBearMan->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, true);
	}

	if (KEY_TAP(KEY::NUM_2))
	{
		m_pIceBearMan->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT, true);
		m_pIceBearMan->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HITANIM, true);
		m_pIceBearMan->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, true);
	}

	if (KEY_TAP(KEY::NUM_3))
		m_pIceBearMan->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, false);
}

void CIceBearManBT::Init_NodeStart()
{
	m_pRootNode->Init_Start();
}

_bool CIceBearManBT::IsZeroHp()
{
	if (m_pIceBearMan->Get_Stat().fHp <= 0)
		return true;

	return false;
}

_bool CIceBearManBT::IsHitAnim()
{
	if (m_pIceBearMan->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HITANIM) ||
		m_pIceBearMan->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
		return true;

	return false;
}

_bool CIceBearManBT::IsAtkRound()
{
	if (m_pIceBearMan->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
	{
		if (m_pIceBearMan->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) ||
			m_pIceBearMan->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE))
		{
			return true;
		}
	}

	return false;
}

_bool CIceBearManBT::IsChase()
{
	if (m_pIceBearMan->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT) &&
		!m_pIceBearMan->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) &&
		!m_pIceBearMan->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE) &&
		!m_pIceBearMan->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HITANIM) &&
		!m_pIceBearMan->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
	{
		return true;
	}

	return false;
}

//_bool CIceBearManBT::IsReturn()
//{
//	if (!m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
//	{
//		_float4 vPos;
//		_float4 vOriginPos;
//
//		XMStoreFloat4(&vPos, m_tBTNodeDesc.pOwnerTransform->Get_Position());
//		XMStoreFloat4(&vOriginPos, dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_OriginPos());
//
//		if (vPos.x >= vOriginPos.x - 0.1f && vPos.x <= vOriginPos.x + 0.1f &&
//			vPos.z >= vOriginPos.z - 0.1f && vPos.z <= vOriginPos.z + 0.1f)
//		{
//			return false;
//		}
//
//		return true;
//	}
//
//	return false;
//}


CIceBearManBT* CIceBearManBT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject)
{
	CIceBearManBT* pInstance = new CIceBearManBT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pObject)))
	{
		MSG_BOX("Fail Create : CIceBearManBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CIceBearManBT::Clone(void* pArg)
{
	CIceBearManBT* pInstance = new CIceBearManBT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CIceBearManBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBearManBT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}