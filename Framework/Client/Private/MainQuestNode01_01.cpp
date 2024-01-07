#include "stdafx.h"
#include "MainQuestNode01_01.h"

#include "GameInstance.h"
#include "Utils.h"

CMainQuestNode01_01::CMainQuestNode01_01()
{
}

HRESULT CMainQuestNode01_01::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest01/MainQuest01_01.json");
	TALK_DELS TalkDesc;

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		TalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		TalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	// string strLoad = Load.dump();

	return S_OK;
}

void CMainQuestNode01_01::Start()
{
}

CBTNode::NODE_STATE CMainQuestNode01_01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_iTalkIndex >= m_vecTalkDesc.size())
	{
		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	wstring temp = m_vecTalkDesc[m_iTalkIndex].strOwner;
	wstring temp2 = m_vecTalkDesc[m_iTalkIndex].strTalk;

	if (KEY_TAP(KEY::LBTN))
	{
		m_iTalkIndex += 1;
	}

	return NODE_STATE::NODE_RUNNING;
}

CMainQuestNode01_01* CMainQuestNode01_01::Create()
{
	CMainQuestNode01_01* pInstance = new CMainQuestNode01_01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode01_01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode01_01::Free()
{
	__super::Free();
}
