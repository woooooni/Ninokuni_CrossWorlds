#include "stdafx.h"
#include "State_CurlingGame_Adjust_Intensity.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Effect_Manager.h"

#include "Camera_Group.h"
#include "CurlingGame_Group.h"

#include "Player.h"
#include "Character.h"


CState_CurlingGame_Adjust_Intensity::CState_CurlingGame_Adjust_Intensity(CManager_StateMachine* pStateMachine)
	: CState_CurlingGame_Base(pStateMachine)
{
}

HRESULT CState_CurlingGame_Adjust_Intensity::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CState_CurlingGame_Adjust_Intensity::Enter_State(void* pArg)
{
	if (nullptr == m_pManager->m_pCurStone)
		return;

	m_tGuageDesc.Start();
}

void CState_CurlingGame_Adjust_Intensity::Tick_State(const _float& fTimeDelta)
{
	if (!m_tGuageDesc.bActive)
	{
		
	}
	else
	{
		m_tGuageDesc.Tick(fTimeDelta);

		if (m_pManager->m_bPlayerTurn)
		{
			if (KEY_AWAY(KEY::SPACE))
			{
				m_pManager->m_pCurStone->Launch(m_pManager->m_vCurStoneLook.ZeroY().Normalized(), m_tGuageDesc.fMaxPower * m_tGuageDesc.tLerpValue.fCurValue);

				m_pManager->m_pStonesLaunched.push_back(m_pManager->m_pCurStone);

				m_tGuageDesc.Stop();

				m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_PLAYER].iNumStone--;

				if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::LAUNCH)))
					return;
			}

		}
		else
		{
			if (KEY_TAP(KEY::Q))
			{
				/* 저장된 힘으로 사용 */
				m_tGuageDesc.tLerpValue.fCurValue = m_pManager->m_tCurAiPath.fPower;

				m_pManager->m_pCurStone->Launch(m_pManager->m_vCurStoneLook.ZeroY().Normalized(), m_tGuageDesc.fMaxPower * m_tGuageDesc.tLerpValue.fCurValue);

				m_pManager->m_pStonesLaunched.push_back(m_pManager->m_pCurStone);

				m_tGuageDesc.Stop();

				m_pManager->m_tParticipants[CCurlingGame_Manager::PARTICIPANT_NPC].iNumStone--;

				if (FAILED(m_pManager_StateMachine->Change_State(CCurlingGame_Manager::CURLINGGAME_STATE::LAUNCH)))
					return;
			}
		}
	}
}

void CState_CurlingGame_Adjust_Intensity::LateTick_State(const _float& fTimeDelta)
{
	Vec2		vPos = { g_iWinSizeX * 0.2f, g_iWinSizeY * 0.5f };
	const Vec2	vScale(0.4f);
	const wstring wstrFont = L"Default_Bold";
	CRenderer* pRenderer = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CRenderer>(L"Com_Renderer");

	CRenderer::TEXT_DESC desc = {};
	{
		desc.strText = L"Power : " + to_wstring(_int(m_tGuageDesc.tLerpValue.fCurValue * 100.f));
		desc.vPosition = vPos;
		desc.vColor = (Vec4)DirectX::Colors::DarkViolet;
		desc.strFontTag = wstrFont;
		desc.vScale = vScale * 1.5f;
	}

	pRenderer->Add_Text(desc);
}

void CState_CurlingGame_Adjust_Intensity::Exit_State()
{
}

HRESULT CState_CurlingGame_Adjust_Intensity::Render()
{
	return S_OK;
}

CState_CurlingGame_Adjust_Intensity* CState_CurlingGame_Adjust_Intensity::Create(CManager_StateMachine* pStateMachine)
{
	CState_CurlingGame_Adjust_Intensity* pInstance = new CState_CurlingGame_Adjust_Intensity(pStateMachine);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CState_CurlingGame_Adjust_Intensity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_CurlingGame_Adjust_Intensity::Free()
{
	__super::Free();
}

