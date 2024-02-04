#include "stdafx.h"
#include "MainQuestNode_FinalBattle05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "DreamMazeWitch_Npc.h"

CMainQuestNode_FinalBattle05::CMainQuestNode_FinalBattle05()
{
}

HRESULT CMainQuestNode_FinalBattle05::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/08.MainQuest_FinalBattle/MainQuest_FinalBattle05.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_FinalBattle05::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pWitch = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("DreamMazeWitch"));

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pWitch);

	/* ��ȭ ī�޶� ���� */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pWitch);

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN))
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

			/* ��ȭ ī�޶� ���� */
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
				pActionCam->Finish_Action_Talk();

			m_bIsClear = true;
			return NODE_STATE::NODE_FAIL;
		}

		// ������ ������ �ֱ⿡ ������ ���� �ɱ�
		if (m_iTalkIndex < m_vecTalkDesc.size())
		{
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle05::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_FinalBattle05::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_01_ArenSay_Hmm.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Aren.ao|Aren_Idle02"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_02_02_KuuSay_Introduce.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pWitch->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pWitch->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	}

}

CMainQuestNode_FinalBattle05* CMainQuestNode_FinalBattle05::Create()
{
	CMainQuestNode_FinalBattle05* pInstance = new CMainQuestNode_FinalBattle05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle05::Free()
{
	__super::Free();
}