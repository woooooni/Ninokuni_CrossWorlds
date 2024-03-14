#include "stdafx.h"
#include "InvasionState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Monster.h"

CInvasionState_Base::CInvasionState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CInvasionState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CInvasionState_Base::Enter_State(void* pArg)
{
}

void CInvasionState_Base::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() == true && m_pModelCom->Is_Tween() == false)
		m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CInvasionState_Base::Exit_State()
{
}

void CInvasionState_Base::Free()
{
	__super::Free();
}
