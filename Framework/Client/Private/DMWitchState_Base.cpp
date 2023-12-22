#include "stdafx.h"
#include "DMWitchState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "DMWitch.h"

_uint CDMWitchState_Base::m_iAtkIndex = 0;

CDMWitchState_Base::CDMWitchState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CDMWitchState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_pWitch = dynamic_cast<CDMWitch*>(m_pOwner);

	/* 후에 플레이어로 교체 */
	for (auto iter : GI->Find_GameObjects(LEVEL_TEST, LAYER_CHARACTER))
	{
		m_pPlayer = dynamic_cast<CCharacter_Dummy*>(iter);
	}
	if (m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	m_fRunSpeed = 4.f;

	// 레이지 전환 어떻게 할 지 생각하기. 

	/* 공격 패턴만 따로 모아놓기. (후에 순차적 혹은 랜덤으로 전환하기 위해) */
	m_vecAtkState.push_back(CDMWitch::DMWITCH_ATTACK1);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_ATTACK2);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_COLLFIRE);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_QUADBOMB);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_VULCANBALL);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_CHARGE);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_TRAMPOLINE);

	return S_OK;
}

void CDMWitchState_Base::Enter_State(void* pArg)
{
}

void CDMWitchState_Base::Tick_State(_float fTimeDelta)
{
	/* Dead */
	if (m_pWitch->Get_Stat().fHp <= 0.f)
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_DEAD);
}

void CDMWitchState_Base::Exit_State()
{
}

void CDMWitchState_Base::Init_Pattern()
{
	m_vecAtkState.clear();

	m_vecAtkState.push_back(CDMWitch::DMWITCH_ATTACK2);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_QUADBOMB);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_PULLANDCOMBO);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_CHARGE);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_TRAMPOLINE);
	m_vecAtkState.push_back(CDMWitch::DMWITCH_CHILDDREAM_START);
}

_bool CDMWitchState_Base::State_Wait(_float fDestTime, _float fTimeDelta)
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

void CDMWitchState_Base::Free()
{
	__super::Free();
}
