#include "stdafx.h"
#include "Shadow_ThiefBT.h"

#include "GameInstance.h"

#include "Shadow_Thief.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_RandomSelect.h"
#include "BTNode_Sequence.h"
#include "BTNode_Loop.h"

#include "Shadow_ThiefNode_Idle.h"

#include "Shadow_ThiefNode_Chase.h"

#include "Shadow_ThiefNode_Attack1.h"
#include "Shadow_ThiefNode_Attack2.h"
#include "Shadow_ThiefNode_Skill1.h"

#include "Shadow_ThiefNode_Return.h"

#include "Shadow_ThiefNode_Dead.h"

CShadow_ThiefBT::CShadow_ThiefBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBehaviorTree(pDevice, pContext)
{
}

CShadow_ThiefBT::CShadow_ThiefBT(const CShadow_ThiefBT& rhs)
	: CBehaviorTree(rhs)
{
}

HRESULT CShadow_ThiefBT::Initialize_Prototype(CGameObject* pObject)
{
	m_tBTNodeDesc.pOwner = pObject;
	m_tBTNodeDesc.pOwnerModel = pObject->Get_Component<CModel>(L"Com_Model");
	m_tBTNodeDesc.pOwnerTransform = pObject->Get_Component<CTransform>(L"Com_Transform");

	for (auto iter : GI->Find_GameObjects(LEVEL_TEST, LAYER_CHARACTER))
	{
		m_tBTNodeDesc.pTarget = iter;
	}

	// m_tBTNodeDesc.pTarget = GI->Find_GameObjects(LEVEL_TEST, LAYER_CHARACTER);
	m_tBTNodeDesc.pTargetModel = m_tBTNodeDesc.pTarget->Get_Component<CModel>(L"Com_Model");
	m_tBTNodeDesc.pTargetTransform = m_tBTNodeDesc.pTarget->Get_Component<CTransform>(L"Com_Transform");

	m_pShadow_Thief = dynamic_cast<CShadow_Thief*>(pObject);
	m_pRootNode = CBTNode_Select::Create(this);

	/* 상위 Sequence 관련 */
	CBTNode_Sequence* pSeq_Dead = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Combat = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Chase = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Return = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Idle = CBTNode_Sequence::Create(this);

	/* Dead 관련 */
	CShadow_ThiefNode_Dead* pDeadNode = CShadow_ThiefNode_Dead::Create(&m_tBTNodeDesc, this);

	/* Combat 관련 */
	CBTNode_Sequence* pSeq_Pattern = CBTNode_Sequence::Create(this);
	CShadow_ThiefNode_Attack1* pAtk1Node = CShadow_ThiefNode_Attack1::Create(&m_tBTNodeDesc, this);
	CShadow_ThiefNode_Attack2* pAtk2Node = CShadow_ThiefNode_Attack2::Create(&m_tBTNodeDesc, this);
	CShadow_ThiefNode_Skill1* pSkill1Node = CShadow_ThiefNode_Skill1::Create(&m_tBTNodeDesc, this);

	/* Chase 관련 */
	CShadow_ThiefNode_Chase* pChaseNode = CShadow_ThiefNode_Chase::Create(&m_tBTNodeDesc, this);

	/* Return 관련 */
	CShadow_ThiefNode_Return* pReturnNode = CShadow_ThiefNode_Return::Create(&m_tBTNodeDesc, this);

	/* Idel 관련 */
	CShadow_ThiefNode_Idle* pIdleNode = CShadow_ThiefNode_Idle::Create(&m_tBTNodeDesc, this);

	/* Condition 관련*/
	/* function<_bool()>을 받는 CBTNode_Condition::Create 함수에서는 멤버 함수를 사용하고 있기 때문에 추가적인 처리가 필요 */
	CBTNode_Condition* pCon_IsDead = CBTNode_Condition::Create(bind(&CShadow_ThiefBT::IsZeroHp, this), pDeadNode, pAtk1Node);
	CBTNode_Condition* pCon_IsCombat = CBTNode_Condition::Create(bind(&CShadow_ThiefBT::IsAtkRound, this), nullptr, pChaseNode);
	CBTNode_Condition* pCon_IsChase = CBTNode_Condition::Create(bind(&CShadow_ThiefBT::IsChase, this), pChaseNode, pReturnNode);
	CBTNode_Condition* pCon_IsReturn = CBTNode_Condition::Create(bind(&CShadow_ThiefBT::IsReturn, this), pReturnNode, pIdleNode);


	m_pRootNode->Add_ChildNode(pSeq_Dead);
	pSeq_Dead->Add_ChildNode(pCon_IsDead);
	pSeq_Dead->Add_ChildNode(pDeadNode);

	m_pRootNode->Add_ChildNode(pSeq_Combat);
	pSeq_Combat->Add_ChildNode(pCon_IsCombat);
	pSeq_Combat->Add_ChildNode(pSeq_Pattern);
	pSeq_Pattern->Add_ChildNode(pAtk1Node);
	pSeq_Pattern->Add_ChildNode(pAtk2Node);
	pSeq_Pattern->Add_ChildNode(pSkill1Node);

	m_pRootNode->Add_ChildNode(pSeq_Chase);
	pSeq_Chase->Add_ChildNode(pCon_IsChase);
	pSeq_Chase->Add_ChildNode(pChaseNode);

	m_pRootNode->Add_ChildNode(pSeq_Return);
	pSeq_Return->Add_ChildNode(pCon_IsReturn);
	pSeq_Return->Add_ChildNode(pReturnNode);

	m_pRootNode->Add_ChildNode(pSeq_Idle);
	pSeq_Idle->Add_ChildNode(pIdleNode);

	return S_OK;
}

HRESULT CShadow_ThiefBT::Initialize(void* pArg)
{
	return S_OK;
}

void CShadow_ThiefBT::Tick(const _float& fTimeDelta)
{
	m_pRootNode->Tick(fTimeDelta);
}

void CShadow_ThiefBT::LateTick(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::Y))
		m_pShadow_Thief->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, true);
	else if (KEY_TAP(KEY::U))
		m_pShadow_Thief->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, false);

}

void CShadow_ThiefBT::Init_NodeStart()
{
	m_pRootNode->Init_Start();
}

_bool CShadow_ThiefBT::IsZeroHp()
{
	if (m_pShadow_Thief->Get_Stat().fHp <= 0)
		return true;

	return false;
}

_bool CShadow_ThiefBT::IsAtkRound()
{
	if (m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT) &&
		m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK))
	{
		return true;
	}

	return false;
}

_bool CShadow_ThiefBT::IsChase()
{
	if (m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT) &&
		!m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK))
	{
		return true;
	}

	return false;
}

_bool CShadow_ThiefBT::IsReturn()
{
	if (!m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
	{
		_float4 vPos;
		_float4 vOriginPos;

		XMStoreFloat4(&vPos, m_tBTNodeDesc.pOwnerTransform->Get_Position());
		XMStoreFloat4(&vOriginPos, dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_OriginPos());

		if (vPos.x >= vOriginPos.x - 0.1f && vPos.x <= vOriginPos.x + 0.1f &&
			vPos.z >= vOriginPos.z - 0.1f && vPos.z <= vOriginPos.z + 0.1f)
		{
			return false;
		}

		return true;
	}

	return false;
}


CShadow_ThiefBT* CShadow_ThiefBT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject)
{
	CShadow_ThiefBT* pInstance = new CShadow_ThiefBT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pObject)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShadow_ThiefBT::Clone(void* pArg)
{
	CShadow_ThiefBT* pInstance = new CShadow_ThiefBT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CShadow_ThiefBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefBT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}
