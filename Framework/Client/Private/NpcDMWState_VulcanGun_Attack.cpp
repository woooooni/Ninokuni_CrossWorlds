#include "stdafx.h"
#include "NpcDMWState_VulcanGun_Attack.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_VulcanGun_Attack::CNpcDMWState_VulcanGun_Attack(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_VulcanGun_Attack::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcDMWState_VulcanGun_Attack::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
	m_pWitch->Set_IsFollowing(true);

	m_bIsAttack = false;
	m_iAtkCount = 0;
}

void CNpcDMWState_VulcanGun_Attack::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pStellia != nullptr)
	{
		if (m_pStellia->Get_Component_Model()->Get_CurrAnimationFrame() > 45)
		{
			m_pStellia->Get_Component_Model()->Set_Stop_Animation(true);
		}
	}

	// 15프레임 마다
	if (!m_bIsAttack && m_pModelCom->Get_CurrAnimationFrame() == 40 + (m_iAtkCount * 15))
	{
		Vec4 vBulletPos = m_pTransformCom->Get_Position();
		Vec3 vBulletLook = XMVector3Rotate(m_pTransformCom->Get_Look(), XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(m_iAtkCount * -30.f), 0.0f));

		CGameObject* pBullet = nullptr;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Witch_VulcanBullet", m_pStellia, &pBullet)))
		{
			MSG_BOX("Add VulcanBullet Failed.");
			return;
		}

		CTransform* pBulletTransform = pBullet->Get_Component_Transform();

		pBulletTransform->Set_Position(vBulletPos);
		pBulletTransform->Set_Look(vBulletLook);

		m_iAtkCount += 1;
		m_bIsAttack = true;
	}
	// 프레임 당 한 번만 공격하도록
	else if(m_pModelCom->Get_CurrAnimationFrame() != 40 + (m_iAtkCount * 15))
		m_bIsAttack = false;


	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING);
		if (m_pStellia != nullptr)
		{
			m_pStellia->Get_Component_Model()->Set_Stop_Animation(false);
		}
	}
}

void CNpcDMWState_VulcanGun_Attack::Exit_State()
{
}

CNpcDMWState_VulcanGun_Attack* CNpcDMWState_VulcanGun_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_VulcanGun_Attack* pInstance = new CNpcDMWState_VulcanGun_Attack(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_VulcanGun_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_VulcanGun_Attack::Free()
{
	__super::Free();
}

