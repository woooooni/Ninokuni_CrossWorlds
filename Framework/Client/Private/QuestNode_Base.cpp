#include "stdafx.h"
#include "QuestNode_Base.h"
#include "..\Public\QuestNode_Base.h"

#include "GameInstance.h"

CQuestNode_Base::CQuestNode_Base()
{
}

HRESULT CQuestNode_Base::Initialize()
{
	return S_OK;
}

void CQuestNode_Base::Start()
{
}

CBTNode::NODE_STATE CQuestNode_Base::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

void CQuestNode_Base::LateTick(const _float& fTimeDelta)
{
}

Vec4 CQuestNode_Base::Set_DestSpot(CGameObject* pGameObject)
{
	CTransform* pTransform = pGameObject->Get_Component<CTransform>(TEXT("Com_Transform"));
	Vec4 vSpotPos = pTransform->Get_Position() + pTransform->Get_Look() * 2.5f;

	return vSpotPos;
}

void CQuestNode_Base::Delete_QuestDestSpot()
{
	Safe_Release(m_pQuestDestSpot);
}

void CQuestNode_Base::Free()
{
	__super::Free();
	Safe_Release(m_pQuestDestSpot);

	Safe_Delete_Array(m_szpOwner);
	Safe_Delete_Array(m_szpTalk);
}

