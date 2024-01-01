#include "stdafx.h"
#include "Clown_WizardBT.h"

#include "GameInstance.h"

#include "Clown_Wizard.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_RandomSelect.h"
#include "BTNode_Sequence.h"
#include "BTNode_Loop.h"

#include "Clown_WizardNode_Dead.h"

#include "Clown_WizardNode_Stun.h"
#include "Clown_WizardNode_Hit.h"

#include "Clown_WizardNode_Attack1.h"
#include "Clown_WizardNode_Attack2.h"

#include "Clown_WizardNode_Chase.h"

#include "Clown_WizardNode_Roaming.h"
#include "Clown_WizardNode_Idle.h"

#include "Game_Manager.h"
#include "Player.h"

CClown_WizardBT::CClown_WizardBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBehaviorTree(pDevice, pContext)
{
}

CClown_WizardBT::CClown_WizardBT(const CClown_WizardBT& rhs)
	: CBehaviorTree(rhs)
{
}

HRESULT CClown_WizardBT::Initialize_Prototype(CGameObject* pObject)
{
	m_tBTNodeDesc.pOwner = pObject;
	m_tBTNodeDesc.pOwnerModel = pObject->Get_Component<CModel>(L"Com_Model");
	m_tBTNodeDesc.pOwnerTransform = pObject->Get_Component<CTransform>(L"Com_Transform");

	m_tBTNodeDesc.pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_tBTNodeDesc.pTarget != nullptr)
	{
		m_tBTNodeDesc.pTargetModel = m_tBTNodeDesc.pTarget->Get_Component<CModel>(L"Com_Model");
		m_tBTNodeDesc.pTargetTransform = m_tBTNodeDesc.pTarget->Get_Component<CTransform>(L"Com_Transform");
	}

	m_pClown_Wizard = dynamic_cast<CClown_Wizard*>(pObject);
	m_pRootNode = CBTNode_Select::Create(this);

	/* 상위 Sequence 관련 */
	CBTNode_Sequence* pSeq_Dead = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Hit = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Combat = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Chase = CBTNode_Sequence::Create(this);
	//CBTNode_Sequence* pSeq_Return = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Idle = CBTNode_Sequence::Create(this);

	/* Dead 관련 */
	CClown_WizardNode_Dead* pDeadNode = CClown_WizardNode_Dead::Create(&m_tBTNodeDesc, this);

	/* Hit 관련 */
	CBTNode_Select* pSel_Hit = CBTNode_Select::Create(this);
	CClown_WizardNode_Stun* pStunNode = CClown_WizardNode_Stun::Create(&m_tBTNodeDesc, this);
	CClown_WizardNode_Hit* pHitNode = CClown_WizardNode_Hit::Create(&m_tBTNodeDesc, this);

	/* Combat 관련 */
	CBTNode_Sequence* pSeq_Pattern = CBTNode_Sequence::Create(this);
	CClown_WizardNode_Attack1* pAtk1Node = CClown_WizardNode_Attack1::Create(&m_tBTNodeDesc, this);
	CClown_WizardNode_Attack2* pAtk2Node = CClown_WizardNode_Attack2::Create(&m_tBTNodeDesc, this);

	/* Chase 관련 */
	CClown_WizardNode_Chase* pChaseNode = CClown_WizardNode_Chase::Create(&m_tBTNodeDesc, this);

	/* Return 관련 */
	//CBaobam_WaterNode_Return* pReturnNode = CBaobam_WaterNode_Return::Create(&m_tBTNodeDesc, this);

	/* Idel 관련 */
	vector<wstring> vecAnimationName;
	vecAnimationName.push_back(TEXT("SKM_ClownWizard.ao|ClownWizard_Idle01"));
	vecAnimationName.push_back(TEXT("SKM_ClownWizard.ao|ClownWizard_Idle02"));
	vecAnimationName.push_back(TEXT("SKM_ClownWizard.ao|ClownWizard_Idle01"));
	CClown_WizardNode_Idle* pIdleNode = CClown_WizardNode_Idle::Create(&m_tBTNodeDesc, this, vecAnimationName);
	CClown_WizardNode_Roaming* pRoamingNode = CClown_WizardNode_Roaming::Create(&m_tBTNodeDesc, this, dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_RoamingArea());

	/* Condition 관련*/
	/* function<_bool()>을 받는 CBTNode_Condition::Create 함수에서는 멤버 함수를 사용하고 있기 때문에 추가적인 처리가 필요 */
	CBTNode_Condition* pCon_IsDead = CBTNode_Condition::Create(bind(&CClown_WizardBT::IsZeroHp, this), pDeadNode, pHitNode);
	CBTNode_Condition* pCon_IsWeak = CBTNode_Condition::Create(bind(&CClown_WizardBT::IsWeak, this), pHitNode, pChaseNode);
	CBTNode_Condition* pCon_IsCombat = CBTNode_Condition::Create(bind(&CClown_WizardBT::IsAtk, this), nullptr, pChaseNode);
	CBTNode_Condition* pCon_IsChase = CBTNode_Condition::Create(bind(&CClown_WizardBT::IsChase, this), pChaseNode, nullptr);
	//CBTNode_Condition* pCon_IsReturn = CBTNode_Condition::Create(bind(&CClown_WizardBT::IsReturn, this), pReturnNode, pIdleNode);


	m_pRootNode->Add_ChildNode(pSeq_Dead);
	pSeq_Dead->Add_ChildNode(pCon_IsDead);
	pSeq_Dead->Add_ChildNode(pDeadNode);

	m_pRootNode->Add_ChildNode(pSeq_Hit);
	pSeq_Hit->Add_ChildNode(pCon_IsWeak);
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

HRESULT CClown_WizardBT::Initialize(void* pArg)
{
	return S_OK;
}

void CClown_WizardBT::Tick(const _float& fTimeDelta)
{
	if (m_tBTNodeDesc.pTarget != nullptr)
		m_pRootNode->Tick(fTimeDelta);
}

void CClown_WizardBT::LateTick(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::F4))
	{
		m_pRootNode->Init_Start();
		m_pClown_Wizard->Set_StunTime(3.f);
		m_tBTNodeDesc.pOwnerModel->Set_Animation(TEXT("SKM_ClownWizard.ao|ClownWizard_Stun"));
		m_pClown_Wizard->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN, true);
		m_pClown_Wizard->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, true);
	}
}

void CClown_WizardBT::Init_NodeStart()
{
	m_pRootNode->Init_Start();
}

_bool CClown_WizardBT::IsZeroHp()
{
	if (m_pClown_Wizard->Get_Stat().fHp <= 0)
		return true;

	return false;
}

_bool CClown_WizardBT::IsWeak()
{
	if (m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT) ||
		m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
		return true;

	return false;
}

_bool CClown_WizardBT::IsAtk()
{
	if (m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT) &&
		m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND) ||
		m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) ||
		m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE))
	{
		return true;
	}

	return false;
}

_bool CClown_WizardBT::IsChase()
{
	if (m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT) &&
		!m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) &&
		!m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE) &&
		!m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT) &&
		!m_pClown_Wizard->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
	{
		return true;
	}

	return false;
}

//_bool CClown_WizardBT::IsReturn()
//{
//	if (!m_pBaobam_Water->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
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


CClown_WizardBT* CClown_WizardBT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject)
{
	CClown_WizardBT* pInstance = new CClown_WizardBT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pObject)))
	{
		MSG_BOX("Fail Create : CClown_WizardBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CClown_WizardBT::Clone(void* pArg)
{
	CClown_WizardBT* pInstance = new CClown_WizardBT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CClown_WizardBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClown_WizardBT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}
