#include "stdafx.h"
#include "UIMinigame_Manager.h"
#include "GameInstance.h"
#include "Transform.h"

#include "UI_Manager.h"

#include "UI_Minigame_Basic.h"
#include "UI_Minigame_TowerSelect.h"
#include "UI_Minigame_Timer.h"

#include "UI_Minigame_EnemyInfo.h"
#include "UI_Minigame_ClassSkill.h"
#include "UI_Minigame_EnemyHP.h"

IMPLEMENT_SINGLETON(CUIMinigame_Manager)

CUIMinigame_Manager::CUIMinigame_Manager()
{

}

void CUIMinigame_Manager::Set_HPOwner(CGameObject* pOwner, GRANDPRIX_ENEMY eEnemyID)
{
	if (GRANDPRIX_ENEMY::ENEMY_END == eEnemyID)
		return;
	
	if (m_EnemyHP[eEnemyID] == nullptr)
		return;
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
		Ready_Granprix();
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

HRESULT CUIMinigame_Manager::Ready_MinigameUI_ToLayer(LEVELID eID)
{
	if (LEVELID::LEVEL_EVERMORE == eID)
	{
		if (nullptr == m_pMenu)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pMenu)))
			return E_FAIL;
		Safe_AddRef(m_pMenu);

		for (auto& iter : m_TowerSelect)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		if (nullptr == m_pTimer)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pTimer)))
			return E_FAIL;
		Safe_AddRef(m_pTimer);

		if (nullptr == m_pStartBtn)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pStartBtn)))
			return E_FAIL;
		Safe_AddRef(m_pStartBtn);

		if (nullptr == m_pGold)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pGold)))
			return E_FAIL;
		Safe_AddRef(m_pGold);

		if (nullptr == m_pCloud)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pCloud)))
			return E_FAIL;
		Safe_AddRef(m_pCloud);

		for (auto& iter : m_EnemyHP)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

		for (auto& iter : m_Skill)
		{
			if (nullptr == iter)
				return E_FAIL;

			if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
				return E_FAIL;
			Safe_AddRef(iter);
		}

	}

	return S_OK;
}

void CUIMinigame_Manager::OnOff_TowerDefence_Select(_bool bOnOff)
{
	if (true == bOnOff)
	{
		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);

		if (nullptr != m_pMenu)
			m_pMenu->Set_Active(true);

		for (auto& pButton : m_TowerSelect)
		{
			if (nullptr != pButton)
				pButton->Set_Active(true);
		}
		 
		if (nullptr != m_pTimer)
			m_pTimer->Set_Active(true);

		if (nullptr != m_pStartBtn)
			m_pStartBtn->Set_Active(true);

		if (nullptr != m_pGold)
			m_pGold->Set_Active(true);
	}
	else
	{
		if (nullptr != m_pMenu)
			m_pMenu->Set_Active(false);

		for (auto& pButton : m_TowerSelect)
		{
			if (nullptr != pButton)
				pButton->Set_Active(false);
		}

		if (nullptr != m_pTimer)
			m_pTimer->Set_Active(false);

		if (nullptr != m_pStartBtn)
			m_pStartBtn->Set_Active(false);

		if (nullptr != m_pGold)
			m_pGold->Set_Active(false);

		CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
	}
}

void CUIMinigame_Manager::OnOff_Granprix(_bool bOnOff)
{
	if (true == bOnOff)
	{
		if (nullptr != m_pCloud)
			m_pCloud->Set_Active(true);

		for (auto& pHP : m_EnemyHP)
		{
			if (nullptr != pHP)
				pHP->Set_Active(true);
		}

		for (auto& pSkill : m_Skill)
		{
			if (nullptr != pSkill)
				pSkill->Set_Active(true);
		}
	}
	else
	{
		if (nullptr != m_pCloud)
			m_pCloud->Set_Active(false);

		for (auto& pHP : m_EnemyHP)
		{
			if (nullptr != pHP)
				pHP->Set_Active(false);
		}

		for (auto& pSkill : m_Skill)
		{
			if (nullptr != pSkill)
				pSkill->Set_Active(false);
		}
	}
}

