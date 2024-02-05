#include "stdafx.h"
#include "State_RubyIdle.h"

#include "GameNpc.h"

#include "Animation.h"
#include "GameInstance.h"
#include "RubyCarriage.h"
#include "Ruby.h"

CState_RubyIdle::CState_RubyIdle(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CState_RubyIdle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CState_RubyIdle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);

	CRuby* pRuby = static_cast<CRuby*>(m_pOwner);

	m_pRubyCarriage = static_cast<CRubyCarriage*>(pRuby->Get_RidingObject());
	if (nullptr == m_pRubyCarriage)
		return;
}

void CState_RubyIdle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	/* 여분의 Idle 애니메이션이 있을 때에만. */
	_bool bTake = m_pRubyCarriage->TakeTheCarriage();

	if (true == bTake && true == m_pModelCom->Is_Finish() && false == m_pModelCom->Is_Tween())
		m_pStateMachineCom->Change_State(CRuby::RUBY_STATE::RUBY_SEAT);
}

void CState_RubyIdle::Exit_State()
{
}

CState_RubyIdle* CState_RubyIdle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_RubyIdle* pInstance = new CState_RubyIdle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CState_RubyIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_RubyIdle::Free()
{
	__super::Free();
}

