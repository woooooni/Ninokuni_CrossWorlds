#include "stdafx.h"
#include "MainQuestNode01_02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode01_02::CMainQuestNode01_02()
{
}

HRESULT CMainQuestNode01_02::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest01/MainQuest01_02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	// string strLoad = Load.dump();

	return S_OK;
}

void CMainQuestNode01_02::Start()
{
	szOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	szTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(szOwner, szTalk);
}

CBTNode::NODE_STATE CMainQuestNode01_02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN))
	{
		Safe_Delete_Array(szOwner);
		Safe_Delete_Array(szTalk);

		m_iTalkIndex += 1;

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);
			return NODE_STATE::NODE_FAIL;
		}

		szOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		szTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(szOwner, szTalk);
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode01_02::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode01_02* CMainQuestNode01_02::Create()
{
	CMainQuestNode01_02* pInstance = new CMainQuestNode01_02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode01_02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode01_02::Free()
{
	__super::Free();
}
