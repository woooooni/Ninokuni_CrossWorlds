#include "stdafx.h"
#include "MainQuestNode_Invasion02_4.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "Camera_Group.h"

#include "DreamMazeWitch_Npc.h"

CMainQuestNode_Invasion02_4::CMainQuestNode_Invasion02_4()
{
}

HRESULT CMainQuestNode_Invasion02_4::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/05.MainQuest_Invasion/MainQuest_Invasion02_4.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Invasion02_4::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	/* 대화 카메라 세팅 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(nullptr);

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();

}

CBTNode::NODE_STATE CMainQuestNode_Invasion02_4::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN) && m_iTalkIndex == 0)
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;
		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_Dialogue_Page_1.mp3"), CHANNELID::SOUND_UI,
			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			m_bIsWitchAppear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

			/* 대화 카메라 종료 */
			//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			//if (nullptr != pActionCam)
			//	pActionCam->Finish_Action_Talk();
		}

		// 대화는 하나고 마녀의 연출이 필요하기에 여기서 강제로 조건 걸어주기.
		if (m_iTalkIndex < 1)
		{
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
		}
	}

	if (m_bIsWitchAppear)
	{
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		if (nullptr == pActionCam)
			return NODE_STATE::NODE_FAIL;

		// 마녀 등장해서 내려오기 시작
		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_NPC), TEXT("Prorotype_GameObject_DreamMazeWitch_Npc"), nullptr, &m_pWitch)))
			MSG_BOX("Fail AddGameObj : Quest DreamerMazeWitch");

		// 초기 높이 값 기억 및 데이터 세팅 
		pActionCam->Ready_Action_Witch_Invasion(m_pWitch);
		m_fWitchOriginHeight = Vec4(m_pWitch->Get_Component_Transform()->Get_Position()).y;

		// 대화 카메라 세팅을 위해 일시적으로 높이 낮춰주기
		{
			Vec4 vPos = m_pWitch->Get_Component_Transform()->Get_Position();
			vPos.y = m_fWitchDestHeight;
			m_pWitch->Get_Component_Transform()->Set_Position(vPos);
		}

		/* 대화 카메라 다시 세팅 */
		{
			pActionCam->Start_Action_Talk(m_pWitch);
		}

		// 플레이어가 카메라에 안나오게 내려주기 
		CCharacter* pCharacter = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
		if (nullptr != pCharacter)
		{
			Vec4 vPos = pCharacter->Get_Component_Transform()->Get_Position();
			vPos.y -= 3.f;
			pCharacter->Get_Component_Transform()->Set_Position(vPos);
		}

		// 다시 높이 높여주기 
		{
			Vec4 vPos = m_pWitch->Get_Component_Transform()->Get_Position();
			vPos.y = m_fWitchOriginHeight;
			m_pWitch->Get_Component_Transform()->Set_Position(vPos);
		}

		/* 내려오는 컷신 실행 */
		pActionCam->Start_Action_Witch_Invasion();

		m_pWitch->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_INVASION_APPEAR);

		m_bIsWitchAppear = false;
	}

	if (m_pWitch != nullptr)
	{
		// 마녀 다 내려옴 
		if (m_pWitch->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Get_CurrState() == CDreamMazeWitch_Npc::WITCHSTATE_INVASION_IDLE)
		{
			// 갔다와서 여기 바로 걸리는지 확인. 바로 걸리면 안되거덩

			// 다시 대화 캠으로 변경 (플레이어나 npc 먼저 대화 시작)
			m_bIsClear = true;
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Invasion02_4::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Invasion02_4::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Invasion_02_04_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* 대화 카메라 타겟 변경 */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	}
}

CMainQuestNode_Invasion02_4* CMainQuestNode_Invasion02_4::Create()
{
	CMainQuestNode_Invasion02_4* pInstance = new CMainQuestNode_Invasion02_4();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Invasion02_4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Invasion02_4::Free()
{
	__super::Free();
}
