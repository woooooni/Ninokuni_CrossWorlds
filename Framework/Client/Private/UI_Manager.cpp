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
#include "UI_LevelUp.h"
#include "UI_MapName.h"
#include "UI_BtnClose.h"
#include "UI_MainMenu.h"
#include "UI_BtnAccept.h"
#include "UI_PlayerInfo.h"
#include "UI_PopupQuest.h"
#include "UI_BasicButton.h"
#include "UI_WindowQuest.h"
#include "UI_BtnShowMenu.h"
#include "UI_BtnInventory.h"
#include "UI_BtnQuickQuest.h"
#include "UI_BtnChangeCamera.h"
#include "UI_Loading_Character.h"
#include "UI_SubMenu_Character.h"
#include "UI_BtnCharacterSelect.h"
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

_bool CUI_Manager::Get_MainMenuActive()
{
	return m_pMainBG->Get_Active();
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
		//Ready_UIGamePlayPrototypes();
		break;

	case LEVELID::LEVEL_LOBBY:
		Ready_UILobbyPrototypes();
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
	if (nullptr == m_pUICursor)
		return E_FAIL;

	Safe_AddRef(m_pUICursor);

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
	if (nullptr == m_pUIFade)
		return E_FAIL;

	Safe_AddRef(m_pUIFade);

	return S_OK;
}

HRESULT CUI_Manager::Ready_LobbyUIs()
{
	m_Basic.reserve(4);

	CGameObject* pNameTag = nullptr;

	CUI::UI_INFO UIDesc = {}; // 1
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	// 우측 상단에 있는 플레이어 NickName을 담을 Frame
	UIDesc.fCX = 512.f * 0.5f;
	UIDesc.fCY = 256.f * 0.45f;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f) - 40.f;
	UIDesc.fY = UIDesc.fCY * 0.5f + 15.f;

	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_NicknameFrame"), &UIDesc, &pNameTag)))
		return E_FAIL;

	m_Basic.push_back(dynamic_cast<CUI_Basic*>(pNameTag));
	if (nullptr == pNameTag)
		return E_FAIL;

	Safe_AddRef(pNameTag);


	CGameObject* pDeco = nullptr; // 2
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	// 상단 메뉴바 Deco
	UIDesc.fCX = 128.f * 0.8f;
	UIDesc.fCY = 64.f * 0.6f;
	UIDesc.fX = UIDesc.fCX * 0.5f;
	UIDesc.fY = UIDesc.fCY * 0.5f;

	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Common_MenuDeco_Left"), &UIDesc, &pDeco)))
		return E_FAIL;

	m_Basic.push_back(dynamic_cast<CUI_Basic*>(pDeco));
	if (nullptr == pDeco)
		return E_FAIL;

	Safe_AddRef(pDeco);


	pDeco = nullptr; // 3
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	// 상단 메뉴 Title
	_float2 vOffset = _float2(90.f, 10.f);

	UIDesc.fCX = 277.f * 0.7f;
	UIDesc.fCY = 93.f * 0.7f;
	UIDesc.fX = UIDesc.fCX * 0.5f + vOffset.x;
	UIDesc.fY = UIDesc.fCY * 0.5f + vOffset.y;

	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Title"), &UIDesc, &pDeco)))
		return E_FAIL;

	m_Basic.push_back(dynamic_cast<CUI_Basic*>(pDeco));
	if (nullptr == pDeco)
		return E_FAIL;

	Safe_AddRef(pDeco);



	m_Buttons.reserve(1);

	CGameObject* pButton = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	// 게임 시작 버튼
	UIDesc.fCX = 512.f * 0.6f;
	UIDesc.fCY = 128.f * 0.6f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 30.f;
	UIDesc.fY = g_iWinSizeY - 70.f;

	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Btn_Basic"), &UIDesc, &pButton)))
		return E_FAIL;

	m_Buttons.push_back(dynamic_cast<CUI_BasicButton*>(pButton));
	if (nullptr == pButton)
		return E_FAIL;

	Safe_AddRef(pButton);



	pDeco = nullptr; // 4
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	// GameStart Text
	UIDesc.fCX = 188.f * 0.7f;
	UIDesc.fCY = 53.f * 0.7f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 115.f;
	UIDesc.fY = g_iWinSizeY - 70.f;

	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_BtnText"), &UIDesc, &pDeco)))
		return E_FAIL;

	m_Basic.push_back(dynamic_cast<CUI_Basic*>(pDeco));
	if (nullptr == pDeco)
		return E_FAIL;

	Safe_AddRef(pDeco);


	// Buttons
	m_ClickedPlayer.reserve(5);
	m_UnclickedPlayer.reserve(5);
	vOffset = _float2(50.f, 90.f);

	CGameObject* pBtn = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 128.f * 0.6f;
	UIDesc.fCY = 128.f * 0.6f;
	UIDesc.fX = UIDesc.fCX * 0.5f + vOffset.x;
	UIDesc.fY = g_iWinSizeY * 0.32f;


	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Btn_Unclicked_Rogue"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_UnclickedPlayer.push_back(dynamic_cast<CUI_BtnCharacterSelect*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;

	Safe_AddRef(pBtn);


	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Btn_Clicked_Rogue"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_ClickedPlayer.push_back(dynamic_cast<CUI_BtnCharacterSelect*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;

	Safe_AddRef(pBtn);


	pBtn = nullptr;
	UIDesc.fY = UIDesc.fY + vOffset.y;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Btn_Unclicked_Destroyer"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_UnclickedPlayer.push_back(dynamic_cast<CUI_BtnCharacterSelect*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;

	Safe_AddRef(pBtn);


	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Btn_Clicked_Destroyer"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_ClickedPlayer.push_back(dynamic_cast<CUI_BtnCharacterSelect*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;

	Safe_AddRef(pBtn);


	pBtn = nullptr;
	UIDesc.fY = UIDesc.fY + vOffset.y;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Btn_Unclicked_Witch"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_UnclickedPlayer.push_back(dynamic_cast<CUI_BtnCharacterSelect*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;

	Safe_AddRef(pBtn);

	
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Btn_Clicked_Witch"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_ClickedPlayer.push_back(dynamic_cast<CUI_BtnCharacterSelect*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;

	Safe_AddRef(pBtn);


	pBtn = nullptr;
	UIDesc.fY = UIDesc.fY + vOffset.y;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Btn_Unclicked_Engineer"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_UnclickedPlayer.push_back(dynamic_cast<CUI_BtnCharacterSelect*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;

	Safe_AddRef(pBtn);


	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Btn_Clicked_Engineer"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_ClickedPlayer.push_back(dynamic_cast<CUI_BtnCharacterSelect*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;

	Safe_AddRef(pBtn);


	pBtn = nullptr;
	UIDesc.fY = UIDesc.fY + vOffset.y;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Btn_Unclicked_Swordman"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_UnclickedPlayer.push_back(dynamic_cast<CUI_BtnCharacterSelect*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;

	Safe_AddRef(pBtn);


	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Lobby_Btn_Clicked_Swordman"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_ClickedPlayer.push_back(dynamic_cast<CUI_BtnCharacterSelect*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;

	Safe_AddRef(pBtn);


	return S_OK;
}

