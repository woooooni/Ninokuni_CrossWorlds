#include "stdafx.h"
#include "NpcDMWState_Rage2Sphere.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_Rage2Sphere::CNpcDMWState_Rage2Sphere(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Rage2Sphere::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_iCount = 5;

	m_fPlayerAtkTime = 5.f;
	m_fAllAtkTime = 0.5f;

	return S_OK;
}

void CNpcDMWState_Rage2Sphere::Enter_State(void* pArg)
{
	m_pStellia = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_MONSTER, TEXT("Stellia"));

	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	m_pWitch->Set_IsFollowing(true);

	m_bIsAttack = false;
}

void CNpcDMWState_Rage2Sphere::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (!m_bIsAttack)
	{
		for (_uint i = 0; i < m_iCount; ++i)
		{
			Vec4 vShperePos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position();
			vShperePos.x += GI->RandomFloat(-7.f, 7.f);
			vShperePos.y += 2.f;
			vShperePos.z += GI->RandomFloat(-7.f, 7.f);

			CGameObject* pShpere = nullptr;

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Witch_Rage02Sphere", m_pStellia, &pShpere)))
			{
				MSG_BOX("Add Rage02Sphere Failed.");
				return;
			}

			CTransform* pShpereTransform = pShpere->Get_Component_Transform();

			pShpereTransform->Set_State(CTransform::STATE_POSITION, vShperePos);

		}

		m_bIsAttack = true;
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING);
	}
}

void CNpcDMWState_Rage2Sphere::Exit_State()
{
}

CNpcDMWState_Rage2Sphere* CNpcDMWState_Rage2Sphere::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Rage2Sphere* pInstance = new CNpcDMWState_Rage2Sphere(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Rage2Sphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Rage2Sphere::Free()
{
	__super::Free();
}

