#include "stdafx.h"
#include "..\Public\UI_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Effect.h"
#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"

#include "UI_Fade.h"
#include "UI_Cursor.h"
#include "UI_Basic.h"
#include "UI_MapName.h"
#include "UI_Loading_Background.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{

}

CUI_Fade* CUI_Manager::Get_Fade()
{
	if (nullptr != m_pUIFade)
		return m_pUIFade;
	else
		return nullptr;
}

HRESULT CUI_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	


	return S_OK;
}


void CUI_Manager::Tick(_float fTimeDelta)
{
	if (nullptr != m_pUICursor)
		m_pUICursor->Tick(fTimeDelta);
}

void CUI_Manager::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pUICursor)
	{
		m_pUICursor->LateTick(fTimeDelta);
		m_pUICursor->Render(); // Temp
	}
}

HRESULT CUI_Manager::Ready_UIPrototypes(LEVELID eID)
{
	switch (eID)
	{
	case LEVELID::LEVEL_STATIC:
		Ready_UIStaticPrototypes();
		break;

	case LEVELID::LEVEL_LOGO:
		break;

	case LEVELID::LEVEL_TEST: // Temp -> 이후 GamePlay
		Ready_UIGamePlayPrototypes();
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CUI_Manager::Ready_Cursor()
{
	CGameObject* pCursor = nullptr;

	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 50.f * 0.6f;
	UIDesc.fCY = 50.f * 0.6f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_STATIC, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Cursor"), &UIDesc, &pCursor)))
		return E_FAIL;

	m_pUICursor = dynamic_cast<CUI_Cursor*>(pCursor);

	return S_OK;
}

HRESULT CUI_Manager::Ready_Veils()
{
	CGameObject* pVeil = nullptr;

	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	
	//	Add_GameObject(_uint iLevelIndex, const _uint iLayerType, const wstring & strPrototypeTag,
	//					void* pArg = nullptr, __out class CGameObject** ppOut = nullptr);

	if(FAILED(GI->Add_GameObject(LEVELID::LEVEL_STATIC, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Fade_Black"), &UIDesc, &pVeil)))
		return E_FAIL;

	m_pUIFade = dynamic_cast<CUI_Fade*>(pVeil);

	return S_OK;
}

HRESULT CUI_Manager::Ready_LobbyUIs()
{
//	m_Basic.reserve(1);
//
//	CGameObject* pNameTag = nullptr;
//
//	CUI::UI_INFO UIDesc = {};
//	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
//	// 우측 상단에 있는 플레이어 NickName을 담을 Frame
//	UIDesc.fCX = g_iWinSizeX;
//	UIDesc.fCY = g_iWinSizeY;
//	UIDesc.fX = g_iWinSizeX * 0.5f;
//	UIDesc.fY = g_iWinSizeY * 0.5f;
//
//	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_STATIC, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Fade_Black"), &UIDesc, &pNameTag)))
//		return E_FAIL;
//
//	m_Basic.push_back(dynamic_cast<CUI_Basic*>(pNameTag));

	return S_OK;
}

HRESULT CUI_Manager::Ready_GamePlayUIs()
{
	// MapName 생성
	CGameObject* pMapName = nullptr;
	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 1200.f * 0.35f;
	UIDesc.fCY = 256.f * 0.35f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.15f;

	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_TEST, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MapName"), &UIDesc, &pMapName)))
		return E_FAIL;

	m_pUIMapName = dynamic_cast<CUI_MapName*>(pMapName);

	return S_OK;
}

HRESULT CUI_Manager::Tick_UIs(LEVELID eID, _float fTimeDelta)
{
	switch (eID)
	{
	case LEVELID::LEVEL_LOGO:
		Tick_LogoLevel(fTimeDelta);
		break;
	case LEVELID::LEVEL_TEST:
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CUI_Manager::Tick_LogoLevel(_float fTimeDelta)
{
	// Temp
//	if (nullptr != m_pUIFade)
//	{
//		m_pUIFade->Tick(fTimeDelta);
//		m_pUIFade->LateTick(fTimeDelta);
//		m_pUIFade->Render();
//	}
//
	return S_OK;
}

HRESULT CUI_Manager::Tick_LobbyLevel(_float fTimeDelta)
{
	// Temp
//	for (auto& pBasic : m_Basic)
//	{
//		if (nullptr != pBasic)
//			pBasic->Tick(fTimeDelta);
//	}
//
	return S_OK;
}

HRESULT CUI_Manager::Save_UIData()
{
	return S_OK;
}

void CUI_Manager::Load_UIData()
{
}


#pragma region Loading_Prototypes

HRESULT CUI_Manager::Ready_UIStaticPrototypes()
{
	// Add Prototypes

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Fade_Black"),
		CUI_Fade::Create(m_pDevice, m_pContext, CUI_Fade::UI_VEIL::VEIL_BLACK), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Cursor"),
		CUI_Cursor::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Manager::Ready_UIGamePlayPrototypes()
{
	// MapName Prototype
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MapName"),
		CUI_MapName::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion


void CUI_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pUICursor);
	Safe_Release(m_pUIFade);
	Safe_Release(m_pUIMapName);

	for (auto& pBasic : m_Basic)
		Safe_Release(pBasic);
	m_Basic.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
