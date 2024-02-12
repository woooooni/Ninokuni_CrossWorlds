#include "stdafx.h"
#include "DreamMazeWitch_Npc.h"

#include "GameInstance.h"

#include "UI_World_NPCTag.h"
#include "UI_World_NPCSpeechBalloon.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Talk.h"

#include "NpcDMWState_Idle.h"
#include "NpcDMWState_InvasionAppear.h"
#include "NpcDMWState_InvasionDisappearTurn.h"
#include "NpcDMWState_InvasionDisappear.h"

#include "NpcDMWState_Following.h"
#include "NpcDMWState_Attack.h"

#include "NpcDMWState_VulcanGun_Ready.h"
#include "NpcDMWState_VulcanGun_Attack.h"
#include "NpcDMWState_VulcanGun_End.h"

#include "NpcDMWState_Charge_Ready.h"
#include "NpcDMWState_Charge.h"
#include "NpcDMWState_Charge_End.h"

#include "NpcDMWState_Rage01QuadBlackHole.h"

#include "NpcDMWState_Following_Rage02.h"

#include "NpcDMWState_Following_Rage03.h"
#include "NpcDMWState_Rage03Laser.h"

#include "Game_Manager.h"
#include "Player.h"

#include "GameInstance.h"

CDreamMazeWitch_Npc::CDreamMazeWitch_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CDreamMazeWitch_Npc::CDreamMazeWitch_Npc(const CDreamMazeWitch_Npc& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CDreamMazeWitch_Npc::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDreamMazeWitch_Npc::Initialize(void* pArg)
{
	/* 툴용 */
	//m_bIsBattle = true;
	//m_bIsFollowing = true;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	// UI NameTag
	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_Tag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTag)
		return E_FAIL;

	m_pTag = dynamic_cast<CUI_World_NPCTag*>(pTag);
	m_pTag->Set_Owner(this, m_strKorName, 1.f);

	CGameObject* pBalloon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_SpeechBalloon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pBalloon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon))
		return E_FAIL;
	m_pBalloon = dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon);
	m_pBalloon->Set_Owner(this, 1.2f);
	m_pBalloon->Set_Balloon(TEXT(""));

	m_fFollowingSpeed = 12.f;

	return S_OK;
}

void CDreamMazeWitch_Npc::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_bIsBattle)
	{
		if (m_pStellia != nullptr)
		{
			if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_DEAD)
			{
				m_bIsBattle = false;
				return;
			}
			else
			{
				Following_Stellia(fTimeDelta);
			}
		}
	}

	//if (nullptr != m_pTag)
	//	m_pTag->Tick(fTimeDelta);
	//if (nullptr != m_pBalloon)
	//	m_pBalloon->Tick(fTimeDelta);
}

void CDreamMazeWitch_Npc::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//if (nullptr != m_pTag)
	//	m_pTag->LateTick(fTimeDelta);
	//if (nullptr != m_pBalloon)
	//	m_pBalloon->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CDreamMazeWitch_Npc::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CDreamMazeWitch_Npc::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CDreamMazeWitch_Npc::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CDreamMazeWitch_Npc::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CDreamMazeWitch_Npc::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_DreamerMazeWitch"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For. Com_RigidBody*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDreamMazeWitch_Npc::Ready_States()
{
	m_tStat.fSpeed = 6.f;

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(WITCHSTATE_INVASION_IDLE, CNpcDMWState_Idle::Create(m_pStateCom, strAnimationName));

	/* Invasion */
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_CSDreamersMazeWitch01");
	m_pStateCom->Add_State(WITCHSTATE_INVASION_APPEAR, CNpcDMWState_InvasionAppear::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(WITCHSTATE_INVASION_DISAPPEARTURN, CNpcDMWState_InvasionDisappearTurn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Walk");
	m_pStateCom->Add_State(WITCHSTATE_INVASION_DISAPPEAR, CNpcDMWState_InvasionDisappear::Create(m_pStateCom, strAnimationName));

	/* Battle */
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Walk");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_FOLLOWING, CNpcDMWState_Following::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Attack02");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_ATTACK, CNpcDMWState_Attack::Create(m_pStateCom, strAnimationName));
	
	// VulcanGun
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_VULCAN_READY, CNpcDMWState_VulcanGun_Ready::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill06");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_VULCAN_ATTACK, CNpcDMWState_VulcanGun_Attack::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_VULCAN_END, CNpcDMWState_VulcanGun_End::Create(m_pStateCom, strAnimationName));

	// Charge
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_CHARGE_READY, CNpcDMWState_Charge_Ready::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill03");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_CHARGE, CNpcDMWState_Charge::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_CHARGE_END, CNpcDMWState_Charge_End::Create(m_pStateCom, strAnimationName));

	// Rage1
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill04");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_QUADBLACKHOLE, CNpcDMWState_Rage01QuadBlackHole::Create(m_pStateCom, strAnimationName));

	// Rage2
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Walk");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_FOLLOWING_RAGE02, CNpcDMWState_Following_Rage02::Create(m_pStateCom, strAnimationName));

	// Rage3
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Walk");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_FOLLOWING_RAGE03, CNpcDMWState_Following_Rage03::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Attack02");
	m_pStateCom->Add_State(WITCHSTATE_BATTLE_RAGE3_LASER, CNpcDMWState_Rage03Laser::Create(m_pStateCom, strAnimationName));


	// 툴용
	//m_pStateCom->Change_State(WITCHSTATE_BATTLE_FOLLOWING);
	//m_bIsBattle = true;
	//


	m_pStateCom->Change_State(WITCHSTATE_INVASION_IDLE);

	return S_OK;
}

