#include "stdafx.h"
#include "Glanix_PhoenixState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Glanix_Phoenix.h"

#include "Game_Manager.h"
#include "Player.h"

CGlanixPhoenixState_Base::CGlanixPhoenixState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CGlanixPhoenixState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_pPhoenix = dynamic_cast<CGlanix_Phoenix*>(m_pOwner);

	m_pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	return S_OK;
}

void CGlanixPhoenixState_Base::Enter_State(void* pArg)
{
}

void CGlanixPhoenixState_Base::Tick_State(_float fTimeDelta)
{
}

void CGlanixPhoenixState_Base::Exit_State()
{
}

void CGlanixPhoenixState_Base::Free()
{
	__super::Free();
}
