#include "stdafx.h"
#include "MainQuestNode_FinalBattle06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Game_Manager.h"
#include "Player.h"

CMainQuestNode_FinalBattle06::CMainQuestNode_FinalBattle06()
{
}

HRESULT CMainQuestNode_FinalBattle06::Initialize()
{
	__super::Initialize();

	return S_OK;
}

void CMainQuestNode_FinalBattle06::Start()
{
	m_pWitch = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("DreamMazeWitch"));

	if (m_pWitch != nullptr)
	{
		// 마녀 컷신 애니메이션 시작
		m_pWitch->Get_Component_Model()->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_BossSkillRage01"));
		
		// 마녀 컷신 카메라 연출 시작
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		if (nullptr != pActionCam)
			pActionCam->Start_Action_Witch_Roar();
	}
}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle06::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
	{
		if (m_pWitch != nullptr)
		{
			if (m_pWitch->Get_Component_Model()->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_BossSkillRage01"))
			{
				// 마녀 카메라 연출 시그널
				if(!m_bCamSignal && m_pWitch->Get_Component_Model()->Get_CurrAnimationFrame() == 70)
				{
					m_bCamSignal = true;
					CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
					if(nullptr != pActionCam)
						pActionCam->Start_Action_Witch_Roar();

					// 레디얼 블러 활성화
					CGame_Manager::GetInstance()->Set_RadialBlur(true, 16.f, 0.05f);
				}

				if(m_pWitch->Get_Component_Model()->Get_CurrAnimationFrame() == 100)
				{
					if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
					{
						// 레디얼 블러 비활성화
						CGame_Manager::GetInstance()->Set_RadialBlur(false);

						m_bIsFadeOut = true;
						CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 1.f);
					}
				}
			}
		}
		
		if (m_bIsFadeOut && CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			m_bIsClear = true;
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 1.f);

			return NODE_STATE::NODE_FAIL;
		}
	}


	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle06::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_FinalBattle06* CMainQuestNode_FinalBattle06::Create()
{
	CMainQuestNode_FinalBattle06* pInstance = new CMainQuestNode_FinalBattle06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle06::Free()
{
	__super::Free();
}
