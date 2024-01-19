#include "stdafx.h"
#include "UI_InGame_Setting_RadioGroup.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_InGame_Setting_RadioBtn.h"

CUI_InGame_Setting_RadioGroup::CUI_InGame_Setting_RadioGroup(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_RADIOGROUP eType)
	: CUI(pDevice, pContext, L"UI_InGame_Setting_RadioGroup")
	, m_eGroupType(eType)
{
}

CUI_InGame_Setting_RadioGroup::CUI_InGame_Setting_RadioGroup(const CUI_InGame_Setting_RadioGroup& rhs)
	: CUI(rhs)
	, m_eGroupType(rhs.m_eGroupType)
{
}

HRESULT CUI_InGame_Setting_RadioGroup::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_RadioGroup::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Ready_RadioButtons();

	m_fAlpha = 1.f;
	m_bUseMouse = true;

	return S_OK;
}

void CUI_InGame_Setting_RadioGroup::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != m_pOnBtn)
			m_pOnBtn->Tick(fTimeDelta);
		if (nullptr != m_pOffBtn)
			m_pOffBtn->Tick(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_InGame_Setting_RadioGroup::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != m_pOnBtn)
			m_pOnBtn->LateTick(fTimeDelta);
		if (nullptr != m_pOffBtn)
			m_pOffBtn->LateTick(fTimeDelta);
	}
}

HRESULT CUI_InGame_Setting_RadioGroup::Render()
{
	if (m_bActive)
	{
		if (nullptr != m_pOnBtn)
			m_pOnBtn->Render();
		if (nullptr != m_pOffBtn)
			m_pOffBtn->Render();
	}

	return S_OK;
}

HRESULT CUI_InGame_Setting_RadioGroup::Ready_Components()
{
	// Transform Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

void CUI_InGame_Setting_RadioGroup::Ready_RadioButtons()
{
	CUI::UI_INFO UIDesc = {};
	UIDesc.fCX = 64.f * 0.5f;
	UIDesc.fCY = UIDesc.fCX;
	CGameObject* pButton = nullptr;

	switch (m_eGroupType)
	{
	case GRAPHIC_NATURAL:
		UIDesc.fX = 1300.f;
		UIDesc.fY = 305.f;

		pButton = nullptr;
		pButton = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton"), LAYER_TYPE::LAYER_UI, &UIDesc);
		if (nullptr == pButton)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton))
			return;
		m_pOnBtn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton);

		UIDesc.fX = 1450.f;
		pButton = nullptr;
		pButton = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton"), LAYER_TYPE::LAYER_UI, &UIDesc);
		if (nullptr == pButton)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton))
			return;
		m_pOffBtn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton);
		break;

	case GRAPHIC_SHADOW:
		UIDesc.fX = 1300.f;
		UIDesc.fY = 350.f;

		pButton = nullptr;
		pButton = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton"), LAYER_TYPE::LAYER_UI, &UIDesc);
		if (nullptr == pButton)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton))
			return;
		m_pOnBtn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton);

		UIDesc.fX = 1450.f;
		pButton = nullptr;
		pButton = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton"), LAYER_TYPE::LAYER_UI, &UIDesc);
		if (nullptr == pButton)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton))
			return;
		m_pOffBtn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton);
		break;
	
	case GRAPHIC_OUTLINE:
		UIDesc.fX = 1300.f;
		UIDesc.fY = 395.f;

		pButton = nullptr;
		pButton = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton"), LAYER_TYPE::LAYER_UI, &UIDesc);
		if (nullptr == pButton)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton))
			return;
		m_pOnBtn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton);

		UIDesc.fX = 1450.f;
		pButton = nullptr;
		pButton = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton"), LAYER_TYPE::LAYER_UI, &UIDesc);
		if (nullptr == pButton)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton))
			return;
		m_pOffBtn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pButton);
		break;
		// 그룹 없애야함
	default:
		break;
	}
}

CUI_InGame_Setting_RadioGroup* CUI_InGame_Setting_RadioGroup::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_RADIOGROUP eType)
{
	CUI_InGame_Setting_RadioGroup* pInstance = new CUI_InGame_Setting_RadioGroup(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_InGame_Setting_RadioGroup");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InGame_Setting_RadioGroup::Clone(void* pArg)
{
	CUI_InGame_Setting_RadioGroup* pInstance = new CUI_InGame_Setting_RadioGroup(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_InGame_Setting_RadioGroup");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InGame_Setting_RadioGroup::Free()
{
	Safe_Release(m_pOnBtn);
	Safe_Release(m_pOffBtn);

	Safe_Release(m_pTransformCom);
}
