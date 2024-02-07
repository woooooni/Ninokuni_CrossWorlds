#include "stdafx.h"
#include "MainQuestNode_Invasion02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "UI_PopupQuest.h"

#include "Player.h"
#include "Character.h"
#include "SkyDome.h"
#include "Quest_Manager.h"

CMainQuestNode_Invasion02::CMainQuestNode_Invasion02()
{
}

HRESULT CMainQuestNode_Invasion02::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("성 밖의 소란");
	m_strQuestContent = TEXT("성 밖으로 나가보자");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("파괴된 에스타나비아");
	m_strNextQuestContent = TEXT("좀 더 나아가서 상황을 살펴보자");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/05.MainQuest_Invasion/MainQuest_Invasion02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Invasion02::Start()
{
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);

	/* 현재 퀘스트에 연관있는 객체들 */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

}

CBTNode::NODE_STATE CMainQuestNode_Invasion02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_EVERMORE)
	{
		if (false == m_bBGMStart)
		{

			list<CGameObject*>& SkyDomes = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_SKYBOX);
			for (auto& pSkyDome : SkyDomes)
			{
				if (wstring::npos != pSkyDome->Get_PrototypeTag().find(L"Skydome"))
				{
					CSkyDome* pCastSky = dynamic_cast<CSkyDome*>(pSkyDome);
					if (nullptr != pCastSky)
					{
						CQuest_Manager::GetInstance()->Set_OriginSkyCenterColor(pCastSky->Get_CenterColor());
						CQuest_Manager::GetInstance()->Set_OriginSkyApexColor(pCastSky->Get_ApexColor());

						pCastSky->Set_CenterColor(Vec4(0.671f, 0.059f, 0.0f, 1.0f));
						pCastSky->Set_ApexColor(Vec4(0.329f, 0.173f, 0.157f, 1.0f));
					}
				}
			}
			CQuest_Manager::GetInstance()->Ready_InvasionLight(TEXT("Evermore Light"));

			CRenderer::FOG_DESC desc;
			::ZeroMemory(&desc, sizeof(desc));
			{
				desc.fFogDistanceValue = 30.0f;
				desc.fFogHeightValue = 50.0f;
				desc.fFogStartDepth = 100.0f;
				desc.fFogStartDistance = 0.04f;
				//desc.fFogHeightDensity = 0.110f;
				desc.fFogDistanceDensity = 0.110f;
			}
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_FogDesc(desc);
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_FogColor(Vec4(0.671f,0.059f,0.0f,1.0f));

			// Fog
			// Fog Color : r 0.671, g : 0.059, b : 0.0, a : 1.0f
			// FogDistvalue  : 30.0f
			// FogHeight 50.0
			// Depth 100, FogStartDist = 0.04 , Density = 0.110f
			// Renderer로 던진다.

			// Light
			// Diffuse Ambient Upper 


			GI->Stop_Sound(CHANNELID::SOUND_BGM_CURR, 0.f);
			GI->Play_BGM(L"BGM_Field_Hunting_CastleInside_Dark_1.ogg", 1.f, true);
			m_bBGMStart = true;
		}

		if (!m_bIsStart)
		{
			if (CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_CurrentState() == CCharacter::STATE::NEUTRAL_DOOR_ENTER)
				return NODE_STATE::NODE_RUNNING;
			
			/* 대화 */
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();

			m_bIsStart = true;
		}

		if (KEY_TAP(KEY::LBTN))
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_PopupQuest::QUEST_INFO QuestDesc = {};
				QuestDesc.strType = m_strNextQuestTag;
				QuestDesc.strTitle = m_strNextQuestName;
				QuestDesc.strContents = m_strNextQuestContent;
				CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

				m_bIsClear = true;
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

				//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
				//if (nullptr != pActionCam)
				//	pActionCam->Finish_Action_Talk();

				return NODE_STATE::NODE_FAIL;
			}

			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Invasion02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Invasion02::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_00_KuuSay_Hmm....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		//m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 1:
		// CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_01_KuuSay_HHH.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_01_KuuSay_HHH.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		//m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		break;
	}
}

CMainQuestNode_Invasion02* CMainQuestNode_Invasion02::Create()
{
	CMainQuestNode_Invasion02* pInstance = new CMainQuestNode_Invasion02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Invasion02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Invasion02::Free()
{
	__super::Free();
}
