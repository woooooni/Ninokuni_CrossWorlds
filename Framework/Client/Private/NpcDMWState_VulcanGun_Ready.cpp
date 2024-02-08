#include "stdafx.h"
#include "NpcDMWState_VulcanGun_Ready.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_VulcanGun_Ready::CNpcDMWState_VulcanGun_Ready(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_VulcanGun_Ready::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_fDist = 5.f;
	m_fMoveTime = 2.f;

	return S_OK;
}

void CNpcDMWState_VulcanGun_Ready::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	m_pWitch->Set_IsFollowing(true);

	m_bIsFindStellia = false;
}

void CNpcDMWState_VulcanGun_Ready::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	// 한 번만 세팅
	if (m_pStellia != nullptr && !m_bIsFindStellia)
	{
		Vec4 vStelliaPos = m_pStellia->Get_Component_Transform()->Get_Position();
		Vec3 vStelliaLook = m_pStellia->Get_Component_Transform()->Get_Look();
		vStelliaLook.Normalize();

		m_vDestPos = vStelliaPos + (vStelliaLook * m_fDist);
		m_tRage03Lerp.Start(m_pTransformCom->Get_Position(), m_vDestPos, m_fMoveTime, LERP_MODE::EASE_IN);

		m_bIsFindStellia = true;
	}

	if (CGame_Manager::GetInstance()->Get_Player()->Get_Character() != nullptr)
	{
		// 플레이어를 바라보면서 이동
		if (m_tRage03Lerp.bActive)
		{
			Vec4 vCurPos = m_tRage03Lerp.Update_Lerp(fTimeDelta);
			m_pTransformCom->LookAt_ForLandObject(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position());
			m_pTransformCom->Set_Position(vCurPos);
		}
		else // 이동 끝나면 넘어가기
			m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_VULCAN_ATTACK);
	}

}

void CNpcDMWState_VulcanGun_Ready::Exit_State()
{
}

CNpcDMWState_VulcanGun_Ready* CNpcDMWState_VulcanGun_Ready::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_VulcanGun_Ready* pInstance = new CNpcDMWState_VulcanGun_Ready(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_VulcanGun_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_VulcanGun_Ready::Free()
{
	__super::Free();
}

