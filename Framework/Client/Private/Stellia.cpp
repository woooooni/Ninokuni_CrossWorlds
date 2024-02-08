#include "stdafx.h"
#include "Stellia.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "StelliaState_Spawn.h"
#include "StelliaState_SpawnIdle.h"
#include "StelliaState_SpawnStand.h"

#include "StelliaState_CombatIdle.h"
#include "StelliaState_Chase.h"

#include "StelliaState_Attack1.h"
#include "StelliaState_AfterSpinTail.h"
#include "StelliaState_Attack2.h"
#include "StelliaState_Laser.h"
#include "StelliaState_TripleLaser.h"
#include "StelliaState_JumpStamp.h"
#include "StelliaState_Charge.h"
#include "StelliaState_SpinTail.h"
#include "StelliaState_BigBang.h"
#include "StelliaState_ChaseJumpStamp.h"

#include "StelliaState_CounterStart.h"
#include "StelliaState_CounterLoop.h"
#include "StelliaState_CounterEnd.h"

#include "StelliaState_Berserk.h"

// Rage 1
#include "StelliaState_Rage1StartTurnOC.h"
#include "StelliaState_Rage1StartJump.h"
#include "StelliaState_Rage1StartTurnOL.h"
#include "StelliaState_Rage1Start.h"

#include "StelliaState_Rage1Loop_Turn.h"
#include "StelliaState_Rage1Loop_Idle.h"
#include "StelliaState_Rage1Loop_Explosion.h"
#include "StelliaState_Rage1Loop_SpinTail.h"
#include "StelliaState_Rage1Loop_JumpStamp.h"

// Rage 2
#include "StelliaState_Rage2StartTurnOC.h"
#include "StelliaState_Rage2StartJump.h"
#include "StelliaState_Rage2StartTurnOL.h"
#include "StelliaState_Rage2Start.h"

#include "StelliaState_Rage2Loop.h"
#include "StelliaState_Rage2Finish.h"

// Rage3
#include "StelliaState_Rage3Start_FadeOut.h"
#include "StelliaState_Rage3Start_FadeIn.h"

#include "StelliaState_Rage3Around.h"
#include "StelliaState_Rage3AroundBreak.h"
#include "StelliaState_Rage3TurnAround.h"
#include "StelliaState_Rage3TurnPL.h"

#include "StelliaState_Rage3ClawReady.h"
#include "StelliaState_Rage3ClawCharge.h"
#include "StelliaState_Rage3Claw.h"
#include "StelliaState_Rage3ClawEndCharge.h"
#include "StelliaState_Rage3ClawEndBreak.h"

#include "StelliaState_Rage3ChargeReady.h"
#include "StelliaState_Rage3Charge.h"
#include "StelliaState_Rage3ChargeBreak.h"

#include "StelliaState_Rage3Damage.h"
#include "StelliaState_Rage3Escape.h"

#include "StelliaState_Rage3TurnOC.h"

// 
#include "StelliaState_BackStep.h"
#include "StelliaState_Turn.h"
#include "StelliaState_Dead.h"

#include "Camera_Manager.h"
#include "Quest_Manager.h"

#include "Stellia_Crystal_Controller.h"

#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CStellia::CStellia(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CBoss(pDevice, pContext, strObjectTag, tStat)
{
}

CStellia::CStellia(const CStellia& rhs)
	: CBoss(rhs)
{

}

HRESULT CStellia::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CStellia::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	// m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));

	//if (FAILED(Ready_Sockets()))
	//	return E_FAIL;

	//if (FAILED(Ready_Parts()))
	//	return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_iObjectType = OBJ_TYPE::OBJ_BOSS;

	m_vBloomPower = _float3(2.f, 2.f, 2.f);

	// 레이지 1에서 채워야 하는 데미지
	m_iDestDamage = 20000;

	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
	{
		if (FAILED(CUI_Manager::GetInstance()->Ready_BossHPBar(this)))
			return E_FAIL;
	}

//	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Stellia_Timer"))))
//		return E_FAIL;

	return S_OK;
}

