#include "stdafx.h"
#include "GlanixState_Rage2Wave.h"

#include "GameInstance.h"

#include "Glanix.h"
#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Glanix_ShockWave.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

CGlanixState_Rage2Wave::CGlanixState_Rage2Wave(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Rage2Wave::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iMaxCount = 3;

	return S_OK;
}

void CGlanixState_Rage2Wave::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Skill06")); 
	m_bIsCreateWave = false;
}

void CGlanixState_Rage2Wave::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	// 여기서 슬로우 처리.
	//m_pGlanix->Get_SlowStack();
	m_fAccIcicleGen += fTimeDelta;
	if (m_fAccIcicleGen >= m_fGenTime)
	{
		m_fAccIcicleGen = 0.f;
		Generate_Icicle(1);
	}

	if (!m_bIsCreateWave && m_pModelCom->Get_CurrAnimationFrame() == 50)
	{
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
		GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_ShockWave"), m_pGlanix);
		m_bIsCreateWave = true;
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		if (m_iWaveCount >= m_iMaxCount)
		{
			m_iWaveCount = 0;
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2RISING);
		}
		else
		{
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2IDLE, &m_iWaveCount);
			m_iWaveCount += 1;
		}
	}
}

void CGlanixState_Rage2Wave::Exit_State()
{
}

void CGlanixState_Rage2Wave::Generate_Icicle(_uint iCount)
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
		vInitPos.x += GI->RandomFloat(-1.f, 1.f);
		vInitPos.y += 20.f + GI->RandomFloat(0.f, 10.f);
		vInitPos.z += GI->RandomFloat(-1.f, 1.f);
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

CGlanixState_Rage2Wave* CGlanixState_Rage2Wave::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Rage2Wave* pInstance = new CGlanixState_Rage2Wave(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Rage2Wave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Rage2Wave::Free()
{
	__super::Free();
}

