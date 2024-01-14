#include "stdafx.h"
#include "Glanix.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"
#include "UI_Manager.h"

#include "GlanixState_IntroIdle.h"
#include "GlanixState_IntroRoar.h"
#include "GlanixState_IntroJump.h"
#include "GlanixState_IntroFinish.h"

#include "GlanixState_Spawn.h"
#include "GlanixState_CombatIdle.h"
#include "GlanixState_Chase.h"

#include "GlanixState_Attack1.h"
#include "GlanixState_Attack2.h"
#include "GlanixState_SpinBomb.h"
#include "GlanixState_JumpStamp.h"
#include "GlanixState_QuadBlow.h"
#include "GlanixState_IceWave.h"
#include "GlanixState_Charge.h"
#include "GlanixState_SnowBall.h"
#include "GlanixState_SpinBombBomb.h"

#include "GlanixState_Berserk.h"

#pragma region Rage1
#include "GlanixState_RageStartTurnOC.h"
#include "GlanixState_RageStartJump.h"
#include "GlanixState_RageStartTurnOL.h"
#include "GlanixState_RageStart.h"

#include "GlanixState_RageTurn.h"
#include "GlanixState_RageIdle.h"

#include "GlanixState_RageCharge.h"
#include "GlanixState_RageChargeEnd.h"

#include "GlanixState_RageReturn.h"
#include "GlanixState_RageFinishTurnOL.h"
#include "GlanixState_RagePull.h"
#include "GlanixState_RageRising.h"
#include "GlanixState_RageStamp.h"
#include "GlanixState_RageCrash.h"
#pragma endregion

#pragma region Rage2
#include "GlanixState_Rage2StartTurnWP.h"
#include "GlanixState_Rage2StartJump.h"
#include "GlanixState_Rage2StartTurnOL.h"
#include "GlanixState_Rage2Start.h"

#include "GlanixState_Rage2Wave.h"
#include "GlanixState_Rage2Idle.h"

#include "GlanixState_Rage2Rising.h"
#include "GlanixState_Rage2Stamp.h"
#pragma endregion

#include "GlanixState_Turn.h"
#include "GlanixState_Stun.h"
#include "GlanixState_CounterStart.h"
#include "GlanixState_CounterLoop.h"
#include "GlanixState_CounterEnd.h"
#include "GlanixState_Dead.h"

#include "Camera_Manager.h"
#include "Quest_Manager.h"

#include "Glanix_IcePillar.h"

#include "Glanix_IcePillar_Controller.h"

CGlanix::CGlanix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CBoss(pDevice, pContext, strObjectTag, tStat)
{
}

CGlanix::CGlanix(const CGlanix& rhs)
	: CBoss(rhs)
{

}

HRESULT CGlanix::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGlanix::Initialize(void* pArg)
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

	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
	{
		if (FAILED(CUI_Manager::GetInstance()->Ready_BossHPBar(this)))
			return E_FAIL;
	}

	m_iObjectType = OBJ_TYPE::OBJ_BOSS;

	m_vBloomPower = _float3(0.4f, 0.4f, 0.4f);

	return S_OK;
}

void CGlanix::Tick(_float fTimeDelta)
{
	/* 최초 등장 대기 시간 */
	if (!m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_INTRO])
	{
		m_fIntroTime += fTimeDelta;
		if (m_fIntroTime > 3.f)
		{
			m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_INTRO] = true;
			m_pStateCom->Change_State(GLANIX_INTRO_ROAR);
		}
	}

	/* 임시 */
	if (KEY_TAP(KEY::Z))
		m_tStat.fHp -= m_tStat.fMaxHp * 0.1f;

#ifdef _DEBUG
	if (LEVELID::LEVEL_TOOL == GI->Get_CurrentLevel() && KEY_TAP(KEY::B))
		m_pStateCom->Change_State(GLANIX_QUADBLOW);
#endif // DEBUG

	m_pStateCom->Tick_State(fTimeDelta);

	__super::Tick(fTimeDelta);

	if (nullptr != m_pPillarController)
		m_pPillarController->Tick(fTimeDelta);
}

void CGlanix::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CGlanix::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CGlanix::Collision_Enter(const COLLISION_INFO& tInfo)
{
	/* 플레이어와 충돌 */
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_ATKAROUND] = true;
	}

	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
	{
		m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_SKILLAROUND] = true;
	}

	/* 피격 */
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
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
				m_pStateCom->Change_State(GLANIX_COUNTERSTART);
				m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_COUNTER] = false;
			}
		}
	}
}

