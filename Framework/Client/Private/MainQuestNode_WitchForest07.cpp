#include "stdafx.h"
#include "MainQuestNode_WitchForest07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Quest_Manager.h"

#include "GameNpc.h"

CMainQuestNode_WitchForest07::CMainQuestNode_WitchForest07()
{
}

HRESULT CMainQuestNode_WitchForest07::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/06.MainQuest_WitchForest/MainQuest_WitchForest05_2.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_WitchForest07::Start()
{
}

CBTNode::NODE_STATE CMainQuestNode_WitchForest07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// 혹시 나중에 연출 들어갈 수도 있으니 비워둔다.
	m_bIsClear = true;

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_WitchForest07::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_WitchForest07* CMainQuestNode_WitchForest07::Create()
{
	CMainQuestNode_WitchForest07* pInstance = new CMainQuestNode_WitchForest07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_WitchForest07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_WitchForest07::Free()
{
	__super::Free();
}
