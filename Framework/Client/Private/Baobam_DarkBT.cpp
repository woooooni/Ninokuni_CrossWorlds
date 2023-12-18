#include "stdafx.h"
#include "Baobam_DarkBT.h"

#include "GameInstance.h"

#include "Baobam_Dark.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_RandomSelect.h"
#include "BTNode_Sequence.h"
#include "BTNode_Loop.h"


#include "Baobam_DarkNode_Dead.h"

#include "Baobam_DarkNode_Stun.h"
#include "Baobam_DarkNode_Hit.h"

#include "Baobam_DarkNode_Attack1.h"
#include "Baobam_DarkNode_Attack2.h"
#include "Baobam_DarkNode_Skill1.h"
#include "Baobam_DarkNode_Skill2.h"

#include "Baobam_DarkNode_Chase.h"

#include "Baobam_DarkNode_Roaming.h"
#include "Baobam_DarkNode_Idle.h"

CBaobam_DarkBT::CBaobam_DarkBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBehaviorTree(pDevice, pContext)
{
}

CBaobam_DarkBT::CBaobam_DarkBT(const CBaobam_DarkBT& rhs)
	: CBehaviorTree(rhs)
{
}

HRESULT CBaobam_DarkBT::Initialize_Prototype(CGameObject* pObject)
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


	m_pBaobam_Dark = dynamic_cast<CBaobam_Dark*>(pObject);
	m_pRootNode = CBTNode_Select::Create(this);

	/* 상위 Sequence 관련 */
	CBTNode_Sequence* pSeq_Dead = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Hit = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Combat = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Chase = CBTNode_Sequence::Create(this);
	//CBTNode_Sequence* pSeq_Return = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Idle = CBTNode_Sequence::Create(this);

	/* Dead 관련 */
	CBaobam_DarkNode_Dead* pDeadNode = CBaobam_DarkNode_Dead::Create(&m_tBTNodeDesc, this);

	/* Hit 관련 */
	CBTNode_Select* pSel_Hit = CBTNode_Select::Create(this);
	CBaobam_DarkNode_Stun* pStunNode = CBaobam_DarkNode_Stun::Create(&m_tBTNodeDesc, this);
	CBaobam_DarkNode_Hit* pHitNode = CBaobam_DarkNode_Hit::Create(&m_tBTNodeDesc, this);

	/* Combat 관련 */
	CBTNode_Sequence* pSeq_Pattern = CBTNode_Sequence::Create(this);
	CBaobam_DarkNode_Attack1* pAtk1Node = CBaobam_DarkNode_Attack1::Create(&m_tBTNodeDesc, this);
	CBaobam_DarkNode_Attack2* pAtk2Node = CBaobam_DarkNode_Attack2::Create(&m_tBTNodeDesc, this);
	CBaobam_DarkNode_Skill1* pSkill1Node = CBaobam_DarkNode_Skill1::Create(&m_tBTNodeDesc, this);
	CBaobam_DarkNode_Skill2* pSkill2Node = CBaobam_DarkNode_Skill2::Create(&m_tBTNodeDesc, this);

	/* Chase 관련 */
	CBaobam_DarkNode_Chase* pChaseNode = CBaobam_DarkNode_Chase::Create(&m_tBTNodeDesc, this);

	/* Return 관련 */
	//CBaobam_DarkNode_Return* pReturnNode = CBaobam_DarkNode_Return::Create(&m_tBTNodeDesc, this);

	/* Idel 관련 */
	vector<wstring> vecAnimationName;
	vecAnimationName.push_back(TEXT("SKM_Baobam_Darkness.ao|BaoBam_Idle02"));
	vecAnimationName.push_back(TEXT("SKM_Baobam_Darkness.ao|BaoBam_Idle01"));
	vecAnimationName.push_back(TEXT("SKM_Baobam_Darkness.ao|BaoBam_Idle02"));
	CBaobam_DarkNode_Idle* pIdleNode = CBaobam_DarkNode_Idle::Create(&m_tBTNodeDesc, this, vecAnimationName);
	CBaobam_DarkNode_Roaming* pRoamingNode = CBaobam_DarkNode_Roaming::Create(&m_tBTNodeDesc, this, dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_RoamingArea());

	/* Condition 관련*/
	/* function<_bool()>을 받는 CBTNode_Condition::Create 함수에서는 멤버 함수를 사용하고 있기 때문에 추가적인 처리가 필요 */
	CBTNode_Condition* pCon_IsDead = CBTNode_Condition::Create(bind(&CBaobam_DarkBT::IsZeroHp, this), pDeadNode, pHitNode);
	CBTNode_Condition* pCon_IsHitAnim = CBTNode_Condition::Create(bind(&CBaobam_DarkBT::IsHitAnim, this), pHitNode, pChaseNode);
	CBTNode_Condition* pCon_IsCombat = CBTNode_Condition::Create(bind(&CBaobam_DarkBT::IsAtk, this), nullptr, pChaseNode);
	CBTNode_Condition* pCon_IsChase = CBTNode_Condition::Create(bind(&CBaobam_DarkBT::IsChase, this), pChaseNode, nullptr);
	//CBTNode_Condition* pCon_IsReturn = CBTNode_Condition::Create(bind(&CBaobam_DarkBT::IsReturn, this), pReturnNode, pIdleNode);


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
	pSeq_Pattern->Add_ChildNode(pSkill1Node);
	pSeq_Pattern->Add_ChildNode(pSkill2Node);

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

HRESULT CBaobam_DarkBT::Initialize(void* pArg)
{
	return S_OK;
}

void CBaobam_DarkBT::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pTarget != nullptr)
		m_pRootNode->Tick(fTimeDelta);
}

void CBaobam_DarkBT::LateTick(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::T))
	{
		m_pRootNode->Init_Start();
		m_pBaobam_Dark->Set_StunTime(3.f);
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Darkness.ao|BaoBam_Stun"));
		m_pBaobam_Dark->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN, true);
		m_pBaobam_Dark->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, true);
	}

	if (KEY_TAP(KEY::Y))
	{
		m_pBaobam_Dark->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT, true);
		m_pBaobam_Dark->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HITANIM, true);
		m_pBaobam_Dark->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, true);
	}

	if (KEY_TAP(KEY::U))
		m_pBaobam_Dark->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, false);
}

void CBaobam_DarkBT::Init_NodeStart()
{
	m_pRootNode->Init_Start();
}

_bool CBaobam_DarkBT::IsZeroHp()
{
	if (m_pBaobam_Dark->Get_Stat().fHp <= 0)
		return true;

	return false;
}

_bool CBaobam_DarkBT::IsHitAnim()
{
	if (m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HITANIM) ||
		m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
		return true;

	return false;
}

_bool CBaobam_DarkBT::IsAtk()
{
	if (m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
	{
		if (m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) ||
			m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE))
		{
			return true;
		}
	}

	return false;
}

_bool CBaobam_DarkBT::IsChase()
{
	if (m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT) &&
		!m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) &&
		!m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE) &&
		!m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HITANIM) &&
		!m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
	{
		return true;
	}

	return false;
}

//_bool CBaobam_DarkBT::IsReturn()
//{
//	if (!m_pBaobam_Dark->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
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


CBaobam_DarkBT* CBaobam_DarkBT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject)
{
	CBaobam_DarkBT* pInstance = new CBaobam_DarkBT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pObject)))
	{
		MSG_BOX("Fail Create : CBaobam_DarkBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CBaobam_DarkBT::Clone(void* pArg)
{
	CBaobam_DarkBT* pInstance = new CBaobam_DarkBT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CBaobam_DarkBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_DarkBT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}