void CGlanix::Collision_Continue(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_ATKAROUND] = true;
	}

	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
	{
		m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_SKILLAROUND] = true;
	}
}

void CGlanix::Collision_Exit(const COLLISION_INFO& tInfo)
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

void CGlanix::On_Damaged(const COLLISION_INFO& tInfo)
{
	__super::On_Damaged(tInfo);
}

void CGlanix::Set_SkillTree()
{
	map<_uint, CState*> pStates = m_pStateCom->Get_States();

	for (auto& iter : pStates)
	{
		dynamic_cast<CGlanixState_Base*>(iter.second)->Init_Pattern();
	}
}

HRESULT CGlanix::Create_Pillars()
{
	m_pPillarController = new CGlanix_IcePillar_Controller();
	if (nullptr == m_pPillarController)
		return E_FAIL;

	if (FAILED(m_pPillarController->Create_Pillars(6, 20.f, m_vOriginPos, this)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlanix::Delete_Pillar(const _int& iKey)
{
	if (nullptr == m_pPillarController)
		return E_FAIL;

	m_pPillarController->Delete_Pillar(iKey);
}

HRESULT CGlanix::Clear_Pillars()
{
	if (nullptr == m_pPillarController)
		return E_FAIL;

	m_pPillarController->Clear_Pillars();

	delete m_pPillarController;

	m_pPillarController = nullptr;

	return E_NOTIMPL;
}

HRESULT CGlanix::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 10.f, 0.f, 1.f));
	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-40.f, 1.6, 361.f, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-27.f, 25.35f, 425.f, 1.f));
	m_pTransformCom->FixRotation(0.f, 180.f, 0.f);

	m_vOriginLook = m_pTransformCom->Get_Look();
	//m_vOriginPos = XMVectorSet(-40.f, 1.6, 361.f, 1.f);
	m_vOriginPos = XMVectorSet(-55.f, 1.6, 363.f, 1.f);
	m_vWavePoint = { -63.f, 1.6f, 393.f, 1.f };

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Glanix"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
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
HRESULT CGlanix::Ready_States()
{
	m_tStat.fMaxHp = 1000000;
	m_tStat.fHp = 1000000;
	m_tStat.iAtk = 250;
	m_tStat.iDef = 200;

	list<wstring> strAnimationName;
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BattleStand");
	m_pStateCom->Add_State(GLANIX_INTRO_IDLE, CGlanixState_IntroIdle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BossSkillRage");
	m_pStateCom->Add_State(GLANIX_INTRO_ROAR, CGlanixState_IntroRoar::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_IntroJump");
	m_pStateCom->Add_State(GLANIX_INTRO_JUMP, CGlanixState_IntroJump::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_IntroFinish01");
	m_pStateCom->Add_State(GLANIX_INTRO_FINISH, CGlanixState_IntroFinish::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Spawn_New");
	m_pStateCom->Add_State(GLANIX_SPAWN, CGlanixState_Spawn::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BattleStand");
	m_pStateCom->Add_State(GLANIX_COMBATIDLE, CGlanixState_CombatIdle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Battlerun");
	m_pStateCom->Add_State(GLANIX_CHASE, CGlanixState_Chase::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Attack01");
	m_pStateCom->Add_State(GLANIX_ATTACK1, CGlanixState_Attack1::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Attack02");
	m_pStateCom->Add_State(GLANIX_ATTACK2, CGlanixState_Attack2::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill01");
	m_pStateCom->Add_State(GLANIX_SPINBOMB, CGlanixState_SpinBomb::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill03");
	m_pStateCom->Add_State(GLANIX_JUMPSTAMP, CGlanixState_JumpStamp::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill02_New");
	m_pStateCom->Add_State(GLANIX_QUADBLOW, CGlanixState_QuadBlow::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill06");
	m_pStateCom->Add_State(GLANIX_ICEWAVE, CGlanixState_IceWave::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill07_RootMotion");
	m_pStateCom->Add_State(GLANIX_CHARGE, CGlanixState_Charge::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill08");
	m_pStateCom->Add_State(GLANIX_SNOWBALL, CGlanixState_SnowBall::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill09");
	m_pStateCom->Add_State(GLANIX_SPINBOMBBOMB, CGlanixState_SpinBombBomb::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BossSkillRage");
	m_pStateCom->Add_State(GLANIX_BERSERK, CGlanixState_Berserk::Create(m_pStateCom, strAnimationName));

#pragma region Rage1
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn180");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn180");
	m_pStateCom->Add_State(GLANIX_RAGESTART_TURN_OC, CGlanixState_RageStartTurnOC::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill03");
	m_pStateCom->Add_State(GLANIX_RAGESTART_JUMP, CGlanixState_RageStartJump::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn180");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn180");
	m_pStateCom->Add_State(GLANIX_RAGESTART_TURN_OL, CGlanixState_RageStartTurnOL::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BossSkillRage");
	m_pStateCom->Add_State(GLANIX_RAGESTART, CGlanixState_RageStart::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn180");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn180");
	m_pStateCom->Add_State(GLANIX_RAGETURN, CGlanixState_RageTurn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BattleStand");
	m_pStateCom->Add_State(GLANIX_RAGEIDLE, CGlanixState_RageIdle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RageSkillCharge");
	m_pStateCom->Add_State(GLANIX_RAGECHARGE, CGlanixState_RageCharge::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill07_Finish");
	m_pStateCom->Add_State(GLANIX_RAGECHARGE_END, CGlanixState_RageChargeEnd::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill03");
	m_pStateCom->Add_State(GLANIX_RAGERETURN, CGlanixState_RageReturn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn180");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn180");
	m_pStateCom->Add_State(GLANIX_FINISHTURN_OL, CGlanixState_RageFinishTurnOL::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RageSkillPull");
	m_pStateCom->Add_State(GLANIX_RAGEPULL, CGlanixState_RagePull::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RageFinalJump");
	m_pStateCom->Add_State(GLANIX_RAGERISING, CGlanixState_RageRising::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RageFinalStamp");
	m_pStateCom->Add_State(GLANIX_RAGESTAMP, CGlanixState_RageStamp::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Crash");
	m_pStateCom->Add_State(GLANIX_RAGECRASH, CGlanixState_RageCrash::Create(m_pStateCom, strAnimationName));

#pragma endregion 

#pragma region Rage2
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn180");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn180");
	m_pStateCom->Add_State(GLANIX_RAGE2START_TURN_WP, CGlanixState_Rage2StartTurnWP::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill03");
	m_pStateCom->Add_State(GLANIX_RAGE2START_JUMP, CGlanixState_Rage2StartJump::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn180");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn180");
	m_pStateCom->Add_State(GLANIX_RAGE2START_TURN_OL, CGlanixState_Rage2StartTurnOL::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BossSkillRage");
	m_pStateCom->Add_State(GLANIX_RAGE2START, CGlanixState_Rage2Start::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill06");
	m_pStateCom->Add_State(GLANIX_RAGE2WAVE, CGlanixState_Rage2Wave::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BattleStand");
	m_pStateCom->Add_State(GLANIX_RAGE2IDLE, CGlanixState_Rage2Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RageFinalJump");
	m_pStateCom->Add_State(GLANIX_RAGE2RISING, CGlanixState_Rage2Rising::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RageFinalStamp");
	m_pStateCom->Add_State(GLANIX_RAGE2STAMP, CGlanixState_Rage2Stamp::Create(m_pStateCom, strAnimationName));

#pragma endregion

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn180");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn180");
	m_pStateCom->Add_State(GLANIX_TURN, CGlanixState_Turn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BattleStand");
	m_pStateCom->Add_State(GLANIX_STUN, CGlanixState_Stun::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_CounterEnter");
	m_pStateCom->Add_State(GLANIX_COUNTERSTART, CGlanixState_CounterStart::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_CounterLoop");
	m_pStateCom->Add_State(GLANIX_COUNTERLOOP, CGlanixState_CounterLoop::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_CounterOut");
	m_pStateCom->Add_State(GLANIX_COUNTEREND, CGlanixState_CounterEnd::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Death");
	m_pStateCom->Add_State(GLANIX_DEAD, CGlanixState_Dead::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(GLANIX_INTRO_IDLE);

	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CGlanix::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 200.f, 175.f, 250.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 175.f, -50.f);

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
	tSphere.Radius = 8.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CGlanix::Ready_Sockets()
{
	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CGlanix::Ready_Parts()
{
	return S_OK;
}
#pragma endregion

CGlanix* CGlanix::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CGlanix* pInstance = new CGlanix(pDevice, pContext, strObjectTag, tStat);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CGlanix");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CGlanix::Clone(void* pArg)
{
	CGlanix* pInstance = new CGlanix(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGlanix");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix::Free()
{
	__super::Free();
}