HRESULT CUI_Manager::Ready_CommonUIs(LEVELID eID)
{
	// MapName 생성
	CGameObject* pMapName = nullptr;
	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 1200.f * 0.35f;
	UIDesc.fCY = 256.f * 0.35f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.15f + 30.f;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MapName"), &UIDesc, &pMapName)))
		return E_FAIL;

	m_pUIMapName = dynamic_cast<CUI_MapName*>(pMapName);
	if (nullptr == m_pUIMapName)
		return E_FAIL;

	Safe_AddRef(m_pUIMapName);


	// LevelUp 생성
	m_LevelUp.reserve(2);

	CGameObject* pLevelUp = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 1000.f * 0.5f;
	UIDesc.fCY = 481.f * 0.5f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.25f;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_LevelUp_Background"), &UIDesc, &pLevelUp)))
		return E_FAIL;

	m_LevelUp.push_back(dynamic_cast<CUI_LevelUp*>(pLevelUp));
	if (nullptr == pLevelUp)
		return E_FAIL;

	Safe_AddRef(pLevelUp);


	pLevelUp = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_LevelUp_Frame"), &UIDesc, &pLevelUp)))
		return E_FAIL;

	m_LevelUp.push_back(dynamic_cast<CUI_LevelUp*>(pLevelUp));
	if (nullptr == pLevelUp)
		return E_FAIL;

	Safe_AddRef(pLevelUp);


	// PlayerStatus 생성
	CGameObject* pPlayerStat = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 512.f * 0.75f;
	UIDesc.fCY = 175.f * 0.7f;
	UIDesc.fX = UIDesc.fCX * 0.5f;
	UIDesc.fY = UIDesc.fCY * 0.5f;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_PlayerStatus"), &UIDesc, &pPlayerStat)))
		return E_FAIL;

	m_pPlayerStatus = dynamic_cast<CUI_PlayerInfo*>(pPlayerStat);
	if (nullptr == m_pPlayerStatus)
		return E_FAIL;

	Safe_AddRef(m_pPlayerStatus);


	// ShowMenuBtn 생성
	CGameObject* pButton = nullptr;

	_float fOffset = 10.f;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 64.f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f) - fOffset;
	UIDesc.fY = UIDesc.fCY * 0.5f + fOffset;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Button_ShowMenu"), &UIDesc, &pButton)))
		return E_FAIL;

	m_pBtnShowMenu = dynamic_cast<CUI_BtnShowMenu*>(pButton);
	if (nullptr == m_pBtnShowMenu)
		return E_FAIL;

	Safe_AddRef(m_pBtnShowMenu);


	// ChangeCameraBtn 생성
	pButton = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 64.f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = UIDesc.fCX * 0.5f + (fOffset * 2.f);
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f) - fOffset;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Button_ChangeCamera"), &UIDesc, &pButton)))
		return E_FAIL;

	m_pBtnCamera = dynamic_cast<CUI_BtnChangeCamera*>(pButton);
	if (nullptr == m_pBtnCamera)
		return E_FAIL;

	Safe_AddRef(m_pBtnCamera);


	// InventoryBtn 생성
	pButton = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 64.f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f) - (fOffset * 7.f);
	UIDesc.fY = UIDesc.fCY * 0.5f + fOffset;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Button_ShowInven"), &UIDesc, &pButton)))
		return E_FAIL;

	m_pBtnInven = dynamic_cast<CUI_BtnInventory*>(pButton);
	if (nullptr == m_pBtnInven)
		return E_FAIL;

	Safe_AddRef(m_pBtnInven);


	// QuestBtn 생성
	pButton = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 64.f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = UIDesc.fCX * 0.5f + (fOffset * 2.f);
	UIDesc.fY = 170.f;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Button_ShowQuest"), &UIDesc, &pButton)))
		return E_FAIL;

	m_pBtnQuest = dynamic_cast<CUI_BtnQuickQuest*>(pButton);
	if (nullptr == m_pBtnQuest)
		return E_FAIL;

	Safe_AddRef(m_pBtnQuest);


	// Quest Window 생성
	CGameObject* pWindow = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 440.f;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f);
	UIDesc.fY = g_iWinSizeY * 0.5f;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Window_Quest"), &UIDesc, &pWindow)))
		return E_FAIL;

	m_pWindowQuest = dynamic_cast<CUI_WindowQuest*>(pWindow);
	if (nullptr == m_pWindowQuest)
		return E_FAIL;

	Safe_AddRef(m_pWindowQuest);


	//////////////////////
	// MainMenu UI 생성 //
	//////////////////////
	CGameObject* pMainBG = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Background"), &UIDesc, &pMainBG)))
		return E_FAIL;

	m_pMainBG = dynamic_cast<CUI_MainMenu*>(pMainBG);
	if (nullptr == m_pMainBG)
		return E_FAIL;

	Safe_AddRef(m_pMainBG);

	////////////////
	// LeftButton //
	////////////////
	m_MainMenuBtn.reserve(10);

	CGameObject* pMainBtn = nullptr;
	_float fBtnOffset = 85.f;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 128.f * 0.6f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = 80.f;
	UIDesc.fY = 285.f;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Btn_Character"), &UIDesc, &pMainBtn)))
		return E_FAIL;
	m_MainMenuBtn.push_back(dynamic_cast<CUI_MainMenu*>(pMainBtn));
	if (nullptr == pMainBtn)
		return E_FAIL;
	Safe_AddRef(pMainBtn);

	UIDesc.fY += fBtnOffset;
	pMainBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Btn_Equipment"), &UIDesc, &pMainBtn)))
		return E_FAIL;
	m_MainMenuBtn.push_back(dynamic_cast<CUI_MainMenu*>(pMainBtn));
	if (nullptr == pMainBtn)
		return E_FAIL;
	Safe_AddRef(pMainBtn);

	UIDesc.fY += fBtnOffset;
	pMainBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Btn_Imajinn"), &UIDesc, &pMainBtn)))
		return E_FAIL;
	m_MainMenuBtn.push_back(dynamic_cast<CUI_MainMenu*>(pMainBtn));
	if (nullptr == pMainBtn)
		return E_FAIL;
	Safe_AddRef(pMainBtn);

	UIDesc.fY += fBtnOffset;
	pMainBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Btn_Record"), &UIDesc, &pMainBtn)))
		return E_FAIL;
	m_MainMenuBtn.push_back(dynamic_cast<CUI_MainMenu*>(pMainBtn));
	if (nullptr == pMainBtn)
		return E_FAIL;
	Safe_AddRef(pMainBtn);

	UIDesc.fY += fBtnOffset;
	pMainBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Btn_Growth"), &UIDesc, &pMainBtn)))
		return E_FAIL;
	m_MainMenuBtn.push_back(dynamic_cast<CUI_MainMenu*>(pMainBtn));
	if (nullptr == pMainBtn)
		return E_FAIL;
	Safe_AddRef(pMainBtn);

	/////////////////
	// RightButton //
	/////////////////

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 128.f * 0.6f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - 80.f;
	UIDesc.fY = 285.f;

	pMainBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Btn_Community"), &UIDesc, &pMainBtn)))
		return E_FAIL;
	m_MainMenuBtn.push_back(dynamic_cast<CUI_MainMenu*>(pMainBtn));
	if (nullptr == pMainBtn)
		return E_FAIL;
	Safe_AddRef(pMainBtn);

	UIDesc.fY += fBtnOffset;
	pMainBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Btn_Dungeon"), &UIDesc, &pMainBtn)))
		return E_FAIL;
	m_MainMenuBtn.push_back(dynamic_cast<CUI_MainMenu*>(pMainBtn));
	if (nullptr == pMainBtn)
		return E_FAIL;
	Safe_AddRef(pMainBtn);

	UIDesc.fY += fBtnOffset;
	pMainBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Btn_Challenge"), &UIDesc, &pMainBtn)))
		return E_FAIL;
	m_MainMenuBtn.push_back(dynamic_cast<CUI_MainMenu*>(pMainBtn));
	if (nullptr == pMainBtn)
		return E_FAIL;
	Safe_AddRef(pMainBtn);

	UIDesc.fY += fBtnOffset;
	pMainBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Btn_Battle"), &UIDesc, &pMainBtn)))
		return E_FAIL;
	m_MainMenuBtn.push_back(dynamic_cast<CUI_MainMenu*>(pMainBtn));
	if (nullptr == pMainBtn)
		return E_FAIL;
	Safe_AddRef(pMainBtn);

	UIDesc.fY += fBtnOffset;
	pMainBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MainMenu_Btn_Shop"), &UIDesc, &pMainBtn)))
		return E_FAIL;
	m_MainMenuBtn.push_back(dynamic_cast<CUI_MainMenu*>(pMainBtn));
	if (nullptr == pMainBtn)
		return E_FAIL;
	Safe_AddRef(pMainBtn);

	// 닫기 버튼
	pButton = nullptr;
	fOffset = 10.f;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 64.f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f) - fOffset;
	UIDesc.fY = UIDesc.fCY * 0.5f + fOffset;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Common_Btn_Close"), &UIDesc, &pButton)))
		return E_FAIL;

	m_pBtnClose = dynamic_cast<CUI_BtnClose*>(pButton);
	if (nullptr == m_pBtnClose)
		return E_FAIL;
	Safe_AddRef(m_pBtnClose);

	// SubMenu Character Tab Buttons 생성
	m_SubMenuChar.reserve(8);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 128.f * 0.68f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = 230.f;
	UIDesc.fY = 245.f;

	CGameObject* pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Rank"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuChar.push_back(dynamic_cast<CUI_SubMenu_Character*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Skill"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuChar.push_back(dynamic_cast<CUI_SubMenu_Character*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Vehicle"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuChar.push_back(dynamic_cast<CUI_SubMenu_Character*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Costume"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuChar.push_back(dynamic_cast<CUI_SubMenu_Character*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Deck"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuChar.push_back(dynamic_cast<CUI_SubMenu_Character*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Title"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuChar.push_back(dynamic_cast<CUI_SubMenu_Character*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	// 두번째 열
	UIDesc.fX = 320.f;
	UIDesc.fY = 245.f + (fBtnOffset * 2.f);
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Style"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuChar.push_back(dynamic_cast<CUI_SubMenu_Character*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Battle"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuChar.push_back(dynamic_cast<CUI_SubMenu_Character*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);


	// MapText 생성
	if (LEVELID::LEVEL_EVERMORE == eID)
	{
		CGameObject* pMapText = nullptr;
		ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

		UIDesc.fCX = 193.f * 0.5f;
		UIDesc.fCY = 93.f * 0.5f;
		UIDesc.fX = g_iWinSizeX * 0.5f;
		UIDesc.fY = g_iWinSizeY * 0.15f;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MapName_Evermore"), &UIDesc, &pMapText)))
			return E_FAIL;

		m_pMapText = nullptr;
		m_pMapText = dynamic_cast<CUI_Basic*>(pMapText);
		if (nullptr == m_pMapText)
			return E_FAIL;

		Safe_AddRef(m_pMapText);
	}
	else if (LEVELID::LEVEL_KINGDOMHALL == eID)
	{
		CGameObject* pMapText = nullptr;
		ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

		UIDesc.fCX = 400.f * 0.5f;
		UIDesc.fCY = 93.f * 0.5f;
		UIDesc.fX = g_iWinSizeX * 0.5f;
		UIDesc.fY = g_iWinSizeY * 0.15f;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MapName_KingdomHall"), &UIDesc, &pMapText)))
			return E_FAIL;

		m_pMapText = nullptr;
		m_pMapText = dynamic_cast<CUI_Basic*>(pMapText);
		if (nullptr == m_pMapText)
			return E_FAIL;

		Safe_AddRef(m_pMapText);
	}
	else if (LEVELID::LEVEL_ICELAND == eID)
	{
		CGameObject* pMapText = nullptr;
		ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

		UIDesc.fCX = 300.f * 0.5f;
		UIDesc.fCY = 93.f * 0.5f;
		UIDesc.fX = g_iWinSizeX * 0.5f;
		UIDesc.fY = g_iWinSizeY * 0.15f;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MapName_IceLand"), &UIDesc, &pMapText)))
			return E_FAIL;

		m_pMapText = nullptr;
		m_pMapText = dynamic_cast<CUI_Basic*>(pMapText);
		if (nullptr == m_pMapText)
			return E_FAIL;

		Safe_AddRef(m_pMapText);
	}
	else if (LEVELID::LEVEL_WITCHFOREST == eID)
	{
		CGameObject* pMapText = nullptr;
		ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

		UIDesc.fCX = 193.f * 0.5f;
		UIDesc.fCY = 93.f * 0.5f;
		UIDesc.fX = g_iWinSizeX * 0.5f;
		UIDesc.fY = g_iWinSizeY * 0.15f;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MapName_WitchForest"), &UIDesc, &pMapText)))
			return E_FAIL;

		m_pMapText = nullptr;
		m_pMapText = dynamic_cast<CUI_Basic*>(pMapText);
		if (nullptr == m_pMapText)
			return E_FAIL;

		Safe_AddRef(m_pMapText);
	}
	else
	{
	}

	return S_OK;
}

HRESULT CUI_Manager::Tick_UIs(LEVELID eID, _float fTimeDelta)
{
	switch (eID)
	{
	case LEVELID::LEVEL_LOGO:
		return E_FAIL;
		break;

	case LEVELID::LEVEL_TEST:
		break;

	case LEVELID::LEVEL_LOBBY:
		Tick_LobbyLevel(fTimeDelta);
		break;

	case LEVELID::LEVEL_EVERMORE:
		Tick_EvermoreLevel(fTimeDelta);
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CUI_Manager::Tick_LobbyLevel(_float fTimeDelta)
{
	// Player Btn에 대한 상태를 제어한다.
	for (_uint i = 0; i < CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::UICHARACTERBTN_END; i++)
	{
		if (m_UnclickedPlayer[i]->Is_Clicked()) // Click상태로 확인되면
		{
			m_UnclickedPlayer[i]->Set_Active(false); // Unclicked를 false로, Clicked를 true로 전환한다.
			m_ClickedPlayer[i]->Set_Active(true);

			// i를 제외한 나머지 것들을 제어한다.
			for (_uint j = 0; j < CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::UICHARACTERBTN_END; j++)
			{
				// Arrived가 false인 경우에는
				if (!m_ClickedPlayer[i]->Is_Arrived())
				{
					if (j != i)
					{
						m_UnclickedPlayer[j]->Set_Clicked(false); // 클릭 이벤트가 발생하지 않도록 한다.
					}
				}
				else
				{
					// Arrived된 상태에서 다른 것에 Click Event가 발생하면 다시 원위치로 돌아가야한다.
					if (j != i)
					{
						if (m_UnclickedPlayer[j]->Is_Clicked())
						{
							m_ClickedPlayer[i]->Set_Move(true);
						}
					}
					
				}
			}
		}

		if (!m_ClickedPlayer[i]->Is_Active()) // Active가 False인데, unclicked도 false라면 unclicked를 true로 바꿔준다.
		{
			if (!m_UnclickedPlayer[i]->Is_Active())
			{
				m_UnclickedPlayer[i]->Set_Active(true);
			}
		}

		if (m_ClickedPlayer[i]->Get_IsMoved()) // 움직이는 상태라면
		{
			if (m_ClickedPlayer[i]->Get_MoveEnd())
			{
				// 원위치로 돌아온 상태이다
				m_ClickedPlayer[i]->Reset_InitializeInfo();
			}
		}
	}

	return S_OK;
}

HRESULT CUI_Manager::Tick_EvermoreLevel(_float fTimeDelta)
{
	if (nullptr != m_pUIMapName)
	{
		if (m_pUIMapName->Get_Active())
		{
			// MapName이 Active상태라면
			if (nullptr != m_pMapText)
			{
				if (!m_pMapText->Get_Active())
					m_pMapText->Set_Active(true);
				else
					m_pMapText->Set_Alpha(m_pUIMapName->Get_Alpha());
			}
		}
		else
		{
			if (nullptr != m_pMapText)
			{
				if (m_pMapText->Get_Active())
					m_pMapText->Set_Active(false);
			}
		}
	}

	// MainGame Btn과 BG가 활성화가 되면, 기본 세팅 UI들은 비활성화 되어야한다.
//	if (m_pMainBG->Get_Active())
//	{
//		OnOff_GamePlaySetting(false);
//	}
	// else로 조건을 주지 않는다. -> 꼬일 확률 높음
	// X버튼을 통해서 true로 전환하도록 한다.

	// TestCiode
	if (KEY_TAP(KEY::Q))
	{
		if (m_pWindowQuest->Get_Active())
			m_pWindowQuest->Set_Active(false);
		else
			m_pWindowQuest->Set_Active(true);
	}

	if (KEY_TAP(KEY::L))
	{
		if (m_LevelUp[CUI_LevelUp::UILEVELUP_FRAME]->Get_Active())
		{
			for (auto& pUI : m_LevelUp)
				pUI->Set_Active(false);
		}
		else
		{
			for (auto& pUI : m_LevelUp)
				pUI->Set_Active(true);
		}
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_GamePlaySetting(_bool bOnOff)
{
	if (bOnOff) // On
	{
		m_pPlayerStatus->Set_Active(true);
		m_pBtnShowMenu->Set_Active(true);
		m_pBtnCamera->Set_Active(true);
		m_pBtnInven->Set_Active(true);
		m_pBtnQuest->Set_Active(true);
	}
	else // Off
	{
		m_pPlayerStatus->Set_Active(false);
		m_pBtnShowMenu->Set_Active(false);
		m_pBtnCamera->Set_Active(false);
		m_pBtnInven->Set_Active(false);
		m_pBtnQuest->Set_Active(false);
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_MainMenu(_bool bOnOff)
{
	if (nullptr == m_pMainBG)
		return E_FAIL;

	if (bOnOff) // On
	{
		m_pMainBG->Set_Active(true);
		for (auto& pUI : m_MainMenuBtn)
		{
			if (nullptr != pUI)
				pUI->Set_Active(true);
		}
	}
	else // Off
	{
		m_pMainBG->Set_Active(false);
		for (auto& pUI : m_MainMenuBtn)
		{
			if (nullptr != pUI)
				pUI->Set_Active(false);
		}
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_QuestWindow(_bool bOnOff)
{
	if (nullptr == m_pWindowQuest)
		return E_FAIL;

	if (bOnOff) // On
	{
		m_pWindowQuest->Set_Active(true);
	}
	else // Off
	{
		m_pWindowQuest->Set_Active(false);
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_CloseButton(_bool bOnOff)
{
	if (nullptr == m_pBtnClose)
		return E_FAIL;

	if (bOnOff)
	{
		m_pBtnClose->Set_Active(true);
	}
	else
	{
		m_pBtnClose->Set_Active(false);
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_SubMenu(_bool bOnOff, _uint iMagicNum)
{
	if (0 > iMagicNum || 9 < iMagicNum)
		return E_FAIL;

	if (bOnOff) // true -> On
	{
		switch (iMagicNum)
		{
		case 0: // 캐릭터창을 선택했다.
			for (auto& iter : m_SubMenuChar)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			break;
		}
	}

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
	// Static Prototypes
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_Background"),
		CUI_Loading_Background::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_CharacterUI"),
		CUI_Loading_Character::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Fade_Black"),
		CUI_Fade::Create(m_pDevice, m_pContext, CUI_Fade::UI_VEIL::VEIL_BLACK), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Cursor"),
		CUI_Cursor::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_LevelUp_Frame"),
		CUI_LevelUp::Create(m_pDevice, m_pContext, CUI_LevelUp::UILEVELUP_FRAME), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_LevelUp_Background"),
		CUI_LevelUp::Create(m_pDevice, m_pContext, CUI_LevelUp::UILEVELUP_BG), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MapName"),
		CUI_MapName::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MapName_Evermore"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_MapName_Evermore", CUI_Basic::UI_BASIC::UIMAPNAME_EVERMORE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MapName_KingdomHall"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_MapName_KingdomHall", CUI_Basic::UI_BASIC::UIMAPNAME_KINGDOM), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MapName_IceLand"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_MapName_IceLand", CUI_Basic::UI_BASIC::UIMAPNAME_ICELAND), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MapName_WitchForest"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_MapName_WitchForest", CUI_Basic::UI_BASIC::UIMAPNAME_FOREST), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_PlayerStatus"),
		CUI_PlayerInfo::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Button_ShowMenu"),
		CUI_BtnShowMenu::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Button_ChangeCamera"),
		CUI_BtnChangeCamera::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Button_ShowInven"),
		CUI_BtnInventory::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Button_ShowQuest"),
		CUI_BtnQuickQuest::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Window_Quest"),
		CUI_WindowQuest::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Btn_AcceptQuest"),
		CUI_BtnAccept::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Quest_Text_Accept"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Quest_Text_Accept", CUI_Basic::UI_BASIC::UIQUEST_ACCEPT), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Quest_Text_Finish"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Quest_Text_Finish", CUI_Basic::UI_BASIC::UIQUEST_FINISH), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Background"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Background", CUI_MainMenu::UI_MAINMENU_TYPE::MAINMENU_BG), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Btn_Character"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Btn_Character",
			CUI_MainMenu::UI_MAINMENU_TYPE::MAINBTN_CHARACTER), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Btn_Equipment"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Btn_Equipment",
			CUI_MainMenu::UI_MAINMENU_TYPE::MAINBTN_EQUIPMENT), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Btn_Imajinn"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Btn_Imajinn",
			CUI_MainMenu::UI_MAINMENU_TYPE::MAINBTN_IMAJINN), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Btn_Record"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Btn_Record",
			CUI_MainMenu::UI_MAINMENU_TYPE::MAINBTN_RECORD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Btn_Growth"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Btn_Growth",
			CUI_MainMenu::UI_MAINMENU_TYPE::MAINBTN_GROWTH), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Btn_Community"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Btn_Community",
			CUI_MainMenu::UI_MAINMENU_TYPE::MAINBTN_COMMUNITY), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Btn_Dungeon"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Btn_Dungeon",
			CUI_MainMenu::UI_MAINMENU_TYPE::MAINBTN_DUNGEON), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Btn_Challenge"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Btn_Challenge",
			CUI_MainMenu::UI_MAINMENU_TYPE::MAINBTN_CHALLENGE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Btn_Battle"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Btn_Battle",
			CUI_MainMenu::UI_MAINMENU_TYPE::MAINBTN_BATTLE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MainMenu_Btn_Shop"),
		CUI_MainMenu::Create(m_pDevice, m_pContext, L"UI_MainMenu_Btn_Shop",
			CUI_MainMenu::UI_MAINMENU_TYPE::MAINBTN_SHOP), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Common_Btn_Close"),
		CUI_BtnClose::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Rank"),
		CUI_SubMenu_Character::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Character_Rank",
			CUI_SubMenu_Character::UI_SUBCHARACTER_TYPE::CHARBTN_RANK), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Skill"),
		CUI_SubMenu_Character::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Character_Skill",
			CUI_SubMenu_Character::UI_SUBCHARACTER_TYPE::CHARBTN_SKILL), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Vehicle"),
		CUI_SubMenu_Character::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Character_Vehicle",
			CUI_SubMenu_Character::UI_SUBCHARACTER_TYPE::CHARBTN_VEHICLE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Costume"),
		CUI_SubMenu_Character::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Character_Costume",
			CUI_SubMenu_Character::UI_SUBCHARACTER_TYPE::CHARBTN_COSTUME), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Deck"),
		CUI_SubMenu_Character::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Character_Deck",
			CUI_SubMenu_Character::UI_SUBCHARACTER_TYPE::CHARBTN_DECK), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Title"),
		CUI_SubMenu_Character::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Character_Title",
			CUI_SubMenu_Character::UI_SUBCHARACTER_TYPE::CHARBTN_TITLE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Style"),
		CUI_SubMenu_Character::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Character_Style",
			CUI_SubMenu_Character::UI_SUBCHARACTER_TYPE::CHARBTN_STYLE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Character_Battle"),
		CUI_SubMenu_Character::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Character_Battle",
			CUI_SubMenu_Character::UI_SUBCHARACTER_TYPE::CHARBTN_BATTLE), LAYER_UI)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Manager::Ready_UILobbyPrototypes()
{
	//////////////
	// Textures //
	//////////////

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_NicknameFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/UI_Lobby_NickName_Frame.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Btn_GameStart"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/UI_Btn_HighGreen.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Common_Deco_UpperMenuLeft"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_Btn_Back_Deco_Left.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Common_Deco_UpperMenRight"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_Btn_Back_Deco_Right.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_TitleText"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/UI_Lobby_Text.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_BtnText"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/UI_Lobby_Text_GameStart.png")))))
		return E_FAIL;

	// 클릭 전 이미지(캐릭터 선택창)
	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Unclick_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/Icon_Portrait_Style01_%d.png"), 5))))
		return E_FAIL;
	// 클릭 후 이미지(캐릭터 선택창)
	if (FAILED(GI->Add_Prototype(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Click_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/Icon_Portrait_Style02_%d.png"), 5))))
		return E_FAIL;
	
	/////////////////
	// GameObjects //
	/////////////////

	// Character NickName Frame
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_NicknameFrame"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Lobby_NicknameFrame", CUI_Basic::UI_BASIC::UILOBBY_NICKFRAME), LAYER_UI)))
		return E_FAIL;

	// GameStart Button
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Btn_Basic"),
		CUI_BasicButton::Create(m_pDevice, m_pContext, L"UI_Btn_Basic_GameStart", CUI_BasicButton::UIBUTTON_TYPE::BUTTON_CHANGESCENE), LAYER_UI)))
		return E_FAIL;

	// 상단바 Default Deco(1)
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Common_MenuDeco_Left"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Common_MenuDeco_Left", CUI_Basic::UI_BASIC::UISTATIC_MENUDECO_L), LAYER_UI)))
		return E_FAIL;

	// 좌상단 Title Text
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Title"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Lobby_Title", CUI_Basic::UI_BASIC::UILOBBY_TITLE), LAYER_UI)))
		return E_FAIL;

	// GameStart Text
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_BtnText"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Lobby_BtnText", CUI_Basic::UI_BASIC::UILOBBY_BTNTEXT), LAYER_UI)))
		return E_FAIL;

	// Character Select Btn
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Btn_Unclicked_Rogue"),
		CUI_BtnCharacterSelect::Create(m_pDevice, m_pContext, L"UI_Lobby_Btn_Uncliked_Rogue",
			CUI_BtnCharacterSelect::UI_SELECTBTN_TYPE::BTN_UNCLIKED,
			CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::BTN_ROGUE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Btn_Unclicked_Destroyer"),
		CUI_BtnCharacterSelect::Create(m_pDevice, m_pContext, L"UI_Lobby_Btn_Uncliked_Destroyer",
			CUI_BtnCharacterSelect::UI_SELECTBTN_TYPE::BTN_UNCLIKED,
			CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::BTN_DESTROYER), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Btn_Unclicked_Witch"),
		CUI_BtnCharacterSelect::Create(m_pDevice, m_pContext, L"UI_Lobby_Btn_Uncliked_Witch",
			CUI_BtnCharacterSelect::UI_SELECTBTN_TYPE::BTN_UNCLIKED,
			CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::BTN_WITCH), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Btn_Unclicked_Engineer"),
		CUI_BtnCharacterSelect::Create(m_pDevice, m_pContext, L"UI_Lobby_Btn_Uncliked_Engineer",
			CUI_BtnCharacterSelect::UI_SELECTBTN_TYPE::BTN_UNCLIKED,
			CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::BTN_ENGINEER), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Btn_Unclicked_Swordman"),
		CUI_BtnCharacterSelect::Create(m_pDevice, m_pContext, L"UI_Lobby_Btn_Uncliked_Swordman",
			CUI_BtnCharacterSelect::UI_SELECTBTN_TYPE::BTN_UNCLIKED,
			CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::BTN_SWORDMAN), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Btn_Clicked_Rogue"),
		CUI_BtnCharacterSelect::Create(m_pDevice, m_pContext, L"UI_Lobby_Btn_Uncliked_Rogue",
			CUI_BtnCharacterSelect::UI_SELECTBTN_TYPE::BTN_CLICKED,
			CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::BTN_ROGUE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Btn_Clicked_Destroyer"),
		CUI_BtnCharacterSelect::Create(m_pDevice, m_pContext, L"UI_Lobby_Btn_Clicked_Destroyer",
			CUI_BtnCharacterSelect::UI_SELECTBTN_TYPE::BTN_CLICKED,
			CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::BTN_DESTROYER), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Btn_Clicked_Witch"),
		CUI_BtnCharacterSelect::Create(m_pDevice, m_pContext, L"UI_Lobby_Btn_Clicked_Witch",
			CUI_BtnCharacterSelect::UI_SELECTBTN_TYPE::BTN_CLICKED,
			CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::BTN_WITCH), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Btn_Clicked_Engineer"),
		CUI_BtnCharacterSelect::Create(m_pDevice, m_pContext, L"UI_Lobby_Btn_Clicked_Engineer",
			CUI_BtnCharacterSelect::UI_SELECTBTN_TYPE::BTN_CLICKED,
			CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::BTN_ENGINEER), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Btn_Clicked_Swordman"),
		CUI_BtnCharacterSelect::Create(m_pDevice, m_pContext, L"UI_Lobby_Btn_Clicked_Swordman",
			CUI_BtnCharacterSelect::UI_SELECTBTN_TYPE::BTN_CLICKED,
			CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::BTN_SWORDMAN), LAYER_UI)))
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
	Safe_Release(m_pMapText);
	Safe_Release(m_pPlayerStatus);
	Safe_Release(m_pBtnShowMenu);
	Safe_Release(m_pBtnCamera);
	Safe_Release(m_pBtnInven);
	Safe_Release(m_pBtnQuest);
	Safe_Release(m_pWindowQuest);
	Safe_Release(m_pMainBG);
	Safe_Release(m_pBtnClose);

	for (auto& pBasic : m_Basic)
		Safe_Release(pBasic);
	m_Basic.clear();

	for (auto& pButton : m_Buttons)
		Safe_Release(pButton);
	m_Buttons.clear();

	for (auto& pUnclickedBtn : m_UnclickedPlayer)
		Safe_Release(pUnclickedBtn);
	m_UnclickedPlayer.clear();

	for (auto& pClickedBtn : m_ClickedPlayer)
		Safe_Release(pClickedBtn);
	m_ClickedPlayer.clear();

	for (auto& pLevelUp : m_LevelUp)
		Safe_Release(pLevelUp);
	m_LevelUp.clear();

	for (auto& pMainBtn : m_MainMenuBtn)
		Safe_Release(pMainBtn);
	m_MainMenuBtn.clear();

	for (auto& pCharBtn : m_SubMenuChar)
		Safe_Release(pCharBtn);
	m_SubMenuChar.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
