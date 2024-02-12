#include "stdafx.h"
#include "NpcDMWState_Charge.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_Charge::CNpcDMWState_Charge(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Charge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	m_fChargeSpeed = 15.f;
	m_fAtkCoolTime = 0.1f;

	return S_OK;
}

void CNpcDMWState_Charge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	CSound_Manager::GetInstance()->Play_Sound(TEXT("DreamersMazeWitch_V_Skill_3.ogg"), CHANNELID::SOUND_VOICE_NPC, 1.f, true);

}

void CNpcDMWState_Charge::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() < 30)
	{
		if (CGame_Manager::GetInstance()->Get_Player()->Get_Character() != nullptr)
		{
			m_pTransformCom->LookAt_ForLandObject(CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform()->Get_Position());
		}
	}

	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill03") &&
		m_pModelCom->Get_CurrAnimationFrame() >= 30 && m_pModelCom->Get_CurrAnimationFrame() <= 70)
	{
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fChargeSpeed, fTimeDelta);
	
		// 第肺 备眉 积己
		m_fAccAtkCoolTime += fTimeDelta;
		if (m_fAccAtkCoolTime > m_fAtkCoolTime)
		{
			m_fAccAtkCoolTime = m_fAtkCoolTime - m_fAccAtkCoolTime;

			Vec4 vBulletPos = m_pTransformCom->Get_Position();

			_float fRandAngel = GI->RandomFloat(-45.f, 45.f);
			Vec3 vBulletLook = XMVector3Rotate(-m_pTransformCom->Get_Look(), XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(fRandAngel), 0.0f));

			CGameObject* pBullet = nullptr;

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Witch_ChargeBullet", m_pStellia, &pBullet)))
			{
				MSG_BOX("Add ChargeBullet Failed.");
				return;
			}

			CTransform* pBulletTransform = pBullet->Get_Component_Transform();
			vBulletPos.y += .5f;
			pBulletTransform->Set_Position(vBulletPos);
			pBulletTransform->Set_Look(vBulletLook);
		}

	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_CHARGE_END);
	}

}

void CNpcDMWState_Charge::Exit_State()
{
}

CNpcDMWState_Charge* CNpcDMWState_Charge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Charge* pInstance = new CNpcDMWState_Charge(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Charge::Free()
{
	__super::Free();
}

