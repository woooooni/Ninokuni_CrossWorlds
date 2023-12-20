#include "stdafx.h"
#include "..\Public\UI_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Effect.h"
#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"

#include "UI_Fade.h"
#include "UI_Veil.h"
#include "UI_Basic.h"
#include "UI_Cursor.h"
#include "UI_LevelUp.h"
#include "UI_MapName.h"
#include "UI_BtnBack.h"
#include "UI_BtnClose.h"
#include "UI_MainMenu.h"
#include "UI_BtnAccept.h"
#include "UI_PlayerInfo.h"
#include "UI_PopupQuest.h"
#include "UI_PlayerHPBar.h"
#include "UI_BasicButton.h"
#include "UI_WindowQuest.h"
#include "UI_Dialog_Flip.h"
#include "UI_BtnShowMenu.h"
#include "UI_Setting_Icon.h"
#include "UI_SubMenu_Shop.h"
#include "UI_Text_TabMenu.h"
#include "UI_BtnInventory.h"
#include "UI_PlayerEXPBar.h"
#include "UI_MonsterHP_Bar.h"
#include "UI_MenuSeparator.h"
#include "UI_Dialog_Window.h"
#include "UI_BtnQuickQuest.h"
#include "UI_Setting_Slider.h"
#include "UI_Setting_Window.h"
#include "UI_BtnShowSetting.h"
#include "UI_WindowWorldMap.h"
#include "UI_Dialog_Portrait.h"
#include "UI_Loading_Imajinn.h"
#include "UI_Setting_Section.h"
#include "UI_SubMenu_Imajinn.h"
#include "UI_BtnChangeCamera.h"
#include "UI_Loading_MainLogo.h"
#include "UI_Btn_WorldMapIcon.h"
#include "UI_Dialog_MiniWindow.h"
#include "UI_Default_BackCloud.h"
#include "UI_Loading_Character.h"
#include "UI_SubMenu_Character.h"
#include "UI_SubMenu_Equipment.h"
#include "UI_Setting_BtnVolume.h"
#include "UI_Default_BackStars.h"
#include "UI_Default_Background.h"
#include "UI_BtnCharacterSelect.h"
#include "UI_Loading_Background.h"
#include "UI_WeaponSection_Slot.h"
#include "UI_Loading_ProgressBar.h"
#include "UI_Loading_Information.h"
#include "UI_MonsterHP_Elemental.h"
#include "UI_ImajinnSection_Slot.h"
#include "UI_SkillSection_BtnRoll.h"
#include "UI_SkillSection_BtnJump.h"
#include "UI_MonsterHP_Background.h"
#include "UI_Loading_CharacterLogo.h"
#include "UI_ImajinnSection_Vehicle.h"
#include "UI_SkillSection_Background.h"
#include "UI_ImajinnSection_Emoticon.h"
#include "UI_MonsterHP_ElementalFrame.h"
#include "UI_SkillSection_Interaction.h"
#include "UI_ImajinnSection_Background.h"
#include "UI_WeaponSection_DefaultWeapon.h"
#include "UI_SkillSection_BtnInteraction.h"

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

HRESULT CUI_Manager::Ready_Loadings()
{
	_int iRandom = GI->RandomInt(0, 4);

	// 배경화면
	CGameObject* pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Loading_Background"), nullptr, &pBackground)))
		return E_FAIL;
	if (nullptr == pBackground)
		return E_FAIL;
	dynamic_cast<CUI_Loading_Background*>(pBackground)->Set_TextureIndex(iRandom);

	// 캐릭터
	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY * 2.f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY;
	CGameObject* pCharacter = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Loading_CharacterUI"), &UIDesc, &pCharacter)))
		return E_FAIL;
	dynamic_cast<CUI_Loading_Character*>(pCharacter)->Set_TextureIndex(iRandom);

	// 캐릭터 설명(with 로고)
	CGameObject* pLogo = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Loading_CharacterLogo"), nullptr, &pLogo)))
		return E_FAIL;
	if (nullptr == pLogo)
		return E_FAIL;
	dynamic_cast<CUI_Loading_CharacterLogo*>(pLogo)->Set_TextureIndex(iRandom);
	dynamic_cast<CUI_Loading_CharacterLogo*>(pLogo)->Set_Text();

	CGameObject* pImajinn = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Loading_Imajinn"), nullptr, &pImajinn)))
		return E_FAIL;

//	CGameObject* pInform = nullptr;
//	if (FAILED(GI->Add_GameObject(LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Loading_Information"), nullptr, &pInform)))
//		return E_FAIL;
//	if (nullptr == pInform)
//		return E_FAIL;
//	dynamic_cast<CUI_Loading_Logo*>(pInform)->Set_TextureIndex(GI->RandomInt(0, 2));

	//UI_Loading_MainLogo
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 1024.f * 0.2f;
	UIDesc.fCY = 512.f * 0.2f;
//	UIDesc.fX = (g_iWinSizeX * 0.5f);
//	UIDesc.fY = (UIDesc.fCY * 0.5f) + 25.f;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f) - 25.f;
	UIDesc.fY = (UIDesc.fCY * 0.5f) + 25.f;

	CGameObject* pMainLogo = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Loading_MainLogo_Text"), &UIDesc, &pMainLogo)))
		return E_FAIL;
	if (nullptr == pMainLogo)
		return E_FAIL;

//	CUI::UI_INFO NumDesc = {};
//	ZeroMemory(&NumDesc, sizeof(CUI::UI_INFO));
//
//	NumDesc.fCX = 147.f * 0.3f;
//	NumDesc.fCY = 198.f * 0.3f;
//	NumDesc.fX = UIDesc.fX - 40.f;
//	NumDesc.fY = UIDesc.fY - 9.f;
//
//	pMainLogo = nullptr;
//	if (FAILED(GI->Add_GameObject(LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Loading_MainLogo_Number"), &NumDesc, &pMainLogo)))
//		return E_FAIL;
//	if (nullptr == pMainLogo)
//		return E_FAIL;

	//ProgressBar
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	_float fOffset = 40.f;
	UIDesc.fCX = 1493.f;
	UIDesc.fCY = 40.f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY + fOffset);

	CGameObject* pBar = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Loading_Progress_Back"), &UIDesc, &pBar)))
		return E_FAIL;
	if (nullptr == pBar)
		return E_FAIL;

	pBar = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Loading_Progress_Bar"), &UIDesc, &pBar)))
		return E_FAIL;
	if (nullptr == pBar)
		return E_FAIL;

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



	_float fOffset = 30.f;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 64.f * 0.6f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = (UIDesc.fCX * 0.5f) + fOffset;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f + fOffset);

	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Icon_ShowSetting"), &UIDesc, &pButton)))
		return E_FAIL;
	m_pBtnShowSetting = dynamic_cast<CUI_BtnShowSetting*>(pButton);
	if (nullptr == m_pBtnShowSetting)
		return E_FAIL;
	Safe_AddRef(m_pBtnShowSetting);

	// For Setting Window
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

