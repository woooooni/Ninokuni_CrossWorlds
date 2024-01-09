#include "stdafx.h"
#include "MainQuestNode01_01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Kuu.h"

CMainQuestNode01_01::CMainQuestNode01_01()
{
}

HRESULT CMainQuestNode01_01::Initialize()
{
	__super::Initialize();

	//Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest01/MainQuest01_01.json");
	//
	//for (const auto& talkDesc : Load) {
	//	TALK_DELS sTalkDesc;
	//	sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
	//	sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
	//	m_vecTalkDesc.push_back(sTalkDesc);
	//}

	return S_OK;
}

void CMainQuestNode01_01::Start()
{
	m_pKuu = dynamic_cast<CKuu*>(GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu")));

	Vec4 vSpotPos = m_pKuu->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_Position() +
		m_pKuu->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_Look() * 1.5f;
	// Vec4 vSpotPos = { 5.f, 2.5f, 10.f, 1.f };

	// 임시로 monster에 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CMainQuestNode01_01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// 콜라이더 생기는지 확인하자. Tick을 안돌려줘서 안돌았다 여태.
	m_pQuestDestSpot->Tick(fTimeDelta);
	m_pQuestDestSpot->LateTick(fTimeDelta);

	if (m_pQuestDestSpot != nullptr)
	{
		if (m_pQuestDestSpot->Get_IsCol())
		{
			// 이거 왜 바로 걸림?
			m_bIsClear = true;
			m_pQuestDestSpot->Set_Dead(true);
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode01_01::LateTick(const _float& fTimeDelta)
{
	m_pQuestDestSpot->LateTick(fTimeDelta);
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
	Safe_Release(m_pQuestDestSpot);
}