HRESULT CUIMinigame_Manager::Ready_MinigameUI_Evermore()
{
	// Å¸¿ö µðÆæ½º¿ë UI
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Menu_Background"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::TOWERDEFENCE_MENU), LAYER_UI)))
		return E_FAIL;

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
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_StartButton"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::TOWERDEFENCE_START), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Gold"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::TOWERDEFENCE_GOLD), LAYER_UI)))
		return E_FAIL;


	// ±×¶ûÇÁ¸®¿ë UI
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_CloudFrame"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_FRAME), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"),
		CUI_Minigame_EnemyInfo::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_First"),
		CUI_Minigame_ClassSkill::Create(m_pDevice, m_pContext, CUI_Minigame_ClassSkill::UI_GRANDPRIXSKILL::SKILL_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_Second"),
		CUI_Minigame_ClassSkill::Create(m_pDevice, m_pContext, CUI_Minigame_ClassSkill::UI_GRANDPRIXSKILL::SKILL_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_Third"),
		CUI_Minigame_ClassSkill::Create(m_pDevice, m_pContext, CUI_Minigame_ClassSkill::UI_GRANDPRIXSKILL::SKILL_THIRD), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_SpecialSkill"),
		CUI_Minigame_ClassSkill::Create(m_pDevice, m_pContext, CUI_Minigame_ClassSkill::UI_GRANDPRIXSKILL::SKILL_SPECIAL), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkillFrame"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_CLASSFRAME), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_SpecialSkillFrame"),
		CUI_Minigame_Basic::Create(m_pDevice, m_pContext, CUI_Minigame_Basic::UI_MINIGAMEBASIC::GRANDPRIX_SPECIALFRAME), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBar"),
		CUI_Minigame_EnemyHP::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CUIMinigame_Manager::Ready_TowerDence()
{
	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 281.f;
	UIDesc.fCY = 900.f;
	UIDesc.fX = UIDesc.fCX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	CGameObject* pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Menu_Background"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pMenu = dynamic_cast<CUI_Minigame_Basic*>(pBackground);
	if (nullptr == m_pMenu)
		return E_FAIL;
	Safe_AddRef(m_pMenu);

	m_TowerSelect.reserve(4);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	_float fOffsetY = 180.f;

	UIDesc.fCX = 176.f;
	UIDesc.fCY = 163.f;
	UIDesc.fX = 140.5f;
	UIDesc.fY = 50.f + fOffsetY;

	CGameObject* pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Cannon"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	UIDesc.fY = 50.f + fOffsetY * 2.f;
	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Crystal"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	UIDesc.fY = 50.f + fOffsetY * 3.f;
	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_TowerSelect_Flame"), &UIDesc, &pButton)))
		return E_FAIL;
	m_TowerSelect.push_back(dynamic_cast<CUI_Minigame_TowerSelect*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;
	Safe_AddRef(pButton);


	UIDesc.fY = 50.f + fOffsetY * 4.f;
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


	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 256.f * 0.7f;
	UIDesc.fCY = 128.f * 0.7f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 20.f;
	UIDesc.fY = g_iWinSizeY - UIDesc.fCY * 0.5f - 20.f;

	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_StartButton"), &UIDesc, &pButton)))
		return E_FAIL;
	m_pStartBtn = dynamic_cast<CUI_Minigame_Basic*>(pButton);
	if (nullptr == m_pStartBtn)
		return E_FAIL;
	Safe_AddRef(m_pStartBtn);


	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 133.f;
	UIDesc.fCY = 35.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 20.f;
	UIDesc.fY = UIDesc.fCY * 0.5f + 20.f;

	CGameObject* pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_TowerDefence_Gold"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_pGold = dynamic_cast<CUI_Minigame_Basic*>(pSlot);
	if (nullptr == m_pGold)
		return E_FAIL;
	Safe_AddRef(m_pGold);

	return S_OK;
}

HRESULT CUIMinigame_Manager::Ready_Granprix()
{
	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	CGameObject* pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_CloudFrame"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_pCloud = dynamic_cast<CUI_Minigame_Basic*>(pFrame);
	if (nullptr == m_pCloud)
		return E_FAIL;
	Safe_AddRef(m_pCloud);


	m_EnemyHP.reserve(9);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	_float2 vOffset = _float2(30.f, 0.f);
	UIDesc.fCX = 178.f;
	UIDesc.fCY = 64.f;
	UIDesc.fX = UIDesc.fCX * 0.5f + vOffset.x;
	UIDesc.fY = 160.f;
	CGameObject* pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(0);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(1);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(2);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(3);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(4);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(5);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(6);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(7);
	Safe_AddRef(pHP);

	UIDesc.fY += (UIDesc.fCY + vOffset.y);
	pHP = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_HPBackground"), &UIDesc, &pHP)))
		return E_FAIL;
	m_EnemyHP.push_back(dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP));
	if (nullptr == pHP)
		return E_FAIL;
	dynamic_cast<CUI_Minigame_EnemyInfo*>(pHP)->Set_TextureIndex(8);
	Safe_AddRef(pHP);


	m_Skill.reserve(4);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	vOffset = _float2(30.f, 30.f);

	UIDesc.fCX = 256.f * 0.3f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = UIDesc.fCX * 0.5f + vOffset.x;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f) - vOffset.y;
	CGameObject* pSkill = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_First"), &UIDesc, &pSkill)))
		return E_FAIL;
	m_Skill.push_back(dynamic_cast<CUI_Minigame_ClassSkill*>(pSkill));
	if (nullptr == pSkill)
		return E_FAIL;
	Safe_AddRef(pSkill);

	UIDesc.fX += UIDesc.fCX + vOffset.x;
	pSkill = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_Second"), &UIDesc, &pSkill)))
		return E_FAIL;
	m_Skill.push_back(dynamic_cast<CUI_Minigame_ClassSkill*>(pSkill));
	if (nullptr == pSkill)
		return E_FAIL;
	Safe_AddRef(pSkill);

	UIDesc.fX += UIDesc.fCX + vOffset.x;
	pSkill = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_ClassSkill_Third"), &UIDesc, &pSkill)))
		return E_FAIL;
	m_Skill.push_back(dynamic_cast<CUI_Minigame_ClassSkill*>(pSkill));
	if (nullptr == pSkill)
		return E_FAIL;
	Safe_AddRef(pSkill);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 256.f * 0.45f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f + vOffset.x);
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f) - vOffset.y;
	pSkill = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Minigame_Granprix_SpecialSkill"), &UIDesc, &pSkill)))
		return E_FAIL;
	m_Skill.push_back(dynamic_cast<CUI_Minigame_ClassSkill*>(pSkill));
	if (nullptr == pSkill)
		return E_FAIL;
	Safe_AddRef(pSkill);

	return S_OK;
}

void CUIMinigame_Manager::Free()
{
	__super::Free();

	// Å¸¿öµðÆæ½º
	Safe_Release(m_pMenu);
	for (auto& pButton : m_TowerSelect)
		Safe_Release(pButton);
	m_TowerSelect.clear();
	Safe_Release(m_pTimer);
	Safe_Release(m_pStartBtn);
	Safe_Release(m_pGold);

	// ±×¶ûÇÁ¸®
	Safe_Release(m_pCloud);
	for (auto& pBackground : m_EnemyHP)
		Safe_Release(pBackground);
	m_EnemyHP.clear();
	for (auto& pSkill : m_Skill)
		Safe_Release(pSkill);
	m_Skill.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
