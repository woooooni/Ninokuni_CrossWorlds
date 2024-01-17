#include "stdafx.h"
#include "ClownBT.h"

#include "GameInstance.h"

#include "Clown.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_RandomSelect.h"
#include "BTNode_Sequence.h"
#include "BTNode_Loop.h"

#include "ClownNode_Dead.h"

#include "ClownNode_Blow.h"
#include "ClownNode_Air.h"
#include "ClownNode_Stun.h"
#include "ClownNode_Hit.h"

#include "ClownNode_Attack1.h"
#include "ClownNode_Attack2.h"
#include "ClownNode_Skill.h"

#include "ClownNode_Chase.h"

#include "ClownNode_Roaming.h"
#include "ClownNode_Idle.h"

#include "Game_Manager.h"
#include "Player.h"

CClownBT::CClownBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonsterBT(pDevice, pContext)
{
}

CClownBT::CClownBT(const CClownBT& rhs)
	: CMonsterBT(rhs)
{
}

HRESULT CClownBT::Initialize_Prototype(CMonster* pOwner)
{
	__super::Initialize_Prototype(pOwner);

	m_pClown = dynamic_cast<CClown*>(m_tBTMonsterDesc.pOwner);
	m_pRootNode = CBTNode_Select::Create(this);

	/* 상위 Sequence 관련 */
	CBTNode_Sequence* pSeq_Dead = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Hit = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Combat = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Chase = CBTNode_Sequence::Create(this);
	//CBTNode_Sequence* pSeq_Return = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Idle = CBTNode_Sequence::Create(this);

	/* Dead 관련 */
	CClownNode_Dead* pDeadNode = CClownNode_Dead::Create(&m_tBTMonsterDesc, this);

	/* Hit 관련 */
	CBTNode_Select* pSel_Hit = CBTNode_Select::Create(this);
	CClownNode_Blow* pBlowNode = CClownNode_Blow::Create(&m_tBTMonsterDesc, this);
	CClownNode_Air* pAirNode = CClownNode_Air::Create(&m_tBTMonsterDesc, this);
	CClownNode_Stun* pStunNode = CClownNode_Stun::Create(&m_tBTMonsterDesc, this);
	CClownNode_Hit* pHitNode = CClownNode_Hit::Create(&m_tBTMonsterDesc, this);

	/* Combat 관련 */
	CBTNode_Sequence* pSeq_Pattern = CBTNode_Sequence::Create(this);
	CClownNode_Attack1* pAtk1Node = CClownNode_Attack1::Create(&m_tBTMonsterDesc, this);
	CClownNode_Attack2* pAtk2Node = CClownNode_Attack2::Create(&m_tBTMonsterDesc, this);
	CClownNode_Skill* pSkillNode = CClownNode_Skill::Create(&m_tBTMonsterDesc, this);

	/* Chase 관련 */
	CClownNode_Chase* pChaseNode = CClownNode_Chase::Create(&m_tBTMonsterDesc, this);

	/* Return 관련 */
	//CBaobam_WaterNode_Return* pReturnNode = CBaobam_WaterNode_Return::Create(&m_tBTMonsterDesc, this);

	/* Idel 관련 */
	vector<wstring> vecAnimationName;
	vecAnimationName.push_back(TEXT("SKM_Clown.ao|Clown_Idle01"));
	vecAnimationName.push_back(TEXT("SKM_Clown.ao|Clown_Idle02"));
	vecAnimationName.push_back(TEXT("SKM_Clown.ao|Clown_Idle01"));
	CClownNode_Idle* pIdleNode = CClownNode_Idle::Create(&m_tBTMonsterDesc, this, vecAnimationName);
	CClownNode_Roaming* pRoamingNode = CClownNode_Roaming::Create(&m_tBTMonsterDesc, this, dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_RoamingArea());

	/* Condition 관련*/
	/* function<_bool()>을 받는 CBTNode_Condition::Create 함수에서는 멤버 함수를 사용하고 있기 때문에 추가적인 처리가 필요 */
	CBTNode_Condition* pCon_IsDead = CBTNode_Condition::Create(bind(&CClownBT::IsZeroHp, this), pDeadNode, pHitNode);
	CBTNode_Condition* pCon_IsHit = CBTNode_Condition::Create(bind(&CClownBT::IsHit, this), pHitNode, pChaseNode);
	CBTNode_Condition* pCon_IsCombat = CBTNode_Condition::Create(bind(&CClownBT::IsAtk, this), nullptr, pChaseNode);
	CBTNode_Condition* pCon_IsChase = CBTNode_Condition::Create(bind(&CClownBT::IsChase, this), pChaseNode, nullptr);
	//CBTNode_Condition* pCon_IsReturn = CBTNode_Condition::Create(bind(&CClownBT::IsReturn, this), pReturnNode, pIdleNode);


	m_pRootNode->Add_ChildNode(pSeq_Dead);
	pSeq_Dead->Add_ChildNode(pCon_IsDead);
	pSeq_Dead->Add_ChildNode(pDeadNode);

	m_pRootNode->Add_ChildNode(pSeq_Hit);
	pSeq_Hit->Add_ChildNode(pCon_IsHit);
	pSeq_Hit->Add_ChildNode(pSel_Hit);
	pSel_Hit->Add_ChildNode(pBlowNode);
	pSel_Hit->Add_ChildNode(pAirNode);
	pSel_Hit->Add_ChildNode(pStunNode);
	pSel_Hit->Add_ChildNode(pHitNode);

	m_pRootNode->Add_ChildNode(pSeq_Combat);
	pSeq_Combat->Add_ChildNode(pCon_IsCombat);
	pSeq_Combat->Add_ChildNode(pSeq_Pattern);
	pSeq_Pattern->Add_ChildNode(pAtk1Node);
	pSeq_Pattern->Add_ChildNode(pAtk2Node);
	pSeq_Pattern->Add_ChildNode(pSkillNode);

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

HRESULT CClownBT::Initialize(void* pArg)
{
	return S_OK;
}

void CClownBT::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwner != nullptr)
		m_pRootNode->Tick(fTimeDelta);
}

void CClownBT::LateTick(const _float& fTimeDelta)
{
}

void CClownBT::Init_NodeStart()
{
	m_pRootNode->Init_Start();
}

_bool CClownBT::IsZeroHp()
{
	if (m_pClown->Get_Stat().fHp <= 0)
		return true;

	return false;
}

_bool CClownBT::IsHit()
{
	if (m_pClown->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT))
		return true;

	return false;
}

_bool CClownBT::IsAtk()
{
	if (m_pClown->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
	{
		if (m_pClown->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND) ||
			m_pClown->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) ||
			m_pClown->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE))
		{
			return true;
		}
	}

	return false;
}

_bool CClownBT::IsChase()
{
	if (m_pClown->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT) &&
		!m_pClown->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) &&
		!m_pClown->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE) &&
		!m_pClown->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT) &&
		!m_pClown->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
	{
		return true;
	}

	return false;
}

//_bool CClownBT::IsReturn()
//{
//	if (!m_pBaobam_Water->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
//	{
//		_float4 vPos;
//		_float4 vOriginPos;
//
//		XMStoreFloat4(&vPos, m_tBTMonsterDesc.pOwnerTransform->Get_Position());
//		XMStoreFloat4(&vOriginPos, dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_OriginPos());
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


CClownBT* CClownBT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMonster* pOwner)
{
	CClownBT* pInstance = new CClownBT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pOwner)))
	{
		MSG_BOX("Fail Create : CClownBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CClownBT::Clone(void* pArg)
{
	CClownBT* pInstance = new CClownBT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CClownBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClownBT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}
