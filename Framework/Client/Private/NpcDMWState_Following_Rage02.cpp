#include "stdafx.h"
#include "NpcDMWState_Following_Rage02.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_Following_Rage02::CNpcDMWState_Following_Rage02(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Following_Rage02::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	m_fSphereCreateTime = .3f;

	return S_OK;
}

void CNpcDMWState_Following_Rage02::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	m_pWitch->Set_IsFollowing(true);

	m_fAccTime = 0.f;
}

void CNpcDMWState_Following_Rage02::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_COUNTERSTART)
	{
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING);
		return;
	}

	if (m_pWitch->Get_IsBattle())
	{
		m_fAccTime += fTimeDelta;
		if (m_fAccTime >= m_fSphereCreateTime)
		{
			m_fAccTime = m_fSphereCreateTime - m_fAccTime;

			Vec4 vSpherePos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position();
			vSpherePos.x += GI->RandomFloat(-10.f, 10.f);
			vSpherePos.z += GI->RandomFloat(-10.f, 10.f);

			CGameObject* pSphere = nullptr;

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Witch_Rage02Sphere", m_pStellia, &pSphere)))
			{
				MSG_BOX("Add Sphere Failed.");
				return;
			}

			CTransform* pSphereTransform = pSphere->Get_Component_Transform();

			pSphereTransform->Set_State(CTransform::STATE_POSITION, vSpherePos);
		}
	}
}

void CNpcDMWState_Following_Rage02::Exit_State()
{
}

CNpcDMWState_Following_Rage02* CNpcDMWState_Following_Rage02::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Following_Rage02* pInstance = new CNpcDMWState_Following_Rage02(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Following_Rage02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Following_Rage02::Free()
{
	__super::Free();
}