void CStellia::Tick(_float fTimeDelta)
{
	m_tTargetDesc.pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
	m_tTargetDesc.pTragetTransform = m_tTargetDesc.pTarget->Get_Component_Transform();

	/* 최초 등장 대기 시간 */
	//if (!m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_INTRO])
	//{
	//	m_fIntroTime += fTimeDelta;
	//	if (m_fIntroTime > 3.f)
	//	{
	//		m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_INTRO] = true;
	//		m_pStateCom->Change_State(GLANIX_INTRO_ROAR);
	//	}
	//}

	//if (KEY_TAP(KEY::L))
	//{
	//	m_pStateCom->Change_State(m_iCurAnimIndex);
	//
	//	m_iCurAnimIndex += 1;
	//	if (m_iCurAnimIndex >= m_pStateCom->Get_States().size())
	//		m_iCurAnimIndex = 0;
	//}

	/* 임시 */
	if (KEY_TAP(KEY::C))
	{
		m_pStateCom->Change_State(STELLIA_RAGE3TURN_AROUND);
	}
	if (KEY_TAP(KEY::X))
	{
		m_tStat.fHp -= m_tStat.fMaxHp * 0.1f;
	}

	//#ifdef _DEBUG
	//	if (LEVELID::LEVEL_TOOL == GI->Get_CurrentLevel() && KEY_TAP(KEY::L))
	//		m_pStateCom->Change_State(GLANIX_SPINBOMBBOMB);
	//#endif // DEBUG

	m_pStateCom->Tick_State(fTimeDelta);

	__super::Tick(fTimeDelta);

	if(m_bDead == true)
		CQuest_Manager::GetInstance()->Set_IsBossKill(true);

	if (nullptr != m_pCrystalController)
		m_pCrystalController->Tick(fTimeDelta);
}

void CStellia::LateTick(_float fTimeDelta)
{
#ifdef _DEBUG
	m_pRendererCom->Add_Debug(m_pStateCom);
#endif

	__super::LateTick(fTimeDelta);
}

HRESULT CStellia::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CStellia::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		/* 레이지3 가드이벤트 */
		if (dynamic_cast<CCharacter*>(tInfo.pOther)->Get_CharacterStateCom()->Get_CurrState() == CCharacter::BATTLE_GUARD &&
			m_pStateCom->Get_CurrState() == STELLIA_RAGE3CHARGE)
		{
			m_bIsPlayerGuardEvent = true;
		}
	
		/* 평소 */
		else
		{
			m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_ATKAROUND] = true;
		}
	}

	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
	{
		m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_SKILLAROUND] = true;
	}

	/* 피격 */
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER_PROJECTILE) &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
		{
			On_Damaged(tInfo);
		}
	}

	/* Counter */
	if (m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_COUNTER])
	{
		if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
			tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK &&
			tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::STUN)
		{
			if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
			{
				On_Damaged(tInfo);
				CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
				m_pStateCom->Change_State(STELLIA_COUNTERSTART);
				m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_COUNTER] = false;
			}
		}
	}
}

void CStellia::Collision_Continue(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		/* 레이지3 가드이벤트 */
		if (dynamic_cast<CCharacter*>(tInfo.pOther)->Get_CharacterStateCom()->Get_CurrState() == CCharacter::BATTLE_GUARD &&
			m_pStateCom->Get_CurrState() == STELLIA_RAGE3CHARGE)
		{
			m_bIsPlayerGuardEvent = true;
		}
		/* 평소 */
		else
		{
			m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_ATKAROUND] = true;
		}
	}

	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
	{
		m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_SKILLAROUND] = true;
	}

}

void CStellia::Collision_Exit(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_ATKAROUND] = false;
	}

	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
	{
		m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_SKILLAROUND] = false;
	}
}

void CStellia::On_Damaged(const COLLISION_INFO& tInfo)
{
	// 맞기 전 체력 
	_int iCurHp = m_tStat.fHp;

	// 데미지를 입힐 수 있는 상태 일 때에만
	if (m_bIsStelliaHit)
		__super::On_Damaged(tInfo);

	// 레이지 1 상태일 때 누적 데미지 증가하기.
	if (m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_RAGE])
		m_iAccDamage += iCurHp - m_tStat.fHp;
}