void CDreamMazeWitch_Npc::Witch_BattleSet(CGameObject* pStellia)
{
	m_pStellia = pStellia;
	// m_pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
	
	if (m_pStellia == nullptr)
		MSG_BOX("Stellia Is Null");
	if (m_pPlayer == nullptr)
		MSG_BOX("Player Is Null");

	m_bIsBattle = true;
	m_bIsFollowing = true;
}

HRESULT CDreamMazeWitch_Npc::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDreamMazeWitch_Npc::Ready_Sockets()
{
	return S_OK;
}

void CDreamMazeWitch_Npc::Following_Stellia(_float fTimeDelta)
{
	if (m_bIsFollowing)
	{
	}

	if (m_bIsFollowingPlayer)
	{
		if (!m_bIsCheckDestPos)
		{
			m_fAccMoveWait += fTimeDelta;
			if (m_fAccMoveWait >= m_fMoveWait)
			{
				Vec4 vLookToWitch = dynamic_cast<CStellia*>(m_pStellia)->Get_OriginPos() - Vec4(m_pTransformCom->Get_Position());

				_float fRandAngle = GI->RandomFloat(10.f, 170.f);
				vLookToWitch = XMVector3Rotate(m_pTransformCom->Get_Look(), XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(fRandAngle), 0.0f));
				vLookToWitch.Normalize();

				m_vStartPos = m_pTransformCom->Get_Position();
				m_vDestPos = dynamic_cast<CStellia*>(m_pStellia)->Get_OriginPos() + (vLookToWitch * GI->RandomFloat(3.f, 20.f));
				m_bIsCheckDestPos = true;

				m_tRage03Lerp.Start(m_vStartPos, m_vDestPos, m_fMoveTime, LERP_MODE::EASE_IN);

				m_fAccMoveWait = m_fMoveWait - m_fAccMoveWait;
			}
		}
		else
		{
			if (m_tRage03Lerp.bActive)
			{
				Vec4 vCurPos = m_tRage03Lerp.Update_Lerp(fTimeDelta);
				vCurPos.y = 5.f;
				m_pTransformCom->LookAt_ForLandObject(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position());
				m_pTransformCom->Set_Position(vCurPos);
			}
			else
				m_bIsCheckDestPos = false;
		}


		//m_pTransformCom->LookAt_ForLandObject(m_pPlayer->Get_Component_Transform()->Get_Position());
		//Vec4 vStelliaPos = m_pPlayer->Get_Component_Transform()->Get_Position();
		//
		//Vec4 vReleativePos = m_pPlayer->Get_Component_Transform()->Get_RelativeOffset({ 1.f , vStelliaPos.y + 5.f, 10.f, 1.f });
		//
		//Vec4 vDestPos = Vec4(vReleativePos + vStelliaPos).OneW();
		//
		//if (Vec4::Zero == m_vCurPos || m_fDampingLimitDistance < Vec4::Distance(vDestPos, m_vCurPos))
		//{
		//	m_vCurPos = vDestPos;
		//}
		//else
		//{
		//	const Vec4 vDist = (vDestPos.ZeroW() - m_vCurPos.ZeroW()) * m_fDampingCoefficient;
		//	m_vCurPos += vDist;
		//	m_vCurPos.y = vDestPos.y;
		//}
		//
		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(m_vCurPos, 1.f));
	}
}

CDreamMazeWitch_Npc* CDreamMazeWitch_Npc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CDreamMazeWitch_Npc* pInstance = new CDreamMazeWitch_Npc(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CDreamMazeWitch_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDreamMazeWitch_Npc::Clone(void* pArg)
{
	CDreamMazeWitch_Npc* pInstance = new CDreamMazeWitch_Npc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CDreamMazeWitch_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDreamMazeWitch_Npc::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
	Safe_Release(m_pBalloon);
}