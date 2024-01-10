#include "stdafx.h"
#include "MainQuestNode01_03.h"

#include "GameInstance.h"
#include "Utils.h"

CMainQuestNode01_03::CMainQuestNode01_03()
{
}

HRESULT CMainQuestNode01_03::Initialize()
{
	__super::Initialize();

	m_szQuestName = L"에스타바니아 둘러보기.";
	m_szQuestContent = L"OO에게 가기.";

	return S_OK;
}

void CMainQuestNode01_03::Start()
{

}

CBTNode::NODE_STATE CMainQuestNode01_03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	///* 메인 퀘스트1의 마지막 단계이므로 success 반환. */
	//if (m_iTalkIndex >= m_vecTalkDesc.size())
	//{
	//	m_bIsClear = true;
	//	return NODE_STATE::NODE_SUCCESS;
	//}
	//
	//wstring temp = m_vecTalkDesc[m_iTalkIndex].strOwner;
	//wstring temp2 = m_vecTalkDesc[m_iTalkIndex].strTalk;
	//
	//if (KEY_TAP(KEY::LBTN))
	//{
	//	m_iTalkIndex += 1;
	//}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode01_03::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode01_03* CMainQuestNode01_03::Create()
{
	CMainQuestNode01_03* pInstance = new CMainQuestNode01_03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode01_03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode01_03::Free()
{
	__super::Free();
}