void CStellia::Set_SkillTree()
{
	map<_uint, CState*> pStates = m_pStateCom->Get_States();

	for (auto& iter : pStates)
	{
		dynamic_cast<CStelliaState_Base*>(iter.second)->Init_Pattern();
	}
}

HRESULT CStellia::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 10.f, 10.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-55.f, 1.6, 363.f, 1.f));

	//if (CGameInstance::GetInstance()->Get_CurrentLevel() == LEVELID::LEVEL_TOOL)
	//{
	//	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.f, 0.f, 1.f));
	//	m_pTransformCom->FixRotation(0.f, 180.f, 0.f);
	//
	//	m_vOriginPos = m_pTransformCom->Get_Position();
	//	m_vOriginLook = m_pTransformCom->Get_Look();
	//	m_vRage3StartPos = Vec4(0.f, 0.f, 40.f, 1.f);
	//}
	//else
	//{
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(138.f, -0.5f, 102.f, 1.f));
		//m_pTransformCom->FixRotation(0.f, 180.f, 0.f);
		//
		//m_vOriginPos = m_pTransformCom->Get_Position();
		//m_vOriginLook = m_pTransformCom->Get_Look();
		//m_vRage3StartPos = Vec4(138.f, -0.5f, 132.f, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(161.5f, 2.311f, 147.5f, 1.f));
		m_pTransformCom->FixRotation(0.f, 195.f, 0.f);

		m_vOriginPos = m_pTransformCom->Get_Position();
		m_vOriginLook = m_pTransformCom->Get_Look();
		m_vRage3StartPos = Vec4(165.f, 2.311f, 177.f, 1.f);

	//}



	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stellia"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	//CPhysX_Controller::CONTROLLER_DESC ControllerDesc;
	//
	//ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	//ControllerDesc.pTransform = m_pTransformCom;
	//ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	//ControllerDesc.fHeight = 1.f;
	//ControllerDesc.fMaxJumpHeight = 10.f;
	//ControllerDesc.fRaidus = 5.f;
	//ControllerDesc.pOwner = this;
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;
	ControllerDesc.eType = CPhysX_Controller::BOX;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.vExtents = {5.f, 5.f, 7.f};
	//ControllerDesc.fHeight = 1.f;
	//ControllerDesc.fMaxJumpHeight = 10.f;
	//ControllerDesc.fRaidus = 5.f;
	ControllerDesc.pOwner = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For. Com_RigidBody*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;

	m_pStateCom->Set_Owner(this);

	/* For. Disslove Texture */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), TEXT("Com_DissolveTexture"), (CComponent**)&m_pDissoveTexture)))
	//	return E_FAIL;

	return S_OK;
}