//	UIDesc.fCX = 957.f;
//	UIDesc.fCY = 580.f;
	UIDesc.fCX = /* _float(g_iWinSizeX); */ 2000.f;
	UIDesc.fCY = /* _float(g_iWinSizeY); */ 1600.f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	CGameObject* pBG = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Setting_Background"), &UIDesc, &pBG)))
		return E_FAIL;
	m_pSettingBG = dynamic_cast<CUI_Setting_Window*>(pBG);
	if (nullptr == m_pSettingBG)
		return E_FAIL;
	Safe_AddRef(m_pSettingBG);


	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 128.f * 0.6f;
	UIDesc.fCY = 128.f * 0.55f;
	UIDesc.fX = UIDesc.fCX * 0.5f + 3.f;
	UIDesc.fY = UIDesc.fCY * 0.5f;

	pBG = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Btn_Back"), &UIDesc, &pBG)))
		return E_FAIL;
	if (nullptr == pBG)
		return E_FAIL;

	// Veil
//	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
//
//	UIDesc.fCX = g_iWinSizeX;
//	UIDesc.fCY = g_iWinSizeY;
//	UIDesc.fX = g_iWinSizeX * 0.5f;
//	UIDesc.fY = g_iWinSizeY * 0.5f;
//
//	CGameObject* pBackground = nullptr;
//	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Veil"), &UIDesc, &pBackground)))
//		return E_FAIL;
//	m_pUIVeil = dynamic_cast<CUI_Veil*>(pBackground);
//	if (nullptr == m_pUIVeil)
//		return E_FAIL;
//	Safe_AddRef(m_pUIVeil);
//
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

	UIDesc.fCX = 512.f * 0.6f;
	UIDesc.fCY = 175.f * 0.57;
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

#pragma region MAINMENU_BTN

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
	fOffset = 20.f;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 64.f * 0.7f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f) - fOffset;
	UIDesc.fY = UIDesc.fCY * 0.5f + fOffset;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Common_Btn_Close"), &UIDesc, &pButton)))
		return E_FAIL;

	m_pBtnClose = dynamic_cast<CUI_BtnClose*>(pButton);
	if (nullptr == m_pBtnClose)
		return E_FAIL;
	Safe_AddRef(m_pBtnClose);
	m_pBtnClose->Set_Active(false);

#pragma endregion

#pragma region SUBMENU_CHARACTER

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

#pragma endregion

#pragma region MENU_SEPARATOR

	// Menu Separator Left
	m_MenuSeparator.reserve(2);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 44.f * 0.9f;
	UIDesc.fCY = 492.f;
	UIDesc.fX = 160.f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	CGameObject* pSeparator = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Common_MainMenu_Separator"), &UIDesc, &pSeparator)))
		return E_FAIL;
	m_MenuSeparator.push_back(dynamic_cast<CUI_MenuSeparator*>(pSeparator));
	if (nullptr == pSeparator)
		return E_FAIL;
	Safe_AddRef(pSeparator);

	// Menu Separaor Right
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 44.f * 0.9f;
	UIDesc.fCY = 492.f;
	UIDesc.fX = g_iWinSizeX - 160.f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	pSeparator = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Common_MainMenu_Separator"), &UIDesc, &pSeparator)))
		return E_FAIL;
	m_MenuSeparator.push_back(dynamic_cast<CUI_MenuSeparator*>(pSeparator));
	if (nullptr == pSeparator)
		return E_FAIL;
	Safe_AddRef(pSeparator);

#pragma endregion

