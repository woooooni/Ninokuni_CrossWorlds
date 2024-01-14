#include "stdafx.h"
#include "GlanixState_Rage2Idle.h"

#include "GameInstance.h"
#include "Glanix.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

CGlanixState_Rage2Idle::CGlanixState_Rage2Idle(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage2Idle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iMaxCount = 3;

	return S_OK;
}

void CGlanixState_Rage2Idle::Enter_State(void* pArg)
{
	if (pArg != nullptr)
		m_iWaveCount = *(_int*)pArg;

	if (m_iWaveCount < m_iMaxCount)
		m_fWaitTime = 2.f;
	else
	{
		m_iWaveCount = 0;
		m_fWaitTime = 4.f;
	}

	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));
	m_fTime = 0.f;
}

void CGlanixState_Rage2Idle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	// 여기서 슬로우 처리.
	//m_pGlanix->Get_SlowStack();

	m_fTime += fTimeDelta;

	if (m_fTime >= m_fWaitTime)
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2WAVE);

	m_fAccIcicleGen += fTimeDelta;
	if (m_fAccIcicleGen >= m_fGenTime)
	{
		m_fAccIcicleGen = 0.f;
		Generate_Icicle(1);
	}

}

void CGlanixState_Rage2Idle::Exit_State()
{
}

void CGlanixState_Rage2Idle::Generate_Icicle(_uint iCount)
{
	CTransform* pTransform = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
	{
		MSG_BOX("Transform Get Failed.");
		return;
	}


	for (_uint i = 0; i < iCount; ++i)
	{
		Vec4 vInitPos = pTransform->Get_Position();
		vInitPos.x += GI->RandomFloat(-5.f, 5.f);
		vInitPos.y += 10.f + GI->RandomFloat(0.f, 5.f);
		vInitPos.z += GI->RandomFloat(-5.f, 5.f);

		CGameObject* pIcicle = nullptr;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Glanix_GlanixIcicle", m_pGlanix, &pIcicle)))
		{
			MSG_BOX("Add Icicle Failed.");
			return;
		}

		CTransform* pIcicleTransform = pIcicle->Get_Component<CTransform>(L"Com_Transform");
		CPhysX_Controller* pPhysXController = pIcicle->Get_Component<CPhysX_Controller>(L"Com_Controller");

		pIcicleTransform->Set_State(CTransform::STATE_POSITION, vInitPos);
		pPhysXController->Set_EnterLevel_Position(vInitPos);
	}
}

CGlanixState_Rage2Idle* CGlanixState_Rage2Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage2Idle* pInstance = new CGlanixState_Rage2Idle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage2Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage2Idle::Free()
{
	__super::Free();
}

