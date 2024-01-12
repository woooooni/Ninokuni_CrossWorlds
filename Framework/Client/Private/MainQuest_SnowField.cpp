#include "stdafx.h"
#include "MainQuest_SnowField.h"

#include "BTNode_Select.h"

CMainQuest_SnowField::CMainQuest_SnowField()
{
}

HRESULT CMainQuest_SnowField::Initialize()
{
	__super::Initialize();

	/* 그렴 여기서도 노드로 하는게 아니라 그냥 배열로 가지고 bool로 처리를 해야한다.*/

	m_pRootNode = CBTNode_Select::Create();

	return S_OK;
}

_bool CMainQuest_SnowField::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest_SnowField::LateTick(const _float& fTimeDelta)
{
}

CMainQuest_SnowField* CMainQuest_SnowField::Create()
{
	CMainQuest_SnowField* pInstance = new CMainQuest_SnowField();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest_SnowField");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest_SnowField::Free()
{
	__super::Free();
}