#pragma region SUBMENU_EQUIPMENT

	// SubMenu Equipment Tab Buttons 생성
	m_SubMenuEquip.reserve(6);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 128.f * 0.68f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = 230.f;
	UIDesc.fY = 245.f;

	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_Inventory"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuEquip.push_back(dynamic_cast<CUI_SubMenu_Equipment*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_Forge"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuEquip.push_back(dynamic_cast<CUI_SubMenu_Equipment*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_Gem"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuEquip.push_back(dynamic_cast<CUI_SubMenu_Equipment*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_TinyGenie"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuEquip.push_back(dynamic_cast<CUI_SubMenu_Equipment*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_Transcend"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuEquip.push_back(dynamic_cast<CUI_SubMenu_Equipment*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_FishingRod"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuEquip.push_back(dynamic_cast<CUI_SubMenu_Equipment*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

#pragma endregion

#pragma region SUBMENU_IMAGINN

	// SubMenu Imaginn Tab Buttons 생성
	m_SubMenuPet.reserve(8);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 128.f * 0.68f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = 230.f;
	UIDesc.fY = 245.f;

	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Imajinn"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuPet.push_back(dynamic_cast<CUI_SubMenu_Imajinn*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Training"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuPet.push_back(dynamic_cast<CUI_SubMenu_Imajinn*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Toy"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuPet.push_back(dynamic_cast<CUI_SubMenu_Imajinn*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Cradle"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuPet.push_back(dynamic_cast<CUI_SubMenu_Imajinn*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Return"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuPet.push_back(dynamic_cast<CUI_SubMenu_Imajinn*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Dungeon"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuPet.push_back(dynamic_cast<CUI_SubMenu_Imajinn*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	// 두번째 열
	UIDesc.fX = 320.f;
	UIDesc.fY = 245.f + (fBtnOffset * 2.f);
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Unitetree"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuPet.push_back(dynamic_cast<CUI_SubMenu_Imajinn*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Ultimate"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuPet.push_back(dynamic_cast<CUI_SubMenu_Imajinn*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

#pragma endregion

#pragma region SUBMENU_SHOP

	// SubMenu Shop Tab Buttons 생성
	m_SubMenuShop.reserve(3);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 128.f * 0.68f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - 230.f;
	UIDesc.fY = 370.f;

	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Shop_Package"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuShop.push_back(dynamic_cast<CUI_SubMenu_Shop*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Shop_Gacha"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuShop.push_back(dynamic_cast<CUI_SubMenu_Shop*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

	UIDesc.fY += fBtnOffset;
	pSubBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SubMenu_Btn_Shop_Token"), &UIDesc, &pSubBtn)))
		return E_FAIL;
	m_SubMenuShop.push_back(dynamic_cast<CUI_SubMenu_Shop*>(pSubBtn));
	if (nullptr == pSubBtn)
		return E_FAIL;
	Safe_AddRef(pSubBtn);

#pragma endregion

#pragma region MAP_NAME_TEXT

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

#pragma endregion

	// WorldMap Background 생성
	CGameObject* pBackground = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 2240.f;
	UIDesc.fCY = 1260.f;
	UIDesc.fX = UIDesc.fCX * 0.5f - 300.f;
	UIDesc.fY = g_iWinSizeY * 0.5f + 100.f;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_WorldMap_Background"), &UIDesc, &pBackground)))
		return E_FAIL;

	m_pWorldMapBG = dynamic_cast<CUI_WindowWorldMap*>(pBackground);
	if (nullptr == m_pWorldMapBG)
		return E_FAIL;
	Safe_AddRef(m_pWorldMapBG);

	// SkillSection
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 400.f * 0.85f;
	UIDesc.fCY = 400.f * 0.85f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f;
	UIDesc.fY = g_iWinSizeY - UIDesc.fCY * 0.5f;

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Background"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pSkillBG = dynamic_cast<CUI_SkillSection_Background*>(pBackground);
	if (nullptr == m_pSkillBG)
		return E_FAIL;
	Safe_AddRef(m_pSkillBG);

	// Imajinn Section
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 540.f * 0.65f;
	UIDesc.fCY = 172.f * 0.6f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY - UIDesc.fCY * 0.5f - fOffset;

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_ImajinnSection_Background"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pImajinnBG = dynamic_cast<CUI_ImajinnSection_Background*>(pBackground);
	if (nullptr == m_pImajinnBG)
		return E_FAIL;
	Safe_AddRef(m_pImajinnBG);

	// Default Bacground(for Tabs)
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Common_Default_Background"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pDefaultBG = dynamic_cast<CUI_Default_Background*>(pBackground);
	if (nullptr == m_pDefaultBG)
		return E_FAIL;
	Safe_AddRef(m_pDefaultBG);

	// Veil
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Veil"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pUIVeil = dynamic_cast<CUI_Veil*>(pBackground);
	if (nullptr == m_pUIVeil)
		return E_FAIL;
	Safe_AddRef(m_pUIVeil);

	// PlayerStatus_EXP
	m_PlayerEXP.reserve(2);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = 8.f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f);

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_PlayerStatus_EXPBackground"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_PlayerEXP.push_back(dynamic_cast<CUI_PlayerEXPBar*>(pBackground));
	if (nullptr == pBackground)
		return E_FAIL;
	Safe_AddRef(pBackground);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = 5.f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f);

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_PlayerStatus_EXPBar"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_PlayerEXP.push_back(dynamic_cast<CUI_PlayerEXPBar*>(pBackground));
	if (nullptr == pBackground)
		return E_FAIL;
	Safe_AddRef(pBackground);

#pragma region MONSTER_HPBAR

	// MonsterHP
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 300.f * 0.8f;
	UIDesc.fCY = 50.f * 0.8f;
	UIDesc.fX = g_iWinSizeX * 0.5f + 30.f;
	UIDesc.fY = UIDesc.fCY * 0.5f + 35.f;

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MonsterHP_Background"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pMonsterHPBack = dynamic_cast<CUI_MonsterHP_Background*>(pBackground);
	if (nullptr == m_pMonsterHPBack)
		return E_FAIL;
	Safe_AddRef(m_pMonsterHPBack);

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MonsterHP_Bar"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pMonsterHPBar = dynamic_cast<CUI_MonsterHP_Bar*>(pBackground);
	if (nullptr == m_pMonsterHPBar)
		return E_FAIL;
	Safe_AddRef(m_pMonsterHPBar);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 128.f * 0.6f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX * 0.5f - 130.f;
	UIDesc.fY = UIDesc.fCY * 0.5f + 15.f;

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MonsterHP_Elemental_Frame"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pMonsterFrame = dynamic_cast<CUI_MonsterHP_ElementalFrame*>(pBackground);
	if (nullptr == m_pMonsterFrame)
		return E_FAIL;
	Safe_AddRef(m_pMonsterFrame);

	UIDesc.fCX = 64.f * 0.6f;
	UIDesc.fCY = UIDesc.fCX;
	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MonsterHP_Elemental"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pMonsterElemental = dynamic_cast<CUI_MonsterHP_Elemental*>(pBackground);
	if (nullptr == m_pMonsterElemental)
		return E_FAIL;
	Safe_AddRef(m_pMonsterElemental);

#pragma endregion

#pragma region DIALOGUE

	// For Dialog
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	fOffset = 20.f;
	UIDesc.fCX = 1050.f * 0.9f;
	UIDesc.fCY = 359.f * 0.75f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f + fOffset);

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Dialog_DefaultWindow"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pDialogWindow = dynamic_cast<CUI_Dialog_Window*>(pBackground);
	if (nullptr == m_pDialogWindow)
		return E_FAIL;
	Safe_AddRef(m_pDialogWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	fOffset = 130.f;
	UIDesc.fCX = 820.f * 0.45f;
	UIDesc.fCY = 348.f * 0.5f;
	UIDesc.fX = g_iWinSizeX * 0.5f + 90.f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f + fOffset);

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Dialog_MiniWindow"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pDialogMini = dynamic_cast<CUI_Dialog_MiniWindow*>(pBackground);
	if (nullptr == m_pDialogMini)
		return E_FAIL;
	Safe_AddRef(m_pDialogMini);

	m_Portrait.reserve(2);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	fOffset = 92.f;
	UIDesc.fCX = 256.f * 0.95f;
	UIDesc.fCY = 256.f * 0.95f;
	UIDesc.fX = g_iWinSizeX * 0.5f - 180.f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f + fOffset);

	CGameObject* pPortrait = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Dialog_PortraitFrame"), &UIDesc, &pPortrait)))
		return E_FAIL;
	m_Portrait.push_back(dynamic_cast<CUI_Dialog_Portrait*>(pPortrait));
	if (nullptr == pPortrait)
		return E_FAIL;
	Safe_AddRef(pPortrait);

#pragma endregion

	// Menu Title
	CGameObject* pTabTitle = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Common_TabMenuTitle"), nullptr, &pTabTitle)))
		return E_FAIL;
	m_pTabMenuTitle = dynamic_cast<CUI_Text_TabMenu*>(pTabTitle);
	if (nullptr == m_pTabMenuTitle)
		return E_FAIL;
	Safe_AddRef(m_pTabMenuTitle);

	return S_OK;
}

HRESULT CUI_Manager::Tick_UIs(LEVELID eID, _float fTimeDelta)
{
	switch (eID)
	{
	case LEVELID::LEVEL_LOGO:
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

	if (KEY_TAP(KEY::W))
	{
		if (m_pWorldMapBG->Get_Active()) // 켜져있다면
			OnOff_WorldMap(false); // 끈다
		else // 꺼져있다면
			OnOff_WorldMap(true); // 켠다
	}

	if (KEY_TAP(KEY::P))
	{
		if (m_pMonsterHPBack->Get_Active()) // 켜져있다면
			OnOff_MonsterHP(false); // 끈다
		else // 꺼져있다면
			OnOff_MonsterHP(true, ELEMENTAL_TYPE::DARK); // 임시
	}

	if (KEY_TAP(KEY::F9))
	{
		if (!m_pDefaultBG->Get_Active())
		{
			m_pDefaultBG->Set_Active(true);
			OnOff_GamePlaySetting(false);
		}
		else
		{
			OnOff_GamePlaySetting(true);
			m_pDefaultBG->Set_Active(false);
		}
	}

	if (KEY_TAP(KEY::F8))
	{
		// DIalogBox Test
		if (nullptr == m_pDialogWindow)
			return E_FAIL;

		if (m_pDialogWindow->Get_Active())
			OnOff_DialogWindow(false, 0);
		else
			OnOff_DialogWindow(true, 0);
	}

	if (KEY_TAP(KEY::F7))
	{
		// DialogBox(Mini) Test
		if (nullptr == m_pDialogMini)
			return E_FAIL;

		if (m_pDialogMini->Get_Active())
			OnOff_DialogWindow(false, 1);
		else
			OnOff_DialogWindow(true, 1);
	}

	return S_OK;
}

HRESULT CUI_Manager::Using_CloseButton()
{
	if (nullptr != m_pMainBG)
	{
		if (m_pMainBG->Get_Active())
		{
			OnOff_MainMenu(false);// MainMenu창을 닫고
			OnOff_GamePlaySetting(true); // 기본 세팅을 켠다
	
			OnOff_CloseButton(false); // CloseBtn도 없앤다.
		}
	}

	else if (nullptr != m_pSettingBG)
	{
		if (m_pSettingBG->Get_Active())
		{
			m_pSettingBG->Set_Active(false); // Setting창을 닫고
			OnOff_CloseButton(false); // ClostBtn도 없앤다.
			if (nullptr != m_pUIVeil)
				if (m_pUIVeil->Get_Active())
					m_pUIVeil->Set_Active(false);
		}
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_Veil(_bool bOnOff)
{
	if (nullptr == m_pUIVeil)
		return E_FAIL;

	if (bOnOff)
	{
		if (nullptr != m_pUIVeil)
		{
			if (!m_pUIVeil->Get_Active())
				m_pUIVeil->Set_Active(true);
		}
	}
	else
	{
		if (nullptr != m_pUIVeil)
		{
			if (m_pUIVeil->Get_Active())
				m_pUIVeil->Set_Active(false);
		}
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_SettingWindow(_bool bOnOff)
{
	if (nullptr == m_pSettingBG)
		return E_FAIL;

	if (bOnOff)
	{
		if (!m_pSettingBG->Get_Active())
		{
			m_pSettingBG->Set_Active(true);
			if (nullptr != m_pBtnClose)
				m_pBtnClose->Set_Active(true);
		}

		OnOff_Veil(true);
	}
	else
	{
		OnOff_Veil(false);
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
		m_pSkillBG->Set_Active(true);
		m_pImajinnBG->Set_Active(true);

		OnOff_MonsterHP(true);

		// EXP Bar를 보여준다.
		for (auto& iter : m_PlayerEXP)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}
	}
	else // Off
	{
		m_pPlayerStatus->Set_Active(false);
		m_pBtnShowMenu->Set_Active(false);
		m_pBtnCamera->Set_Active(false);
		m_pBtnInven->Set_Active(false);
		m_pBtnQuest->Set_Active(false);
		m_pSkillBG->Set_Active(false);
		m_pImajinnBG->Set_Active(false);
	
		OnOff_MonsterHP(false);

		// EXP Bar를 감춘다
		for (auto& iter : m_PlayerEXP)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
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
		m_pBtnClose->Set_Active(true); // Close버튼
	}
	else // Off : 모든 Menu관련 창을 꺼야한다.
	{
		m_pMainBG->Set_Active(false);
		for (auto& pUI : m_MainMenuBtn)
		{
			if (nullptr != pUI)
				pUI->Set_Active(false);
		}
		for (auto& pSepar : m_MenuSeparator) // 세퍼레이터가 활성화 되어있다면 끈다.
		{
			if (nullptr != pSepar)
				pSepar->Set_Active(false);
		}
		for (auto& pBtnChar : m_SubMenuChar) // Char Btn이 활성화 되어있다면 끈다.
		{
			if (nullptr != pBtnChar)
				pBtnChar->Set_Active(false);
		}
		for (auto& pBtnEquip : m_SubMenuEquip) // Equip Btn이 활성화 되어있다면 끈다.
		{
			if (nullptr != pBtnEquip)
				pBtnEquip->Set_Active(false);
		}
		for (auto& pBtnPet : m_SubMenuPet) // Imajinn Btn이 활성화 되어있다면 끈다.
		{
			if (nullptr != pBtnPet)
				pBtnPet->Set_Active(false);
		}
		for (auto& pBtnShop : m_SubMenuShop) // Shop Btn이 활성화 되어있다면 끈다.
		{
			if (nullptr != pBtnShop)
				pBtnShop->Set_Active(false);
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

HRESULT CUI_Manager::OnOff_MonsterHP(_bool bOnOff, ELEMENTAL_TYPE eType)
{
	// 몬스터가 Target이 되면 true를 던진다.
	// Frame Active true
	
	// 이전에, 몬스터의 레벨 정보와 속성은 Elemental에 세팅한다.
	// 몬스터의 이름은 HPBar Background에 세팅한다.
	// Tick에 MonsterHPBar의 Active가 true면 현재의 체력을 계속 받아올 수 있도록 설정한다.
	
	if (bOnOff)
	{
		if (ELEMENTAL_TYPE::ELEMENTAL_END == eType)
			return E_FAIL;

		m_pMonsterHPBack->Set_Active(true);
		m_pMonsterHPBar->Set_Active(true);
		m_pMonsterFrame->Set_Active(true);
		m_pMonsterElemental->Set_Active(true);
	}
	else
	{
		m_pMonsterHPBack->Set_Active(false);
		m_pMonsterHPBar->Set_Active(false);
		m_pMonsterFrame->Set_Active(false);
		m_pMonsterElemental->Set_Active(false);
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
			Off_OtherSubBtn(0);
			for (auto& iter : m_SubMenuChar)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			m_MenuSeparator[0]->Set_Active(true); // 왼쪽구분선이 활성화된다.
			break;
		case 1: // 장비창을 선택했다.
			Off_OtherSubBtn(1);
			for (auto& iter : m_SubMenuEquip)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			m_MenuSeparator[0]->Set_Active(true); // 왼쪽구분선이 활성화된다.
			break;
		case 2: // 이마진창을 선택했다.
			Off_OtherSubBtn(2);
			for (auto& iter : m_SubMenuPet)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			m_MenuSeparator[0]->Set_Active(true); // 왼쪽구분선이 활성화된다.
			break;
		case 9: // 상점창을 선택했다.
			Off_OtherSubBtn(9);
			for (auto& iter : m_SubMenuShop)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			m_MenuSeparator[1]->Set_Active(true); // 오른쪽구분선이 활성화된다.
			break;
		}
	}
	else // off
	{

	}

	return S_OK;
}

HRESULT CUI_Manager::Off_OtherSubBtn(_uint iMagicNum)
{
	// 내가 누른 버튼 외에 다른 버튼이 활성화 되어있다면 꺼버린다.
	// 0 : Char, 1 : Equip, 2 : Pet, 9 : Shop -> Enum값 기준

	if (0 == iMagicNum)
	{
		for (auto& iter : m_SubMenuEquip)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		for (auto& iter : m_SubMenuPet)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		for (auto& iter : m_SubMenuShop)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		// 오른쪽 구분선이 활성화 되어있다면 끈다
		if (m_MenuSeparator[1]->Get_Active())
			m_MenuSeparator[1]->Set_Active(false);
	}
	else if (1 == iMagicNum)
	{
		for (auto& iter : m_SubMenuChar)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		for (auto& iter : m_SubMenuPet)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		for (auto& iter : m_SubMenuShop)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		// 오른쪽 구분선이 활성화 되어있다면 끈다
		if (m_MenuSeparator[1]->Get_Active())
			m_MenuSeparator[1]->Set_Active(false);
	}
	else if (2 == iMagicNum)
	{
		for (auto& iter : m_SubMenuChar)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		for (auto& iter : m_SubMenuEquip)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		for (auto& iter : m_SubMenuShop)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		// 오른쪽 구분선이 활성화 되어있다면 끈다
		if (m_MenuSeparator[1]->Get_Active())
			m_MenuSeparator[1]->Set_Active(false);
	}
	else if (9 == iMagicNum)
	{
		for (auto& iter : m_SubMenuChar)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		for (auto& iter : m_SubMenuEquip)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		for (auto& iter : m_SubMenuPet)
		{
			if (iter->Get_Active())
				iter->Set_Active(false);
		}
		// 왼쪽 구분선이 활성화 되어있다면 끈다
		if (m_MenuSeparator[0]->Get_Active())
			m_MenuSeparator[0]->Set_Active(false);
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_DialogWindow(_bool bOnOff, _uint iMagicNum)
{
	if (iMagicNum == 0) // Normal
	{
		if (nullptr == m_pDialogWindow)
			return E_FAIL;

		if (bOnOff) // On
		{
			if (!m_pDialogWindow->Get_Active())
			{
				m_pDialogWindow->Set_Active(true);
				OnOff_GamePlaySetting(false);
			}

		}
		else // Off
		{
			if (m_pDialogWindow->Get_Active())
			{
				m_pDialogWindow->Set_Active(false);
				OnOff_GamePlaySetting(true);
			}
		}
	}
	else if (iMagicNum == 1) // Mini
	{
		if (nullptr == m_pDialogMini)
			return E_FAIL;

		if (bOnOff) // On
		{
			if (!m_pDialogMini->Get_Active())
			{
				m_pDialogMini->Set_Active(true);

				for (auto& pPortrait : m_Portrait)
					pPortrait->Set_Active(true);
			}
		}
		else
		{
			if (m_pDialogMini->Get_Active())
			{
				m_pDialogMini->Set_Active(false);

				for (auto& pPortrait : m_Portrait)
					pPortrait->Set_Active(false);
			}
		}
	}

	else
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Manager::OnOff_WorldMap(_bool bOnOff)
{
	if (bOnOff) // 켠다
	{
		if (!m_pWorldMapBG->Get_Active())
		{
			// 기본 세팅을 끄고 WorldMap을 켠다
			OnOff_GamePlaySetting(false);

			m_pWorldMapBG->Set_Active(true);
			m_pTabMenuTitle->Set_TextType(CUI_Text_TabMenu::UI_MENUTITLE::TITLE_WORLDMAP);
			m_pTabMenuTitle->Set_Active(true);
		}
	}
	else // 끈다
	{
		if (m_pWorldMapBG->Get_Active())
		{
			// WorldMap을 끄고 기본 세팅을 켠다
			m_pWorldMapBG->Set_Active(false);
			m_pTabMenuTitle->Set_Active(false);

			OnOff_GamePlaySetting(true);
		}
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_CostumeWindow(_bool bOnOff)
{
	if (bOnOff)
	{
		// 메뉴 창을 끈다.
		OnOff_MainMenu(false);

		// Default Window를 활성화한다
		m_pDefaultBG->Set_Active(true);
		// Costume관련된 객체들을 활성화한다.
	}
	else
	{

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

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_CharacterLogo"),
		CUI_Loading_CharacterLogo::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_Progress_Back"),
		CUI_Loading_ProgressBar::Create(m_pDevice, m_pContext, CUI_Loading_ProgressBar::UIPROG_BACK), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_Progress_Bar"),
		CUI_Loading_ProgressBar::Create(m_pDevice, m_pContext, CUI_Loading_ProgressBar::UIPROG_BAR), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_Information"),
		CUI_Loading_Logo::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_MainLogo_Text"),
		CUI_Loading_MainLogo::Create(m_pDevice, m_pContext, CUI_Loading_MainLogo::MAINLOGO_TEXT), LAYER_UI)))
		return E_FAIL;
//	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_MainLogo_Number"),
//		CUI_Loading_MainLogo::Create(m_pDevice, m_pContext, CUI_Loading_MainLogo::MAINLOGO_NUM), LAYER_UI)))
//		return E_FAIL;
//
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_Imajinn"),
		CUI_Loading_Imajinn::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Fade_Black"),
		CUI_Fade::Create(m_pDevice, m_pContext, CUI_Fade::UI_VEIL::VEIL_BLACK), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Veil"),
		CUI_Veil::Create(m_pDevice, m_pContext), LAYER_UI)))
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

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Common_MainMenu_Separator"),
		CUI_MenuSeparator::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_Inventory"),
		CUI_SubMenu_Equipment::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Equipment_Inventory",
			CUI_SubMenu_Equipment::UI_SUBEQUIP_TYPE::EQUIPBTN_INVENTORY), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_Forge"),
		CUI_SubMenu_Equipment::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Equipment_Forge",
			CUI_SubMenu_Equipment::UI_SUBEQUIP_TYPE::EQUIPBTN_FORGE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_Gem"),
		CUI_SubMenu_Equipment::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Equipment_Gem",
			CUI_SubMenu_Equipment::UI_SUBEQUIP_TYPE::EQUIPBTN_GEM), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_TinyGenie"),
		CUI_SubMenu_Equipment::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Equipment_TinyGenie",
			CUI_SubMenu_Equipment::UI_SUBEQUIP_TYPE::EQUIPBTN_TINYGENIE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_Transcend"),
		CUI_SubMenu_Equipment::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Equipment_Transcend",
			CUI_SubMenu_Equipment::UI_SUBEQUIP_TYPE::EQUIPBTN_TRANSCEND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Equipment_FishingRod"),
		CUI_SubMenu_Equipment::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Equipment_FishingRod",
			CUI_SubMenu_Equipment::UI_SUBEQUIP_TYPE::EQUIPBTN_FISHINGROD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Imajinn"),
		CUI_SubMenu_Imajinn::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Imajinn_Imajinn",
			CUI_SubMenu_Imajinn::UI_SUBIMAJINN_TYPE::PETBTN_IMAJINN), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Training"),
		CUI_SubMenu_Imajinn::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Imajinn_Training",
			CUI_SubMenu_Imajinn::UI_SUBIMAJINN_TYPE::PETBTN_TRAINING), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Toy"),
		CUI_SubMenu_Imajinn::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Imajinn_Toy",
			CUI_SubMenu_Imajinn::UI_SUBIMAJINN_TYPE::PETBTN_TOY), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Cradle"),
		CUI_SubMenu_Imajinn::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Imajinn_Cradle",
			CUI_SubMenu_Imajinn::UI_SUBIMAJINN_TYPE::PETBTN_CRADLE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Return"),
		CUI_SubMenu_Imajinn::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Imajinn_Cradle",
			CUI_SubMenu_Imajinn::UI_SUBIMAJINN_TYPE::PETBTN_RETURN), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Dungeon"),
		CUI_SubMenu_Imajinn::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Imajinn_Dungeon",
			CUI_SubMenu_Imajinn::UI_SUBIMAJINN_TYPE::PETBTN_DUNGEON), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Unitetree"),
		CUI_SubMenu_Imajinn::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Imajinn_Unitetree",
			CUI_SubMenu_Imajinn::UI_SUBIMAJINN_TYPE::PETBTN_UNITETREE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Imajinn_Ultimate"),
		CUI_SubMenu_Imajinn::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Imajinn_Ultimate",
			CUI_SubMenu_Imajinn::UI_SUBIMAJINN_TYPE::PETBTN_ULTIMATE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Shop_Package"),
		CUI_SubMenu_Shop::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Shop_Package",
			CUI_SubMenu_Shop::UI_SUBSHOP_TYPE::SHOPBTN_PACKAGE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Shop_Gacha"),
		CUI_SubMenu_Shop::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Shop_Gacha",
			CUI_SubMenu_Shop::UI_SUBSHOP_TYPE::SHOPBTN_GACHA), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SubMenu_Btn_Shop_Token"),
		CUI_SubMenu_Shop::Create(m_pDevice, m_pContext, L"UI_SubMenu_Btn_Shop_Token",
			CUI_SubMenu_Shop::UI_SUBSHOP_TYPE::SHOPBTN_TOKEN), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WorldMap_Background"),
		CUI_WindowWorldMap::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WorldMap_Btn_Evermore"),
		CUI_Btn_WorldMapIcon::Create(m_pDevice, m_pContext, L"UI_WorldMap_Icon_Evermore", CUI_Btn_WorldMapIcon::ICON_EVERMORE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WorldMap_Btn_IceLand"),
		CUI_Btn_WorldMapIcon::Create(m_pDevice, m_pContext, L"UI_WorldMap_Icon_IceLand", CUI_Btn_WorldMapIcon::ICON_ICELAND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WorldMap_Btn_WitchForest"),
		CUI_Btn_WorldMapIcon::Create(m_pDevice, m_pContext, L"UI_WorldMap_Icon_WitchForest", CUI_Btn_WorldMapIcon::ICON_WITCHFOREST), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Player_HPBar"),
		CUI_PlayerHPBar::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_Background"),
		CUI_SkillSection_Background::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_BtnJump"),
		CUI_SkillSection_BtnJump::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_BtnRoll"),
		CUI_SkillSection_BtnRoll::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_ImajinnSection_Background"),
		CUI_ImajinnSection_Background::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_ImajinnSection_Slot_First"),
		CUI_ImajinnSection_Slot::Create(m_pDevice, m_pContext, CUI_ImajinnSection_Slot::IMAJINNSLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_ImajinnSection_Slot_Second"),
		CUI_ImajinnSection_Slot::Create(m_pDevice, m_pContext, CUI_ImajinnSection_Slot::IMAJINNSLOT_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_ImajinnSection_Slot_Third"),
		CUI_ImajinnSection_Slot::Create(m_pDevice, m_pContext, CUI_ImajinnSection_Slot::IMAJINNSLOT_THIRD), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_ImajinnSection_Emoticon"),
		CUI_ImajinnSection_Emoticon::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_ImajinnSection_Vehicle"),
		CUI_ImajinnSection_Vehicle::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Common_Default_Background"),
		CUI_Default_Background::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Common_Default_BackStars"),
		CUI_Default_BackStars::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	// 무기 슬롯은 Skill Section BG의 자식으로 Clone된다.
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSection_Slot_First"),
		CUI_WeaponSection_Slot::Create(m_pDevice, m_pContext, CUI_WeaponSection_Slot::WEAPONSLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSection_Slot_Second"),
		CUI_WeaponSection_Slot::Create(m_pDevice, m_pContext, CUI_WeaponSection_Slot::WEAPONSLOT_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSection_Slot_Third"),
		CUI_WeaponSection_Slot::Create(m_pDevice, m_pContext, CUI_WeaponSection_Slot::WEAPONSLOT_THIRD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_DefaultAttack"),
		CUI_WeaponSection_DefaultWeapon::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Icon_ShowSetting"),
		CUI_BtnShowSetting::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	// PlayerStatus
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_PlayerStatus_EXPBackground"),
		CUI_PlayerEXPBar::Create(m_pDevice, m_pContext, CUI_PlayerEXPBar::UIEXP_BACKGROUND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_PlayerStatus_EXPBar"),
		CUI_PlayerEXPBar::Create(m_pDevice, m_pContext, CUI_PlayerEXPBar::UIEXP_BAR), LAYER_UI)))
		return E_FAIL;

	// Monster HPBar
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MonsterHP_Background"),
		CUI_MonsterHP_Background::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MonsterHP_Bar"),
		CUI_MonsterHP_Bar::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MonsterHP_Elemental_Frame"),
		CUI_MonsterHP_ElementalFrame::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MonsterHP_Elemental"),
		CUI_MonsterHP_Elemental::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_BtnInteraction"),
		CUI_SkillSection_BtnInteraction::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_Interaction"),
		CUI_SkillSection_Interaction::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Common_TitleLine"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Common_TitleLine", CUI_Basic::UI_BASIC::UISTATIC_TITLELINE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialog_DefaultWindow"),
		CUI_Dialog_Window::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialog_MiniWindow"),
		CUI_Dialog_MiniWindow::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialog_FlipArrow"),
		CUI_Dialog_Flip::Create(m_pDevice, m_pContext, CUI_Dialog_Flip::UIFLIP_ARROW), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialog_FlipButton"),
		CUI_Dialog_Flip::Create(m_pDevice, m_pContext, CUI_Dialog_Flip::UIFLIP_BUTTON), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Common_TabMenuTitle"),
		CUI_Text_TabMenu::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialog_PortraitFrame"),
		CUI_Dialog_Portrait::Create(m_pDevice, m_pContext, CUI_Dialog_Portrait::UI_PORTRAIT::PORTRAIT_FRAME), LAYER_UI)))
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
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Common_MenuDeco_Right"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Common_MenuDeco_Right", CUI_Basic::UI_BASIC::UISTATIC_MENUDECO_R), LAYER_UI)))
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

	// SettingWindow
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Setting_Background"),
		CUI_Setting_Window::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Icon_Setting_Game"),
		CUI_Setting_Icon::Create(m_pDevice, m_pContext, CUI_Setting_Icon::UI_SETTING_ICONTYPE::SETICON_GAME), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Icon_Setting_Graphic"),
		CUI_Setting_Icon::Create(m_pDevice, m_pContext, CUI_Setting_Icon::UI_SETTING_ICONTYPE::SETICON_GRAPHIC), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Icon_Setting_Audio"),
		CUI_Setting_Icon::Create(m_pDevice, m_pContext, CUI_Setting_Icon::UI_SETTING_ICONTYPE::SETICON_AUDIO), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Icon_Setting_Slider_First"),
		CUI_Setting_Slider::Create(m_pDevice, m_pContext, CUI_Setting_Slider::UI_SETTING_SLIDERTYPE::FIRST_SLIDER), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Icon_Setting_Slider_Second"),
		CUI_Setting_Slider::Create(m_pDevice, m_pContext, CUI_Setting_Slider::UI_SETTING_SLIDERTYPE::SECOND_SLIDER), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Icon_Setting_Slider_Third"),
		CUI_Setting_Slider::Create(m_pDevice, m_pContext, CUI_Setting_Slider::UI_SETTING_SLIDERTYPE::THIRD_SLIDER), LAYER_UI)))
		return E_FAIL;
	
	//Audio Section
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Setting_Section_Audio"),
		CUI_Setting_Section::Create(m_pDevice, m_pContext, L"UI_Setting_Section_Audio", CUI_Setting_Section::UI_SETTING_SECTION::SECTION_AUDIO), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Setting_BtnVolume_Minus"),
		CUI_Setting_BtnVolume::Create(m_pDevice, m_pContext, CUI_Setting_BtnVolume::UI_SETTING_BTNTYPE::SETBTN_MINUS), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Setting_BtnVolume_Plus"),
		CUI_Setting_BtnVolume::Create(m_pDevice, m_pContext, CUI_Setting_BtnVolume::UI_SETTING_BTNTYPE::SETBTN_PLUS), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Btn_Back"),
		CUI_BtnBack::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DefaultBackground_Cloud"),
		CUI_Default_BackCloud::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;


	return S_OK;
}

