#include "stdafx.h"
#include "Criminal_MonsterBT.h"

#include "GameInstance.h"

#include "Criminal_Monster.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_RandomSelect.h"
#include "BTNode_Sequence.h"
#include "BTNode_Loop.h"

#include "Criminal_MonsterNode_Dead.h"

#include "Criminal_MonsterNode_Blow.h"
#include "Criminal_MonsterNode_Air.h"
#include "Criminal_MonsterNode_Stun.h"
#include "Criminal_MonsterNode_Hit.h"

#include "Criminal_MonsterNode_Attack1.h"
#include "Criminal_MonsterNode_Attack2.h"
#include "Criminal_MonsterNode_Skill1.h"

#include "Criminal_MonsterNode_Chase.h"

#include "Game_Manager.h"
#include "Player.h"


CCriminal_MonsterBT::CCriminal_MonsterBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonsterBT(pDevice, pContext)
{
}

CCriminal_MonsterBT::CCriminal_MonsterBT(const CCriminal_MonsterBT& rhs)
	: CMonsterBT(rhs)
{
}

HRESULT CCriminal_MonsterBT::Initialize_Prototype(CMonster* pOwner)
{
	__super::Initialize_Prototype(pOwner);

	m_pCriminal_Monster = dynamic_cast<CCriminal_Monster*>(m_tBTMonsterDesc.pOwner);
	m_pRootNode = CBTNode_Select::Create(this);

	/* 상위 Sequence 관련 */
	CBTNode_Sequence* pSeq_Dead = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Hit = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Combat = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Chase = CBTNode_Sequence::Create(this);

	/* Dead 관련 */
	CCriminal_MonsterNode_Dead* pDeadNode = CCriminal_MonsterNode_Dead::Create(&m_tBTMonsterDesc, this);

	/* Hit 관련 */
	CBTNode_Select* pSel_Hit = CBTNode_Select::Create(this);
	CCriminal_MonsterNode_Blow* pBlowNode = CCriminal_MonsterNode_Blow::Create(&m_tBTMonsterDesc, this);
	CCriminal_MonsterNode_Air* pAirNode = CCriminal_MonsterNode_Air::Create(&m_tBTMonsterDesc, this);
	CCriminal_MonsterNode_Stun* pStunNode = CCriminal_MonsterNode_Stun::Create(&m_tBTMonsterDesc, this);
	CCriminal_MonsterNode_Hit* pHitNode = CCriminal_MonsterNode_Hit::Create(&m_tBTMonsterDesc, this);

	/* Combat 관련 */
	CBTNode_Sequence* pSeq_Pattern = CBTNode_Sequence::Create(this);
	CCriminal_MonsterNode_Attack1* pAtk1Node = CCriminal_MonsterNode_Attack1::Create(&m_tBTMonsterDesc, this);
	CCriminal_MonsterNode_Attack2* pAtk2Node = CCriminal_MonsterNode_Attack2::Create(&m_tBTMonsterDesc, this);
	CCriminal_MonsterNode_Skill1* pSkill1Node = CCriminal_MonsterNode_Skill1::Create(&m_tBTMonsterDesc, this);

	/* Chase 관련 */
	CCriminal_MonsterNode_Chase* pChaseNode = CCriminal_MonsterNode_Chase::Create(&m_tBTMonsterDesc, this);

	/* Return 관련 */
	//CBaobam_WaterNode_Return* pReturnNode = CBaobam_WaterNode_Return::Create(&m_tBTMonsterDesc, this);

	/* Condition 관련*/
	/* function<_bool()>을 받는 CBTNode_Condition::Create 함수에서는 멤버 함수를 사용하고 있기 때문에 추가적인 처리가 필요 */
	CBTNode_Condition* pCon_IsDead = CBTNode_Condition::Create(bind(&CCriminal_MonsterBT::IsZeroHp, this), pDeadNode, pHitNode);
	CBTNode_Condition* pCon_IsHit = CBTNode_Condition::Create(bind(&CCriminal_MonsterBT::IsHit, this), pHitNode, pChaseNode);
	CBTNode_Condition* pCon_IsCombat = CBTNode_Condition::Create(bind(&CCriminal_MonsterBT::IsAtk, this), nullptr, pChaseNode);
	CBTNode_Condition* pCon_IsChase = CBTNode_Condition::Create(bind(&CCriminal_MonsterBT::IsChase, this), pChaseNode, nullptr);
	//CBTNode_Condition* pCon_IsReturn = CBTNode_Condition::Create(bind(&CCriminal_MonsterBT::IsReturn, this), pReturnNode, pIdleNode);


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
	pSeq_Pattern->Add_ChildNode(pSkill1Node);

	m_pRootNode->Add_ChildNode(pSeq_Chase);
	pSeq_Chase->Add_ChildNode(pCon_IsChase);
	pSeq_Chase->Add_ChildNode(pChaseNode);

	return S_OK;
}

HRESULT CCriminal_MonsterBT::Initialize(void* pArg)
{
	return S_OK;
}

void CCriminal_MonsterBT::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwner != nullptr)
		m_pRootNode->Tick(fTimeDelta);
}

void CCriminal_MonsterBT::LateTick(const _float& fTimeDelta)
{
}

void CCriminal_MonsterBT::Init_NodeStart()
{
	m_pRootNode->Init_Start();
}

_bool CCriminal_MonsterBT::IsZeroHp()
{
	if (m_pCriminal_Monster->Get_Stat().fHp <= 0)
		return true;

	return false;
}

_bool CCriminal_MonsterBT::IsHit()
{
	if (m_pCriminal_Monster->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT))
		return true;

	return false;
}

_bool CCriminal_MonsterBT::IsAtk()
{
	if (m_pCriminal_Monster->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
	{
		if (m_pCriminal_Monster->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND) ||
			m_pCriminal_Monster->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) ||
			m_pCriminal_Monster->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE))
		{
			return true;
		}
	}

	return false;
}

_bool CCriminal_MonsterBT::IsChase()
{
	if (m_pCriminal_Monster->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT) &&
		!m_pCriminal_Monster->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) &&
		!m_pCriminal_Monster->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE) &&
		!m_pCriminal_Monster->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT) &&
		!m_pCriminal_Monster->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
	{
		return true;
	}

	return false;
}

//_bool CCriminal_MonsterBT::IsReturn()
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


CCriminal_MonsterBT* CCriminal_MonsterBT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMonster* pOwner)
{
	CCriminal_MonsterBT* pInstance = new CCriminal_MonsterBT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pOwner)))
	{
		MSG_BOX("Fail Create : CCriminal_MonsterBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCriminal_MonsterBT::Clone(void* pArg)
{
	CCriminal_MonsterBT* pInstance = new CCriminal_MonsterBT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CCriminal_MonsterBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCriminal_MonsterBT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}