#pragma region Ready_States
HRESULT CStellia::Ready_States()
{
	m_tStat.fMaxHp = 700000;
	m_tStat.fHp = 700000;
	m_tStat.iAtk = 350;
	m_tStat.iDef = 120;

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Spawn");
	m_pStateCom->Add_State(STELLIA_SPAWN, CStelliaState_Spawn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Stand");
	m_pStateCom->Add_State(STELLIA_SPAWNIDLE, CStelliaState_SpawnIdle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Stand02");
	m_pStateCom->Add_State(STELLIA_SPAWNSTAND, CStelliaState_SpawnStand::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Stand02");
	m_pStateCom->Add_State(STELLIA_COMBATIDLE, CStelliaState_CombatIdle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Run");
	m_pStateCom->Add_State(STELLIA_CHASE, CStelliaState_Chase::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Attack01");
	m_pStateCom->Add_State(STELLIA_ATTACK1, CStelliaState_Attack1::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill01");
	m_pStateCom->Add_State(STELLIA_AFTERSPINTAIL, CStelliaState_AfterSpinTail::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Attack02");
	m_pStateCom->Add_State(STELLIA_ATTACK2, CStelliaState_Attack2::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill01");
	m_pStateCom->Add_State(STELLIA_SPINTAIL, CStelliaState_SpinTail::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill02_New");
	m_pStateCom->Add_State(STELLIA_JUMPSTAMP, CStelliaState_JumpStamp::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill05_New");
	m_pStateCom->Add_State(STELLIA_LASER, CStelliaState_Laser::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill03_New");
	m_pStateCom->Add_State(STELLIA_TRIPLELASER, CStelliaState_TripleLaser::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06");
	m_pStateCom->Add_State(STELLIA_CHARGE, CStelliaState_Charge::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill04_New");
	m_pStateCom->Add_State(STELLIA_BIGBANG, CStelliaState_BigBang::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkillChaseJump");
	m_pStateCom->Add_State(STELLIA_CHASEJUMPSTAMP, CStelliaState_ChaseJumpStamp::Create(m_pStateCom, strAnimationName));


	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_CounterStart");
	m_pStateCom->Add_State(STELLIA_COUNTERSTART, CStelliaState_CounterStart::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_CounterLoop");
	m_pStateCom->Add_State(STELLIA_COUNTERLOOP, CStelliaState_CounterLoop::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_CounterEnd");
	m_pStateCom->Add_State(STELLIA_COUNTEREND, CStelliaState_CounterEnd::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkillRage");
	m_pStateCom->Add_State(STELLIA_BERSERK, CStelliaState_Berserk::Create(m_pStateCom, strAnimationName));

	/* Rage 1*/
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_InstantTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_LeftTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn180");
	m_pStateCom->Add_State(STELLIA_RAGE1START_TURN_OC, CStelliaState_Rage1StartTurnOC::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill02_New");
	m_pStateCom->Add_State(STELLIA_RAGE1START_JUMP, CStelliaState_Rage1StartJump::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_InstantTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_LeftTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn180");
	m_pStateCom->Add_State(STELLIA_RAGE1START_TURN_OL, CStelliaState_Rage1StartTurnOL::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkillRage");
	m_pStateCom->Add_State(STELLIA_RAGE1START, CStelliaState_Rage1Start::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_InstantTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_LeftTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn180");
	m_pStateCom->Add_State(STELLIA_RAGE1LOOP_TURN, CStelliaState_Rage1Loop_Turn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Stand02");
	m_pStateCom->Add_State(STELLIA_RAGE1LOOP_IDLE, CStelliaState_Rage1Loop_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill04");
	m_pStateCom->Add_State(STELLIA_RAGE1LOOP_EXPLOSION, CStelliaState_Rage1Loop_Explosion::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill01");
	m_pStateCom->Add_State(STELLIA_RAGE1LOOP_SPINTAIL, CStelliaState_Rage1Loop_SpinTail::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill02");
	m_pStateCom->Add_State(STELLIA_RAGE1LOOP_JUMPSTAMP, CStelliaState_Rage1Loop_JumpStamp::Create(m_pStateCom, strAnimationName));

	/* Rage 2 */
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_InstantTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_LeftTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn180");
	m_pStateCom->Add_State(STELLIA_RAGE2START_TURN_OC, CStelliaState_Rage2StartTurnOC::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill02_New");
	m_pStateCom->Add_State(STELLIA_RAGE2START_JUMP, CStelliaState_Rage2StartJump::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_InstantTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_LeftTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn180");
	m_pStateCom->Add_State(STELLIA_RAGE2START_TURN_OL, CStelliaState_Rage2StartTurnOL::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkillRage");
	m_pStateCom->Add_State(STELLIA_RAGE2START, CStelliaState_Rage2Start::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06_New_Start");
	m_pStateCom->Add_State(STELLIA_RAGE2LOOP, CStelliaState_Rage2Loop::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill04_New");
	m_pStateCom->Add_State(STELLIA_RAGE2FINISH, CStelliaState_Rage2Finish::Create(m_pStateCom, strAnimationName));
	
	/* Rage3 */
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkillRage");
	m_pStateCom->Add_State(STELLIA_RAGE3START_FADEOUT, CStelliaState_Rage3Start_FadeOut::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Stand");
	m_pStateCom->Add_State(STELLIA_RAGE3START_FADEIN, CStelliaState_Rage3Start_FadeIn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Sprint");
	m_pStateCom->Add_State(STELLIA_RAGE3AROUND, CStelliaState_Rage3Around::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06_New_End");
	m_pStateCom->Add_State(STELLIA_RAGE3AROUND_BREAK, CStelliaState_Rage3AroundBreak::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_LeftTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn");
	m_pStateCom->Add_State(STELLIA_RAGE3TURN_AROUND, CStelliaState_Rage3TurnAround::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn180");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_LeftTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_InstantTurn");
	m_pStateCom->Add_State(STELLIA_RAGE3TURN_PL, CStelliaState_Rage3TurnPL::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06_New_Start");
	m_pStateCom->Add_State(STELLIA_RAGE3CLAW_READY, CStelliaState_Rage3ClawReady::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06_New_Loop");
	m_pStateCom->Add_State(STELLIA_RAGE3CLAW_CHARGE, CStelliaState_Rage3ClawCharge::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Attack01");
	m_pStateCom->Add_State(STELLIA_RAGE3CLAW, CStelliaState_Rage3Claw::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06_New_Loop");
	m_pStateCom->Add_State(STELLIA_RAGE3CLAW_ENDCHARGE, CStelliaState_Rage3ClawEndCharge::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06_New_End");
	m_pStateCom->Add_State(STELLIA_RAGE3CLAW_ENDBREAK, CStelliaState_Rage3ClawEndBreak::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06_New_Start");
	m_pStateCom->Add_State(STELLIA_RAGE3CHARGE_READY, CStelliaState_Rage3ChargeReady::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06_New_Loop");
	m_pStateCom->Add_State(STELLIA_RAGE3CHARGE, CStelliaState_Rage3Charge::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06_New_End");
	m_pStateCom->Add_State(STELLIA_RAGE3CHARGE_BREAK, CStelliaState_Rage3ChargeBreak::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_CounterEnd");
	m_pStateCom->Add_State(STELLIA_RAGE3DAMAGE, CStelliaState_Rage3Damage::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06_New_Loop");
	m_pStateCom->Add_State(STELLIA_RAGE3ESCAPE, CStelliaState_Rage3Escape::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn180");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_LeftTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_InstantTurn");
	m_pStateCom->Add_State(STELLIA_RAGE3TURN_OC, CStelliaState_Rage3TurnOC::Create(m_pStateCom, strAnimationName));
	// 

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_InstantTurn");
	m_pStateCom->Add_State(STELLIA_BACKSTEP, CStelliaState_BackStep::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn180");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_LeftTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_InstantTurn");
	m_pStateCom->Add_State(STELLIA_TURN, CStelliaState_Turn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Death");
	m_pStateCom->Add_State(STELLIA_DEAD, CStelliaState_Dead::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(STELLIA_SPAWN);
	//m_pStateCom->Change_State(STELLIA_RAGE3START_FADEIN);

	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CStellia::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 200.f, 250.f, 650.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(-50.f, 250.f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	/* Atk */
	OBBDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);
	OBBBox.Extents = { 0.f, 0.f, 0.f };
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;

	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 15.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CStellia::Ready_Sockets()
{
	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CStellia::Ready_Parts()
{
	return S_OK;
}

HRESULT CStellia::Create_Crystals()
{
	m_pCrystalController = new CStellia_Crystal_Controller();
	if (nullptr == m_pCrystalController)
		return E_FAIL;

	if (FAILED(m_pCrystalController->Create_Crystals(this)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStellia::Clear_Crystals()
{
	if (nullptr == m_pCrystalController)
		return E_FAIL;

	m_pCrystalController->Clear_Crystals();

	return S_OK;
}

void CStellia::Set_CrystalTurnData()
{
	m_pCrystalController->Set_CrystalTurnData();
}

#pragma endregion

CStellia* CStellia::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CStellia* pInstance = new CStellia(pDevice, pContext, strObjectTag, tStat);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CStellia");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CStellia::Clone(void* pArg)
{
	CStellia* pInstance = new CStellia(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStellia");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStellia::Free()
{
	__super::Free();
	
	Safe_Delete(m_pCrystalController);
}

