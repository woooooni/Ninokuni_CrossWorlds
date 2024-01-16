#include "stdafx.h"
#include "GlanixState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Glanix.h"

#include "Game_Manager.h"
#include "Player.h"

_uint CGlanixState_Base::m_iAtkIndex = 0;

CGlanixState_Base::CGlanixState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CGlanixState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_pGlanix = dynamic_cast<CGlanix*>(m_pOwner);
	
	m_pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if(m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	m_fRunSpeed = 6.f;

	/* 공격 패턴만 따로 모아놓기. (후에 순차적 혹은 랜덤으로 전환하기 위해) */
	// m_vecAtkState.push_back(CGlanix::GLANIX_SNOWBALL);
	m_vecAtkState.push_back(CGlanix::GLANIX_ATTACK1);
	m_vecAtkState.push_back(CGlanix::GLANIX_ATTACK2);
	m_vecAtkState.push_back(CGlanix::GLANIX_ICEWAVE);
	m_vecAtkState.push_back(CGlanix::GLANIX_QUADBLOW);
	m_vecAtkState.push_back(CGlanix::GLANIX_ATTACK2);
	m_vecAtkState.push_back(CGlanix::GLANIX_SPINBOMB);
	m_vecAtkState.push_back(CGlanix::GLANIX_ATTACK1);

	m_MotionTrailDesc.vRimColor = { 1.f, 1.f, 1.f, 1.f };
	m_MotionTrailDesc.fMotionTrailTime = 0.5f;
	m_MotionTrailDesc.vBloomPower = { 1.f, 1.f, 1.f, };
	m_MotionTrailDesc.fBlurPower = 0.f;
	m_MotionTrailDesc.fAlphaSpeed = 1.f;

	return S_OK;
}

void CGlanixState_Base::Enter_State(void* pArg)
{

}

void CGlanixState_Base::Tick_State(_float fTimeDelta)
{
	/* Dead */
	if (m_pGlanix->Get_Stat().fHp <= 0.f)
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_DEAD);
}

void CGlanixState_Base::Exit_State()
{
}

void CGlanixState_Base::Init_Pattern()
{
	m_vecAtkState.clear();

	m_vecAtkState.push_back(CGlanix::GLANIX_ATTACK2);
	m_vecAtkState.push_back(CGlanix::GLANIX_CHARGE);
	m_vecAtkState.push_back(CGlanix::GLANIX_ICEWAVE);
	m_vecAtkState.push_back(CGlanix::GLANIX_QUADBLOW);
	m_vecAtkState.push_back(CGlanix::GLANIX_SPINBOMBBOMB);
	m_vecAtkState.push_back(CGlanix::GLANIX_SNOWBALL);
}

_bool CGlanixState_Base::State_Wait(_float fDestTime, _float fTimeDelta)
{
	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() != TEXT("SKM_Glanix.ao|Glanix_BattleStand"))
	{
		m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));
	}

	m_fTime += fTimeDelta;

	if (m_fTime >= fDestTime)
	{
		m_fTime = fDestTime - m_fTime;
		return true;
	}

	return false;
}

void CGlanixState_Base::Start_Pattern()
{
	if (m_iAtkIndex >= m_vecAtkState.size())
		m_iAtkIndex = 0;

	// 근접 공격 패턴이면 
	if (m_vecAtkState[m_iAtkIndex] == CGlanix::GLANIX_ATTACK1 || 
		m_vecAtkState[m_iAtkIndex] == CGlanix::GLANIX_ATTACK2 ||
		m_vecAtkState[m_iAtkIndex] == CGlanix::GLANIX_SPINBOMB || 
		m_vecAtkState[m_iAtkIndex] == CGlanix::GLANIX_SPINBOMBBOMB)
	{
		if (m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_ATKAROUND))
		{
			m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
		}
		else
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_CHASE);
	}
	// 장거리 패턴이면(사정거리 긴)
	else
	{
		if (m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_SKILLAROUND))
		{
			m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
		}
		else
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_CHASE);
	}
}

void CGlanixState_Base::Free()
{
	__super::Free();
}