#pragma endregion


void CUI_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDefaultBG);
	Safe_Release(m_pUICursor);
	Safe_Release(m_pUIVeil);
	Safe_Release(m_pUIFade);
	Safe_Release(m_pUIMapName);
	Safe_Release(m_pMapText);

	Safe_Release(m_pSettingBG);

	Safe_Release(m_pPlayerStatus);

	Safe_Release(m_pBtnShowMenu);
	Safe_Release(m_pBtnCamera);
	Safe_Release(m_pBtnInven);
	Safe_Release(m_pBtnQuest);
	Safe_Release(m_pBtnClose);
	Safe_Release(m_pBtnShowSetting);

	Safe_Release(m_pMainBG);
	Safe_Release(m_pWindowQuest);

	Safe_Release(m_pWorldMapBG);
	Safe_Release(m_pSkillBG);
	Safe_Release(m_pImajinnBG);

	Safe_Release(m_pMonsterHPBar);
	Safe_Release(m_pMonsterHPBack);
	Safe_Release(m_pMonsterFrame);
	Safe_Release(m_pMonsterElemental);

	Safe_Release(m_pDialogWindow);
	Safe_Release(m_pDialogMini);

	Safe_Release(m_pTabMenuTitle);

	for (auto& pBasic : m_Basic)
		Safe_Release(pBasic);
	m_Basic.clear();

	for (auto& pButton : m_Buttons)
		Safe_Release(pButton);
	m_Buttons.clear();

	for (auto& pLevelUp : m_LevelUp)
		Safe_Release(pLevelUp);
	m_LevelUp.clear();

	for (auto& pUnclickedBtn : m_UnclickedPlayer)
		Safe_Release(pUnclickedBtn);
	m_UnclickedPlayer.clear();

	for (auto& pClickedBtn : m_ClickedPlayer)
		Safe_Release(pClickedBtn);
	m_ClickedPlayer.clear();

	for (auto& pMainBtn : m_MainMenuBtn)
		Safe_Release(pMainBtn);
	m_MainMenuBtn.clear();

	for (auto& pSeparator : m_MenuSeparator)
		Safe_Release(pSeparator);
	m_MenuSeparator.clear();

	for (auto& pCharBtn : m_SubMenuChar)
		Safe_Release(pCharBtn);
	m_SubMenuChar.clear();

	for (auto& pEquipBtn : m_SubMenuEquip)
		Safe_Release(pEquipBtn);
	m_SubMenuEquip.clear();

	for (auto& pPetBtn : m_SubMenuPet)
		Safe_Release(pPetBtn);
	m_SubMenuPet.clear();

	for (auto& pShopBtn : m_SubMenuShop)
		Safe_Release(pShopBtn);
	m_SubMenuShop.clear();

	for (auto& pEXP : m_PlayerEXP)
		Safe_Release(pEXP);
	m_PlayerEXP.clear();

	for (auto& pPortrait : m_Portrait)
		Safe_Release(pPortrait);
	m_Portrait.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
