#include "stdafx.h"
#include "UIMinigame_Manager.h"
#include "GameInstance.h"
#include "Transform.h"

#include "UI_Manager.h"

#include "UI_Minigame_TowerSelect.h"
#include "UI_Minigame_Timer.h"

IMPLEMENT_SINGLETON(CUIMinigame_Manager)

CUIMinigame_Manager::CUIMinigame_Manager()
{

}

HRESULT CUIMinigame_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	
	return S_OK;
}

void CUIMinigame_Manager::Tick(_float fTimeDelta)
{

}

HRESULT CUIMinigame_Manager::Ready_MinigameUI_Prototypes(LEVELID eID)
{
	switch (eID)
	{
	case LEVELID::LEVEL_EVERMORE:
		Ready_MinigameUI_Evermore();
		break;

	case LEVELID::LEVEL_KINGDOMHALL:
		break;

	case LEVELID::LEVEL_ICELAND:
		break;

	case LEVELID::LEVEL_WITCHFOREST:
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CUIMinigame_Manager::Ready_MinigameUI_GameObject(LEVELID eID)
{
	switch (eID)
	{
	case LEVELID::LEVEL_EVERMORE:
		Ready_TowerDence();
		break;

	case LEVELID::LEVEL_KINGDOMHALL:
		break;

	case LEVELID::LEVEL_ICELAND:
		break;

	case LEVELID::LEVEL_WITCHFOREST:
		break;

	default:
		break;
	}
	return S_OK;
}

void CUIMinigame_Manager::OnOff_TowerDefence_Select(_bool bOnOff)
{
	if (true == bOnOff)
	{
		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

		for (auto& pButton : m_TowerSelect)
		{
			if (nullptr != pButton)
				pButton->Set_Active(true);
		}
		 
		if (nullptr != m_pTimer)
			m_pTimer->Set_Active(true);
	}
	else
	{
		for (auto& pButton : m_TowerSelect)
		{
			if (nullptr != pButton)
				pButton->Set_Active(false);
		}

		if (nullptr != m_pTimer)
			m_pTimer->Set_Active(false);

		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
	}
}

HRESULT CUIMinigame_Manager::Ready_MinigameUI_Evermore()
{
	// 타워 디펜스용 UI
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Cannon"),
		CUI_Minigame_TowerSelect::Create(m_pDevice, m_pContext, CUI_Minigame_TowerSelect::UI_TOWERTYPE::TOWER_CANNON), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Crystal"),
		CUI_Minigame_TowerSelect::Create(m_pDevice, m_pContext, CUI_Minigame_TowerSelect::UI_TOWERTYPE::TOWER_CRYSTAL), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Flame"),
		CUI_Minigame_TowerSelect::Create(m_pDevice, m_pContext, CUI_Minigame_TowerSelect::UI_TOWERTYPE::TOWER_FLAME), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Shadow"),
		CUI_Minigame_TowerSelect::Create(m_pDevice, m_pContext, CUI_Minigame_TowerSelect::UI_TOWERTYPE::TOWER_SHADOW), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Timer"),
		CUI_Minigame_Timer::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIMinigame_Manager::Ready_TowerDence()
{
	m_TowerSelect.reserve(4);

	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 400.f * 0.36f;
	UIDesc.fCY = 400.f * 0.36f;
	UIDesc.fX = UIDesc.fCX * 0.8f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 1.5f);

	CGameObject* pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Cannon"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	UIDesc.fX = UIDesc.fCX * 1.3f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.7f);

	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Crystal"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	UIDesc.fX = UIDesc.fCX * 1.8f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 1.5f);

	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Flame"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	UIDesc.fX = UIDesc.fCX * 2.3f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.7f);

	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Shadow"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 256.f * 0.7f;
	UIDesc.fCY = 120.f * 0.7f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = UIDesc.fCY * 0.6f;

	CGameObject* pTimer = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Timer"), &UIDesc, &pTimer)))
		return E_FAIL;
	m_pTimer = dynamic_cast<CUI_Minigame_Timer*>(pTimer);
	if (nullptr == m_pTimer)
		return E_FAIL;
	Safe_AddRef(m_pTimer);

	return S_OK;
}

void CUIMinigame_Manager::Free()
{
	__super::Free();

	for (auto& pButton : m_TowerSelect)
		Safe_Release(pButton);
	m_TowerSelect.clear();

	Safe_Release(m_pTimer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
