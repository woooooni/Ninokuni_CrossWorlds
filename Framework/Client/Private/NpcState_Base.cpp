#include "stdafx.h"
#include "NpcState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "GameNpc.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcState_Base::CNpcState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CNpcState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_pNpc = dynamic_cast<CGameNpc*>(m_pOwner);

	m_pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	return S_OK;
}

void CNpcState_Base::Enter_State(void* pArg)
{
}

void CNpcState_Base::Tick_State(_float fTimeDelta)
{
}

void CNpcState_Base::Exit_State()
{
}

_bool CNpcState_Base::State_Wait(_bool bIsLoop, wstring strAnimName, _float fDestTime, _float fTimeDelta)
{
	/* 루프 대기 동작 */
	if (bIsLoop)
	{
		if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() != strAnimName)
		{
			m_pModelCom->Set_Animation(strAnimName);
		}

		m_fTime += fTimeDelta;

		if (m_fTime >= fDestTime)
		{
			m_fTime = fDestTime - m_fTime;
			return true;
		}
	}

	/* 루프가 아닌 대기 동작 */
	else
	{
		if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() != strAnimName)
		{
			m_pModelCom->Set_Animation(strAnimName);
		}

		if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
			return true;
	}

	return false;
}

void CNpcState_Base::Free()
{
	__super::Free();
}
