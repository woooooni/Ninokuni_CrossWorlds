#include "stdafx.h"
#include "..\Public\UI_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Effect.h"
#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"
#include "Character_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Monster.h"

#include "UI_Fade.h"
#include "UI_Veil.h"
#include "UI_Basic.h"
#include "UI_Cursor.h"
#include "UI_LevelUp.h"
#include "UI_MapName.h"
#include "UI_BtnBack.h"
#include "UI_Milepost.h"
#include "UI_BtnClose.h"
#include "UI_MainMenu.h"
#include "UI_Announced.h"
#include "UI_BtnAccept.h"
#include "UI_PlayerInfo.h"
#include "UI_BossHP_Bar.h"
#include "UI_PopupQuest.h"
#include "UI_PlayerHPBar.h"
#include "UI_BasicButton.h"
#include "UI_WindowQuest.h"
#include "UI_Dialog_Flip.h"
#include "UI_BtnShowMenu.h"
#include "UI_Costume_Btn.h"
#include "UI_Btn_Minimap.h"
#include "UI_Setting_Icon.h"
#include "UI_SubMenu_Shop.h"
#include "UI_Text_TabMenu.h"
#include "UI_BtnInventory.h"
#include "UI_PlayerEXPBar.h"
#include "UI_MonsterHP_Bar.h"
#include "UI_MenuSeparator.h"
#include "UI_Dialog_Window.h"
#include "UI_BtnQuickQuest.h"
#include "UI_Inventory_Slot.h"
#include "UI_Setting_Slider.h"
#include "UI_Setting_Window.h"
#include "UI_BtnShowSetting.h"
#include "UI_WindowWorldMap.h"
#include "UI_QuickSlot_Item.h"
#include "UI_CharacterDummy.h"
#include "UI_Costume_LineBox.h"
#include "UI_MonsterHP_World.h"
#include "UI_Dialog_Portrait.h"
#include "UI_Loading_Imajinn.h"
#include "UI_Setting_Section.h"
#include "UI_SubMenu_Imajinn.h"
#include "UI_Emoticon_Window.h"
#include "UI_SkillWindow_Btn.h"
#include "UI_Emoticon_Button.h"
#include "UI_BtnChangeCamera.h"
#include "UI_Inventory_TabBtn.h"
#include "UI_Costume_ItemSlot.h"
#include "UI_Loading_MainLogo.h"
#include "UI_Btn_WorldMapIcon.h"
#include "UI_BossHP_Background.h"
#include "UI_Inventory_LineBox.h"
#include "UI_Dialog_MiniWindow.h"
#include "UI_Default_BackCloud.h"
#include "UI_Loading_Character.h"
#include "UI_SubMenu_Character.h"
#include "UI_SubMenu_Equipment.h"
#include "UI_Setting_BtnVolume.h"
#include "UI_Default_BackStars.h"
#include "UI_Costume_ChangeBtn.h"
#include "UI_SkillSection_Frame.h"
#include "UI_Default_Background.h"
#include "UI_BtnCharacterSelect.h"
#include "UI_SetNickname_Window.h"
#include "UI_Loading_Background.h"
#include "UI_WeaponSection_Slot.h"
#include "UI_SetNickname_Textbox.h"
#include "UI_SkillWindow_LineBox.h"
#include "UI_Loading_ProgressBar.h"
#include "UI_Loading_Information.h"
#include "UI_MonsterHP_Elemental.h"
#include "UI_ImajinnSection_Slot.h"
#include "UI_SkillSection_BtnRoll.h"
#include "UI_SkillSection_BtnJump.h"
#include "UI_MonsterHP_Background.h"
#include "UI_SkillWindow_SkillSlot.h"
#include "UI_Loading_CharacterLogo.h"
#include "UI_ImajinnSection_Vehicle.h"
#include "UI_Emoticon_SpeechBalloon.h"
#include "UI_SkillSection_Background.h"
#include "UI_ImajinnSection_Emoticon.h"
#include "UI_Emoticon_BalloonEmoticon.h"
#include "UI_MonsterHP_ElementalFrame.h"
#include "UI_SkillSection_Interaction.h"
#include "UI_SkillSection_ClassicSkill.h"
#include "UI_ImajinnSection_Background.h"
#include "UI_SkillSection_SpecialSkill.h"
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

void CUI_Manager::Set_RandomNick(const wstring& strRandom)
{
	if (LEVELID::LEVEL_LOBBY != GI->Get_CurrentLevel())
		return;

	if (nullptr == m_pNicknamebox || !m_pNicknamebox->Get_Active())
		return;

	m_strNickname = strRandom;
	m_pNicknamebox->Set_Nickname(m_strNickname);
}

void CUI_Manager::Set_UserName()
{
	CGame_Manager::GetInstance()->Set_UserName(m_strNickname);
}

void CUI_Manager::Set_MonsterDescForUI(CMonster* pOwner, void* pArg, _bool bActive)
{
	// 몬스터의 정보를 받아 상단 HPBar UI를 세팅한다.
	// 플레이어의 타겟이 있는지 확인하고 nullptr이면 Active False 후 return;

	if (nullptr == pArg || nullptr == pOwner)
		return;

	m_pHPBarOwner = pOwner;

	CMonster::MONSTER_STAT StatDesc = {};
	ZeroMemory(&StatDesc, sizeof(CMonster::MONSTER_STAT));

	memcpy(&StatDesc, &(m_pHPBarOwner->Get_Stat()), sizeof(CMonster::MONSTER_STAT));

//	m_fCurHP = StatDesc.fHp;
//	m_fMaxHP = StatDesc.fMaxHp;
//
//	m_strName = m_pOwner->Get_KorName();


	// 몬스터가 Target이 되면 true를 던진다.
	// Frame Active true

	// 이전에, 몬스터의 레벨 정보와 속성은 Elemental에 세팅한다.
	// 몬스터의 이름은 HPBar Background에 세팅한다.
	// Tick에 MonsterHPBar의 Active가 true면 현재의 체력을 계속 받아올 수 있도록 설정한다.

	if (bActive)
	{
//		if (ELEMENTAL_TYPE::ELEMENTAL_END == eType)
//			return E_FAIL;

		// m_pMonsterElemental->Set_ElementalType();
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
}

_float2 CUI_Manager::Get_ProjectionPosition(CTransform* pTransform)
{
	if (nullptr == pTransform)
		return _float2(-1.f, -1.f);

	_vector vPosition = pTransform->Get_State(CTransform::STATE_POSITION);

	_float4x4 matWorld = pTransform->Get_WorldFloat4x4();
	_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	_float4x4 matWindow;
	XMStoreFloat4x4(&matWindow, XMLoadFloat4x4(&matWorld) * matView * matProj);

	_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];

	vWindowPos.x /= vWindowPos.z;
	vWindowPos.y /= vWindowPos.z;
	_float fScreenX = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
	_float fScreenY = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);

	return _float2(fScreenX, fScreenY);
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
	if (nullptr == m_pUIFade)
	{
		CGameObject* pVeil = nullptr;

		CUI::UI_INFO UIDesc = {};
		ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

		UIDesc.fCX = g_iWinSizeX;
		UIDesc.fCY = g_iWinSizeY;
		UIDesc.fX = g_iWinSizeX * 0.5f;
		UIDesc.fY = g_iWinSizeY * 0.5f;

		if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_STATIC, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Fade"), &UIDesc, &pVeil)))
			return E_FAIL;

		m_pUIFade = dynamic_cast<CUI_Fade*>(pVeil);
		if (nullptr == m_pUIFade)
			return E_FAIL;

		Safe_AddRef(m_pUIFade);
	}

	return S_OK;
}

HRESULT CUI_Manager::Ready_Dummy()
{
	CGameObject* pDummy = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_CharacterDummy"), LAYER_TYPE::LAYER_CHARACTER);
	if (nullptr == pDummy)
		return E_FAIL;

	m_pDummy = dynamic_cast<CUI_CharacterDummy*>(pDummy);
	//Safe_AddRef(m_pDummy);

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
	if (nullptr == m_pUIVeil)
	{
		if (FAILED(Ready_Veils()))
			return E_FAIL;
	}

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
	_float2 vOffset = _float2(90.f, 3.f);

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
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 128.f * 0.6f;
	UIDesc.fCY = 128.f * 0.55f;
	UIDesc.fX = UIDesc.fCX * 0.5f + 3.f;
	UIDesc.fY = UIDesc.fCY * 0.5f;

	CGameObject* pBack = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_Btn_Back"), &UIDesc, &pBack)))
		return E_FAIL;
	if (nullptr == pBack)
		return E_FAIL;

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

	// For SetNickname Window
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	pBG = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_SetNickname_Window"), &UIDesc, &pBG)))
		return E_FAIL;
	m_pSetNickBG = dynamic_cast<CUI_SetNickname_Window*>(pBG);
	if (nullptr == m_pSetNickBG)
		return E_FAIL;
	Safe_AddRef(m_pSetNickBG);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 420.f;
	UIDesc.fCY = 52.f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;

	pBG = nullptr;
	if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_LOBBY, LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_SetNickname_Textbox"), &UIDesc, &pBG)))
		return E_FAIL;
	m_pNicknamebox = dynamic_cast<CUI_SetNickname_Textbox*>(pBG);
	if (nullptr == m_pNicknamebox)
		return E_FAIL;
	Safe_AddRef(m_pNicknamebox);

	return S_OK;
}

HRESULT CUI_Manager::Ready_CommonUIs(LEVELID eID)
{
	if(FAILED(Ready_Dummy()))
		return E_FAIL;

	if (FAILED(Ready_Veils()))
		return E_FAIL;

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

	UIDesc.fCX = 1000.f * 0.4f;
	UIDesc.fCY = 481.f * 0.4f;
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

//	UIDesc.fCX = 512.f * 0.6f;
//	UIDesc.fCY = 175.f * 0.57;
//	UIDesc.fX = UIDesc.fCX * 0.5f;
//	UIDesc.fY = UIDesc.fCY * 0.5f;

	UIDesc.fCX = 670.f * 0.55f;
	UIDesc.fCY = 175.f * 0.52;
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

	UIDesc.fCX = 64.f * 0.8f;
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

	UIDesc.fCX = 64.f * 0.8f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = UIDesc.fCX * 0.5f + (fOffset * 2.f);
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f) - fOffset;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Button_ChangeCamera"), &UIDesc, &pButton)))
		return E_FAIL;

	m_pBtnCamera = dynamic_cast<CUI_BtnChangeCamera*>(pButton);
	if (nullptr == m_pBtnCamera)
		return E_FAIL;

	Safe_AddRef(m_pBtnCamera);


	// Minimap 활성화 버튼 생성
	pButton = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 64.f * 0.8f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f) - fOffset - 5.f;
	UIDesc.fY = (UIDesc.fCY * 1.5f) + fOffset;

	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Common_Btn_ShowMinimap"), &UIDesc, &pButton)))
		return E_FAIL;

	m_pBtnShowMinimap = dynamic_cast<CUI_Btn_Minimap*>(pButton);
	if (nullptr == m_pBtnShowMinimap)
		return E_FAIL;

	Safe_AddRef(m_pBtnShowMinimap);

	// MinimapBtn 생성
	pButton = nullptr;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 64.f * 0.8f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f) - (fOffset * 6.f);
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

	UIDesc.fCX = 64.f * 0.8;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = UIDesc.fCX * 0.5f + (fOffset * 2.f);
	UIDesc.fY = 135.f;

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

	UIDesc.fCX = 64.f * 0.5f;
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

	UIDesc.fCX = 400.f * 0.75f;
	UIDesc.fCY = 400.f * 0.75f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f;
	UIDesc.fY = g_iWinSizeY - UIDesc.fCY * 0.5f - 5.f;

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Background"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pSkillBG = dynamic_cast<CUI_SkillSection_Background*>(pBackground);
	if (nullptr == m_pSkillBG)
		return E_FAIL;
	Safe_AddRef(m_pSkillBG);

	// Imajinn Section
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 540.f * 0.6f;
	UIDesc.fCY = 172.f * 0.55f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY - UIDesc.fCY * 0.5f - 15.f;

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
	UIDesc.fCX = 300.f * 0.65f;
	UIDesc.fCY = 50.f * 0.65f;
	UIDesc.fX = g_iWinSizeX * 0.5f + 11.f;
	UIDesc.fY = UIDesc.fCY * 0.5f + 25.f;

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
	UIDesc.fCX = 128.f * 0.45f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX * 0.5f - 119.f;
	UIDesc.fY = UIDesc.fCY * 0.5f + 10.f;

	pBackground = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MonsterHP_Elemental_Frame"), &UIDesc, &pBackground)))
		return E_FAIL;
	m_pMonsterFrame = dynamic_cast<CUI_MonsterHP_ElementalFrame*>(pBackground);
	if (nullptr == m_pMonsterFrame)
		return E_FAIL;
	Safe_AddRef(m_pMonsterFrame);

	UIDesc.fCX = 64.f * 0.45f;
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
	fOffset = 115.f;
	UIDesc.fCX = 820.f * 0.35f;
	UIDesc.fCY = 348.f * 0.4f;
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
	fOffset = 83.f;
	UIDesc.fCX = 256.f * 0.8f;
	UIDesc.fCY = 256.f * 0.8f;
	UIDesc.fX = g_iWinSizeX * 0.5f - 130.f;
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

#pragma region COSTUME_TAB

	m_CostumeBtn.reserve(6);
	m_CostumeClickedBtn.reserve(6);

	CUI::UI_INFO ClickedDesc = {};
	ZeroMemory(&ClickedDesc, sizeof(CUI::UI_INFO));

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 250.f * 0.7f;
	UIDesc.fCY = 140.f * 0.7f;
	UIDesc.fX = 130.f;
	UIDesc.fY = 250.f;

	CGameObject* pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_Cloth"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_CostumeBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	ClickedDesc.fCX = 128.f * 0.7f;
	ClickedDesc.fCY = 140.f * 0.7f;
	ClickedDesc.fX = UIDesc.fX - 48.f;
	ClickedDesc.fY = UIDesc.fY;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_Cloth"), &ClickedDesc, &pBtn)))
		return E_FAIL;
	m_CostumeClickedBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	fBtnOffset = UIDesc.fCY - 11.f;
	UIDesc.fY += fBtnOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_Hair"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_CostumeBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	ClickedDesc.fY = UIDesc.fY;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_Hair"), &ClickedDesc, &pBtn)))
		return E_FAIL;
	m_CostumeClickedBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fY += fBtnOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_HairAcc"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_CostumeBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	ClickedDesc.fY = UIDesc.fY;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_HairAcc"), &ClickedDesc, &pBtn)))
		return E_FAIL;
	m_CostumeClickedBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fY += fBtnOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_ExteriorDeco"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_CostumeBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	ClickedDesc.fY = UIDesc.fY;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_ExteriorDeco"), &ClickedDesc, &pBtn)))
		return E_FAIL;
	m_CostumeClickedBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fY += fBtnOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_FaceDeco"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_CostumeBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	ClickedDesc.fY = UIDesc.fY;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_FaceDeco"), &ClickedDesc, &pBtn)))
		return E_FAIL;
	m_CostumeClickedBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fY += (fBtnOffset + 8.f);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_Weapon"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_CostumeBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	ClickedDesc.fY = UIDesc.fY - 5.f;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_Weapon"), &ClickedDesc, &pBtn)))
		return E_FAIL;
	m_CostumeClickedBtn.push_back(dynamic_cast<CUI_Costume_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 307.f;
	UIDesc.fCY = 700.f;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f + 30.f);
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f + 70.f);

	CGameObject* pLineBox = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_LineBox"), &UIDesc, &pLineBox)))
		return E_FAIL;
	m_pCostumeBox = dynamic_cast<CUI_Costume_LineBox*>(pLineBox);
	if (nullptr == m_pCostumeBox)
		return E_FAIL;
	Safe_AddRef(m_pCostumeBox);

#pragma endregion

#pragma region CLASSIC_SKILLS

	m_ClassicSkill.reserve(3);
	m_ClassicFrame.reserve(3);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 256.f * 0.26f;
	UIDesc.fCY = 256.f * 0.26f;
	UIDesc.fX = g_iWinSizeX - 245.5f;
	UIDesc.fY = g_iWinSizeY - 158.f;

	CGameObject* pClassicSkill = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Classic_First"), &UIDesc, &pClassicSkill)))
		return E_FAIL;
	m_ClassicSkill.push_back(dynamic_cast<CUI_SkillSection_ClassicSkill*>(pClassicSkill));
	if (nullptr == pClassicSkill)
		return E_FAIL;
	Safe_AddRef(pClassicSkill);

	UIDesc.fCX = 256.f * 0.25f;
	UIDesc.fCY = 256.f * 0.25f;
	CGameObject* pClassicFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_ClassicSKill_Frame"), &UIDesc, &pClassicFrame)))
		return E_FAIL;
	m_ClassicFrame.push_back(dynamic_cast<CUI_SkillSection_Frame*>(pClassicFrame));
	if (nullptr == pClassicFrame)
		return E_FAIL;
	Safe_AddRef(pClassicFrame);

	UIDesc.fCX = 256.f * 0.26f;
	UIDesc.fCY = 256.f * 0.26f;
	UIDesc.fX = g_iWinSizeX - 220.5f;
	UIDesc.fY = g_iWinSizeY - 227.f;
	pClassicSkill = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Classic_Second"), &UIDesc, &pClassicSkill)))
		return E_FAIL;
	m_ClassicSkill.push_back(dynamic_cast<CUI_SkillSection_ClassicSkill*>(pClassicSkill));
	if (nullptr == pClassicSkill)
		return E_FAIL;
	Safe_AddRef(pClassicSkill);

	UIDesc.fCX = 256.f * 0.25f;
	UIDesc.fCY = 256.f * 0.25f;
	pClassicFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_ClassicSKill_Frame"), &UIDesc, &pClassicFrame)))
		return E_FAIL;
	m_ClassicFrame.push_back(dynamic_cast<CUI_SkillSection_Frame*>(pClassicFrame));
	if (nullptr == pClassicFrame)
		return E_FAIL;
	Safe_AddRef(pClassicFrame);

	UIDesc.fCX = 256.f * 0.26f;
	UIDesc.fCY = 256.f * 0.26f;
	UIDesc.fX = g_iWinSizeX - 153.f;
	UIDesc.fY = g_iWinSizeY - 251.f;
	pClassicSkill = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Classic_Third"), &UIDesc, &pClassicSkill)))
		return E_FAIL;
	m_ClassicSkill.push_back(dynamic_cast<CUI_SkillSection_ClassicSkill*>(pClassicSkill));
	if (nullptr == pClassicSkill)
		return E_FAIL;
	Safe_AddRef(pClassicSkill);

	UIDesc.fCX = 256.f * 0.25f;
	UIDesc.fCY = 256.f * 0.25f;
	pClassicFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_ClassicSKill_Frame"), &UIDesc, &pClassicFrame)))
		return E_FAIL;
	m_ClassicFrame.push_back(dynamic_cast<CUI_SkillSection_Frame*>(pClassicFrame));
	if (nullptr == pClassicFrame)
		return E_FAIL;
	Safe_AddRef(pClassicFrame);

#pragma endregion

#pragma region SPECIAL_SKILLS

	m_SpecialSkill.reserve(3);
	m_SpecialFrame.reserve(3);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 256.f * 0.24f;
	UIDesc.fCY = 256.f * 0.24f;
	UIDesc.fX = g_iWinSizeX - 147.f;
	UIDesc.fY = g_iWinSizeY - 60.f;

	CGameObject* pSpecialSkill = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Special_First"), &UIDesc, &pSpecialSkill)))
		return E_FAIL;
	m_SpecialSkill.push_back(dynamic_cast<CUI_SkillSection_SpecialSkill*>(pSpecialSkill));
	if (nullptr == pSpecialSkill)
		return E_FAIL;
	Safe_AddRef(pSpecialSkill);
	CGameObject* pSpecialFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SpecialSKill_Frame_First"), &UIDesc, &pSpecialFrame)))
		return E_FAIL;
	m_SpecialFrame.push_back(dynamic_cast<CUI_SkillSection_Frame*>(pSpecialFrame));
	if (nullptr == pSpecialFrame)
		return E_FAIL;
	Safe_AddRef(pSpecialFrame);

	UIDesc.fX = g_iWinSizeX - 81.f;
	UIDesc.fY = g_iWinSizeY - 89.f;
	pSpecialSkill = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Special_Second"), &UIDesc, &pSpecialSkill)))
		return E_FAIL;
	m_SpecialSkill.push_back(dynamic_cast<CUI_SkillSection_SpecialSkill*>(pSpecialSkill));
	if (nullptr == pSpecialSkill)
		return E_FAIL;
	Safe_AddRef(pSpecialSkill);
	pSpecialFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SpecialSKill_Frame_Second"), &UIDesc, &pSpecialFrame)))
		return E_FAIL;
	m_SpecialFrame.push_back(dynamic_cast<CUI_SkillSection_Frame*>(pSpecialFrame));
	if (nullptr == pSpecialFrame)
		return E_FAIL;
	Safe_AddRef(pSpecialFrame);

	UIDesc.fX = g_iWinSizeX - 56.f;
	UIDesc.fY = g_iWinSizeY - 153.f;
	pClassicSkill = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Special_Third"), &UIDesc, &pSpecialSkill)))
		return E_FAIL;
	m_SpecialSkill.push_back(dynamic_cast<CUI_SkillSection_SpecialSkill*>(pSpecialSkill));
	if (nullptr == pSpecialSkill)
		return E_FAIL;
	Safe_AddRef(pSpecialSkill);
	pSpecialFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SpecialSKill_Frame_Third"), &UIDesc, &pSpecialFrame)))
		return E_FAIL;
	m_SpecialFrame.push_back(dynamic_cast<CUI_SkillSection_Frame*>(pSpecialFrame));
	if (nullptr == pSpecialFrame)
		return E_FAIL;
	Safe_AddRef(pSpecialFrame);

#pragma endregion

#pragma region ITEM_QUICKSLOT

	m_ItemQuickslot.reserve(3);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 73.f * 0.6f;
	UIDesc.fCY = 73.f * 0.6f;
	UIDesc.fX = 380.f;
	UIDesc.fY = 43.f;

	fOffset = 45.f;

	CGameObject* pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Item_QuickSlot_First"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_ItemQuickslot.push_back(dynamic_cast<CUI_QuickSlot_Item*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);

	UIDesc.fX += fOffset;
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Item_QuickSlot_Second"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_ItemQuickslot.push_back(dynamic_cast<CUI_QuickSlot_Item*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);

	UIDesc.fX += fOffset;
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Item_QuickSlot_Third"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_ItemQuickslot.push_back(dynamic_cast<CUI_QuickSlot_Item*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);

#pragma endregion

#pragma region COSTUME_ITEMSLOT

	m_CostumeCloth.reserve(3);
	m_CostumeHairAcc.reserve(3);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 270.f;
	UIDesc.fCY = 77.f;
	UIDesc.fX = g_iWinSizeX - 184.f;
	UIDesc.fY = 210.f;

	fOffset = 77.f;

	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_CostumeSection_First"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_CostumeCloth.push_back(dynamic_cast<CUI_Costume_ItemSlot*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_HairAccSection_First"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_CostumeHairAcc.push_back(dynamic_cast<CUI_Costume_ItemSlot*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);

	UIDesc.fY += fOffset;
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_CostumeSection_Second"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_CostumeCloth.push_back(dynamic_cast<CUI_Costume_ItemSlot*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_HairAccSection_Second"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_CostumeHairAcc.push_back(dynamic_cast<CUI_Costume_ItemSlot*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);

	UIDesc.fY += fOffset;
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_CostumeSection_Third"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_CostumeCloth.push_back(dynamic_cast<CUI_Costume_ItemSlot*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_HairAccSection_Third"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_CostumeHairAcc.push_back(dynamic_cast<CUI_Costume_ItemSlot*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);

#pragma endregion

	//m_QuestPopUp
	m_QuestPopUp.reserve(3);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 300.f * 0.65f;
	UIDesc.fCY = 120.f * 0.6f;
	UIDesc.fX = 175.f;
	UIDesc.fY = 171.f;

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Window"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 300.f * 0.65f;
	UIDesc.fCY = 32.f * 0.65f;
	UIDesc.fX = 175.f;
	UIDesc.fY = 135.f;
	fOffset = 70.f;

	CGameObject* pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Top"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pFrame));
	if (nullptr == pFrame)
		return E_FAIL;
	Safe_AddRef(pFrame);

	UIDesc.fY += fOffset;

	pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Bottom"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pFrame));
	if (nullptr == pFrame)
		return E_FAIL;
	Safe_AddRef(pFrame);

	//Prototype_GameObject_UI_Announced_Camera

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 700.f * 0.7f;
	UIDesc.fCY = 75.f * 0.7f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = 135.f;

	CGameObject* pAnnounce = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Announced_Camera"), &UIDesc, &pAnnounce)))
		return E_FAIL;
	m_pCameraAnnounce = dynamic_cast<CUI_Announced*>(pAnnounce);
	if (nullptr == m_pCameraAnnounce)
		return E_FAIL;
	Safe_AddRef(m_pCameraAnnounce);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 540.f * 0.6f;
	UIDesc.fCY = 440.f * 0.6f;
	UIDesc.fX = g_iWinSizeX * 0.5f - 20.f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f) - 70.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Emoticon_Window"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_pEmoticonWindow = dynamic_cast<CUI_Emoticon_Window*>(pWindow);
	if (nullptr == m_pEmoticonWindow)
		return E_FAIL;
	Safe_AddRef(m_pEmoticonWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 128.f * 0.9f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Emoticon_SpeechBalloon"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_pBalloon = dynamic_cast<CUI_Emoticon_SpeechBalloon*>(pWindow);
	if (nullptr == m_pBalloon)
		return E_FAIL;
	Safe_AddRef(m_pBalloon);

#pragma region INVENTORY

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 445.f;
	UIDesc.fCY = 700.f;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f + 40.f);
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f + 40.f);
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_LineBox"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_pInvenBox = dynamic_cast<CUI_Inventory_LineBox*>(pWindow);
	if (nullptr == m_pInvenBox)
		return E_FAIL;
	Safe_AddRef(m_pInvenBox);

	m_InvenBtn.reserve(CUI_Inventory_TabBtn::UI_INVENTABBTN::INVENTABBTN_END);
	m_InvenClickedBtn.reserve(CUI_Inventory_TabBtn::UI_INVENTABBTN::INVENTABBTN_END);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 132.f * 0.52f;
	UIDesc.fCY = 85.f * 0.6f;
	UIDesc.fX = 1165.f;
	UIDesc.fY = 136.f;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_Weapon"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_Weapon"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenClickedBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fX += UIDesc.fCX;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_Armor"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_Armor"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenClickedBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fX += UIDesc.fCX;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_Acc"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_Acc"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenClickedBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fX += UIDesc.fCX;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_CrystalBall"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_CrystalBall"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenClickedBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fX += UIDesc.fCX;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_Etc1"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_Etc1"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenClickedBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fX += UIDesc.fCX;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_Etc2"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_Etc2"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenClickedBtn.push_back(dynamic_cast<CUI_Inventory_TabBtn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);


	m_InvenSlots.reserve(CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_END);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 128.f * 0.5f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = UIDesc.fCX;
	UIDesc.fY = 280.f;

	fOffset = UIDesc.fCX + 8.f;

	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Necklace"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	UIDesc.fX += fOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Sword1"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fX = UIDesc.fCX;
	UIDesc.fY = 280.f + fOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Earring"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	UIDesc.fX += fOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Sword2"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fX = UIDesc.fCX;
	UIDesc.fY = 280.f + (fOffset * 2.f);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Ring"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	UIDesc.fX += fOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Sword3"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fX = UIDesc.fCX;
	UIDesc.fY = 280.f + (fOffset * 3.f);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Helmet"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	UIDesc.fX += fOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Armor"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fX = UIDesc.fCX;
	UIDesc.fY = 280.f + (fOffset * 4.f);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Gloves"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	UIDesc.fX += fOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Boots"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	UIDesc.fX = UIDesc.fCX;
	UIDesc.fY = 280.f + (fOffset * 5.f);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Crystal1"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	UIDesc.fX += fOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Inventory_Slot_Crystal2"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_InvenSlots.push_back(dynamic_cast<CUI_Inventory_Slot*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

#pragma endregion

#pragma region SKILLWINDOW

	m_SkillWindow.reserve(4);
	m_SkillBtn.reserve(3);
	m_SkillClickedBtn.reserve(3);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 119.f;
	UIDesc.fCY = 818.f + 10.f;
	UIDesc.fX = UIDesc.fCX * 0.5f;
	UIDesc.fY = g_iWinSizeY - UIDesc.fCY * 0.5f;

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_SubMenuBg"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_SkillWindow.push_back(dynamic_cast<CUI_SkillWindow_LineBox*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 412.f * 0.9f;
	UIDesc.fCY = 900.f * 0.9f;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f + 12.f);
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f + 8.f);

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_LineBox"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_SkillWindow.push_back(dynamic_cast<CUI_SkillWindow_LineBox*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	fOffset = 100.f;

	UIDesc.fCX = 168.f * 0.55f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = 60.f;
	UIDesc.fY = 120.f; // 220

	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_Unclicked_ClassicBtn"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_SkillBtn.push_back(dynamic_cast<CUI_SkillWindow_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_Clicked_ClassicBtn"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_SkillClickedBtn.push_back(dynamic_cast<CUI_SkillWindow_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);


	UIDesc.fY += fOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_Unclicked_ActiveBtn"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_SkillBtn.push_back(dynamic_cast<CUI_SkillWindow_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_Clicked_ActiveBtn"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_SkillClickedBtn.push_back(dynamic_cast<CUI_SkillWindow_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);


	UIDesc.fY += fOffset;
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_Unclicked_PassiveBtn"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_SkillBtn.push_back(dynamic_cast<CUI_SkillWindow_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);
	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_Clicked_PassiveBtn"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_SkillClickedBtn.push_back(dynamic_cast<CUI_SkillWindow_Btn*>(pBtn));
	if (nullptr == pBtn)
		return E_FAIL;
	Safe_AddRef(pBtn);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 32.f;
	UIDesc.fCY = 37.f;
	UIDesc.fX = 100.f + UIDesc.fCX * 0.5f;
	UIDesc.fY = 120.f;

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_Select_Arrow"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_SkillWindow.push_back(dynamic_cast<CUI_SkillWindow_LineBox*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 250.f * 0.7f;
	UIDesc.fCY = 53.f * 0.7f;
	UIDesc.fX = 80.f + UIDesc.fCX * 0.5f;
	UIDesc.fY = 36.f;

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_Title"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_SkillWindow.push_back(dynamic_cast<CUI_SkillWindow_LineBox*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);


	m_ClassSkillSlot.reserve(5); // Class 3개, Separ~ 1개, Burst 1개
	m_SpecialSkillSlot.reserve(3); // 3개

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	fOffset = 102.f;

	UIDesc.fCX = 380.f * 0.9f;
	UIDesc.fCY = 125.f * 0.9f;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f + 25.f);
	UIDesc.fY = 180.f;

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_ClassSkill_First"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_ClassSkillSlot.push_back(dynamic_cast<CUI_SkillWindow_SkillSlot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	UIDesc.fY += fOffset;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_ClassSkill_Second"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_ClassSkillSlot.push_back(dynamic_cast<CUI_SkillWindow_SkillSlot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	UIDesc.fY += fOffset;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_ClassSkill_Third"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_ClassSkillSlot.push_back(dynamic_cast<CUI_SkillWindow_SkillSlot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	UIDesc.fCX = 375.f * 0.9f;
	UIDesc.fCY = 24.f * 0.9f;
	UIDesc.fY += 80.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_ClassSkill_Separator"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_ClassSkillSlot.push_back(dynamic_cast<CUI_SkillWindow_SkillSlot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	UIDesc.fCX = 380.f * 0.9f;
	UIDesc.fCY = 155.f * 0.9f;
	UIDesc.fY += fOffset;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_ClassSkill_Burst"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_ClassSkillSlot.push_back(dynamic_cast<CUI_SkillWindow_SkillSlot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	//m_SpecialSkillSlot
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	fOffset = 102.f;

	UIDesc.fCX = 380.f * 0.9f;
	UIDesc.fCY = 125.f * 0.9f;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f + 25.f);
	UIDesc.fY = 180.f;

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_ActiveSkill_First"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_SpecialSkillSlot.push_back(dynamic_cast<CUI_SkillWindow_SkillSlot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	UIDesc.fY += fOffset;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_ActiveSkill_Second"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_SpecialSkillSlot.push_back(dynamic_cast<CUI_SkillWindow_SkillSlot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	UIDesc.fY += fOffset;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_ActiveSkill_Third"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_SpecialSkillSlot.push_back(dynamic_cast<CUI_SkillWindow_SkillSlot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 762.f;
	UIDesc.fCY = 196.f;
	UIDesc.fX = 500.f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f);

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillWindow_SkillDesc"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_pSkillDesc = dynamic_cast<CUI_SkillWindow_LineBox*>(pWindow);
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

#pragma endregion

#pragma region BOSSHPBAR

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 72.f * 0.8f;
	UIDesc.fCY = 30.f * 0.8f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = 200;

	pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_AnnounceInstall"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_pCostumeAnnounce = dynamic_cast<CUI_Basic*>(pFrame);
	if (nullptr == m_pCostumeAnnounce)
		return E_FAIL;
	Safe_AddRef(m_pCostumeAnnounce);

	// Gara
	_float fY = 755.f;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 689.f * 0.8f;
	UIDesc.fCY = 81.f * 0.8f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = fY;

	pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_BossHP_Information"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_pBossInfo = dynamic_cast<CUI_BossHP_Background*>(pFrame);
	if (nullptr == m_pBossInfo)
		return E_FAIL;
	Safe_AddRef(m_pBossInfo);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 596.f * 0.8f;
	UIDesc.fCY = 22.f * 0.8f;
	UIDesc.fX = g_iWinSizeX * 0.5f - 30.f;
	UIDesc.fY = fY + 22.f;

	pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_BossHP_Barframe"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_pBossHPBack = dynamic_cast<CUI_BossHP_Background*>(pFrame);
	if (nullptr == m_pBossHPBack)
		return E_FAIL;
	Safe_AddRef(m_pBossHPBack);

	pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_BossHP_GaugeBar"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_pBossHPBar = dynamic_cast<CUI_BossHP_Bar*>(pFrame);
	if (nullptr == m_pBossHPBar)
		return E_FAIL;
	Safe_AddRef(m_pBossHPBar);

#pragma endregion

	m_Milepost.reserve(2);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 128.f * 0.35f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	CGameObject* pMilepost = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Milepost_Flag"), &UIDesc, &pMilepost)))
		return E_FAIL;
	m_Milepost.push_back(dynamic_cast<CUI_Milepost*>(pMilepost));
	if (nullptr == pMilepost)
		return E_FAIL;
	Safe_AddRef(pMilepost);

	pMilepost = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Milepost_Arrow"), &UIDesc, &pMilepost)))
		return E_FAIL;
	m_Milepost.push_back(dynamic_cast<CUI_Milepost*>(pMilepost));
	if (nullptr == pMilepost)
		return E_FAIL;
	Safe_AddRef(pMilepost);

	return S_OK;
}

HRESULT CUI_Manager::Ready_BossHPBar(CBoss* pBoss, void* pArg)
{
	if (nullptr == pBoss)
		return E_FAIL;

	m_pBossInfo->Set_Owner(pBoss);
	m_pBossHPBar->Set_Owner(pBoss);

	m_pBossInfo->Set_Active(true);
	m_pBossHPBack->Set_Active(true);
	m_pBossHPBar->Set_Active(true);

	return S_OK;
}

HRESULT CUI_Manager::UI_WndProcHandler(UINT message, WPARAM wParam, LPARAM lParam)
{
//	if (message == WM_LBUTTONDOWN)
//	{
//		m_bUpdate = true;
//	}

	// 입력 컨텍스트를 얻는다.
	HIMC himc = ImmGetContext(g_hWnd);
	_uint iLanguage = ImmGetOpenStatus(himc);
	WCHAR wszComBuffer[256] = { 0 }; // 입력정보를 저장할 버퍼

	switch (message)
	{
	case WM_CHAR:
	{
		wchar_t wchInput = static_cast<wchar_t>(wParam);
		wstring strInput(&wchInput, 1); // wchar_t 타입의 문자 wchInput을 이용하여 wstring 타입의 문자열 strInput을 생성함.
		if (strInput.length() > 0)
		{
			if (strInput == L"\b") // 백스페이스를 누르면
			{
				Update_SetNickname(strInput, false);
			}
			else
			{
				Update_SetNickname(strInput);
			}
		}
		break;
	}

	case WM_IME_COMPOSITION:
	{
		if (lParam & GCS_COMPSTR) // 조합중인 글자 출력
		{
			_uint ilength = ImmGetCompositionStringW(himc, GCS_COMPSTR, NULL, 0);
			ImmGetCompositionStringW(himc, GCS_COMPSTR, wszComBuffer, ilength);

		}

		if (lParam & GCS_RESULTSTR) // 완성된 글자 출력
		{
			_uint ilength = ImmGetCompositionStringW(himc, GCS_RESULTSTR, NULL, 0);
			ImmGetCompositionStringW(himc, GCS_RESULTSTR, wszComBuffer, ilength);
		}

		//ImmReleaseContext(hWnd, hIMC); str[len] = '\0';
		}
		break;
	}

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

	case LEVELID::LEVEL_ICELAND:
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
				if (j == i)
					continue;

				if (!m_ClickedPlayer[i]->Is_Arrived()) // Arrived가 false인 경우에는 
				{
					m_UnclickedPlayer[j]->Set_Clicked(false); // 클릭 이벤트가 발생하지 않도록 한다.
				}
			}
		}

		if (!m_ClickedPlayer[i]->Get_Active()) // Active가 False인데, unclicked도 false라면 unclicked를 true로 바꿔준다.
		{
			if (!m_UnclickedPlayer[i]->Get_Active())
			{
				m_UnclickedPlayer[i]->Set_Active(true);
			}
		}
	}

	//m_Buttons
	// 만약 Click된 것이 없다면 회색으로 출력되게 한다.
//				m_Buttons[0]->Set_UIPass(1); 16
	if (!m_ClickedPlayer[CUI_BtnCharacterSelect::BTN_ENGINEER]->Get_Active()
		&& !m_ClickedPlayer[CUI_BtnCharacterSelect::BTN_DESTROYER]->Get_Active()
		&& !m_ClickedPlayer[CUI_BtnCharacterSelect::BTN_SWORDMAN]->Get_Active())
	{
		m_Buttons[CUI_BasicButton::BUTTON_CHANGESCENE]->Set_UIPass(16);
		m_Buttons[CUI_BasicButton::BUTTON_CHANGESCENE]->Set_AllowClick(false);
		for (auto& iter : m_Basic)
		{
			if (TEXT("UI_Lobby_BtnText") == iter->Get_ObjectTag())
				iter->Set_UIPass(16);
		}
	}
	else
	{
		m_Buttons[CUI_BasicButton::BUTTON_CHANGESCENE]->Set_UIPass(1);
		m_Buttons[CUI_BasicButton::BUTTON_CHANGESCENE]->Set_AllowClick(true);
		for (auto& iter : m_Basic)
		{
			if (TEXT("UI_Lobby_BtnText") == iter->Get_ObjectTag())
				iter->Set_UIPass(1);
		}
	}


	return S_OK;
}

HRESULT CUI_Manager::Tick_EvermoreLevel(_float fTimeDelta)
{
	m_pDummy->Tick(fTimeDelta);

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

	//if (KEY_TAP(KEY::O))
	//{
	//	if (m_pWorldMapBG->Get_Active()) // 켜져있다면
	//		OnOff_WorldMap(false); // 끈다
	//	else // 꺼져있다면
	//		OnOff_WorldMap(true); // 켠다
	//}

	if (m_pCostumeBox->Get_Active())
		Update_CostumeBtn();

	return S_OK;
}

HRESULT CUI_Manager::LateTick_EvermoreLevel(_float fTimeDelta)
{
	m_pDummy->LateTick(fTimeDelta);

	return S_OK;
}

void CUI_Manager::LateTick_Dummy(_float fTimeDelta)
{
	m_pDummy->LateTick(fTimeDelta);
}

HRESULT CUI_Manager::Render_EvermoreLevel()
{
	//m_pDummy->Render();

	return S_OK;
}

void CUI_Manager::Render_Dummy()
{
	m_pDummy->Render();
}

void CUI_Manager::Tick_Fade(_float fTimeDelta)
{
	if (nullptr == m_pUIFade)
		return;

	m_pUIFade->Tick(fTimeDelta);
}

void CUI_Manager::LateTick_Fade(_float fTimeDelta)
{
	if (nullptr == m_pUIFade)
		return;

	m_pUIFade->LateTick(fTimeDelta);
}

void CUI_Manager::Render_Fade()
{
	if (nullptr == m_pUIFade)
		return;

	m_pUIFade->Render();
}

_bool CUI_Manager::Is_FadeFinished()
{
	if (nullptr == m_pUIFade)
		return false;

	return m_pUIFade->Get_Finish();
}

void CUI_Manager::Update_SetNickname(const wstring& strNickname, _bool bUpdate)
{
	if (m_pNicknamebox == nullptr)
		return;

	if (bUpdate)
	{
		if (m_pNicknamebox->Get_Active())
		{
			if (m_bUpdate)
			{
				m_strNickname += strNickname;
				m_pNicknamebox->Set_Text(m_strNickname);
			}
		}
	}
	else
	{
		if (m_pNicknamebox->Get_Active())
		{
			if (m_bUpdate)
			{
				if (m_strNickname.length() > 0)
				{
					m_strNickname.pop_back();
					m_pNicknamebox->Set_Text(m_strNickname);
				}
			}
		}
	}

}

void CUI_Manager::Update_LobbyBtnState(_uint iIndex)
{
	_uint iBtnIndex = iIndex;

	for (_uint i = 0; i < CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::UICHARACTERBTN_END; i++)
	{
		if (i == iBtnIndex)
			continue;

		if (m_ClickedPlayer[i]->Is_Arrived()) // 도착한 다른 Clicked Player가 있다면
			m_ClickedPlayer[i]->Set_Move(true); // 다시 돌아가야한다.
	}

}

void CUI_Manager::Update_CostumeBtnState(_uint iIndex)
{
	m_CostumeClickedBtn[iIndex]->Set_Active(true);

	if (iIndex == _uint(CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_CLOTH))
	{
		if (m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIRACC]->Get_Active())
			m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIRACC]->Set_Active(false);
	}
	else if (iIndex == _uint(CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIRACC))
	{
		if (m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_CLOTH]->Get_Active())
			m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_CLOTH]->Set_Active(false);
	}
}

void CUI_Manager::Update_InvenBtnState(_uint iIndex)
{
	m_InvenClickedBtn[iIndex]->Set_Active(true);

	//if (iIndex == _uint(CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_WEAPON))
	{

		for (_uint i = 0; i < CUI_Inventory_TabBtn::INVENTABBTN_END; ++i)
		{
			if (i == iIndex)
				continue;

			m_InvenClickedBtn[i]->Set_Active(false);
		}

	}
}

void CUI_Manager::Update_SkillBtnState(CTransform* pTransform, _uint iIndex)
{
	m_SkillClickedBtn[iIndex]->Set_Active(true);

	//CTransform* pTransform = m_SkillBtn[iIndex]->Get_Component<CTransform>(TEXT("Com_Transform"));
	//_float4 vClickedPosition;
	//XMStoreFloat4(&vClickedPosition, pTransform->Get_Position());
	//_float fY = vClickedPosition.y;
	_float fY;

	if (!m_SkillWindow[CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_ARROW]->Get_Active())
	{
		m_SkillWindow[CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_ARROW]->Set_Active(true);

		if (iIndex == 0)
			fY = 116.f;
		else if (iIndex == 1)
			fY = 226.f;
		else
			fY = 326.f;
	}
	else
	{
		if (iIndex == 0)
			fY = 116.f;
		else if (iIndex == 1)
			fY = 226.f;
		else
			fY = 326.f;
	}

	m_SkillWindow[CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_TITLE]->Set_TitleIndex(iIndex);
	m_SkillWindow[CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_ARROW]->Set_ArrowPosition(fY);

	for (_uint i = 0; i < CUI_SkillWindow_Btn::SKILLSUBBTN_END; ++i)
	{
		if (i == iIndex)
			continue;
	
		m_SkillClickedBtn[i]->Set_Active(false);
	}
}

void CUI_Manager::Update_SkillSlotState(_uint iSectionType, _uint iSlotIndex)
{
	//	enum UI_SKILLMENU_SECTION { SKILL_CLASS, SKILL_BURST, SKILL_ACTIVE, SKILL_SEPARATOR, SKILLSECTION_END };
	// enum UI_SKILLMENU_SLOT { SKILLSLOT_FIRST, SKILLSLOT_SECOND, SKILLSLOT_THIRD, SKILLSLOT_END };

	
	switch (iSectionType)
	{
	case 0:
		if (iSlotIndex < 0 || CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILLSECTION_END < iSlotIndex)
			return;
		
		m_ClassSkillSlot[iSlotIndex]->Set_Clicked(true);
		m_pSkillDesc->Set_iDescIndex(iSlotIndex); // 0, 1, 2

		for (_uint i = 0; i < m_ClassSkillSlot.size(); ++i)
		{
			if (i == iSlotIndex)
				continue;

			if (0 == m_ClassSkillSlot[i]->Get_SectionType() ||
				1 == m_ClassSkillSlot[i]->Get_SectionType())
				m_ClassSkillSlot[i]->Set_Clicked(false);
		}
		break;

	case 1: // BurstSkill이면 FirstSlot만 있음
		if (iSlotIndex < 0 || CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILLSECTION_END < iSlotIndex)
			return;

		m_ClassSkillSlot[iSlotIndex + 4]->Set_Clicked(true);
		m_pSkillDesc->Set_iDescIndex(iSlotIndex + 3); // 3

		for (_uint i = 0; i < m_ClassSkillSlot.size(); ++i)
		{
			if (i == iSlotIndex + 4)
				continue;

			if (0 == m_ClassSkillSlot[i]->Get_SectionType())
				m_ClassSkillSlot[i]->Set_Clicked(false);
		}
		break;

	case 2:
		if (iSlotIndex < 0 || CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILLSECTION_END < iSlotIndex)
			return;

		m_SpecialSkillSlot[iSlotIndex]->Set_Clicked(true);
		m_pSkillDesc->Set_iDescIndex(iSlotIndex + 4); // 4, 5, 6

		for (_uint i = 0; i < m_SpecialSkillSlot.size(); ++i)
		{
			if (i == iSlotIndex)
				continue;

			m_SpecialSkillSlot[i]->Set_Clicked(false);
		}
		break;

	case 3: // 선택이 되면 안된다.
		//return;
		break;
	}
}

void CUI_Manager::Update_ClothSlotState(_uint iSectionType, _uint iSlotIndex)
{
	//enum UI_COSTUME_SECTION { COSTUMESECTION_CLOTH, COSTUMESECTION_HAIRACC, COSTUMESECTION_END }; iSectionType
	//enum UI_COSTUME_SLOT { COSTUMESLOT_FIRST, COSTUMESLOT_SECOND, COSTUMESLOT_THIRD, COSTUMESLOT_END }; iSlotIndex

	switch (iSectionType)
	{
	case 0: // Cloth
		m_CostumeCloth[iSlotIndex]->Set_Clicked(true); // 선택되었다.
		// 나머지는 선택을 해제한다.
		for (_uint i = 0; i < m_CostumeCloth.size(); ++i)
		{
			if (i == iSlotIndex)
				continue;

			m_CostumeCloth[i]->Set_Clicked(false);
		}
		break;
		
	case 1: // HairAcc
		m_CostumeHairAcc[iSlotIndex]->Set_Clicked(true); // 선택되었다.

		for (_uint i = 0; i < m_CostumeHairAcc.size(); ++i)
		{
			if (i == iSlotIndex)
				continue;

			m_CostumeHairAcc[i]->Set_Clicked(false);
		}
		break;
	}
}

void CUI_Manager::Update_CostumeBtn()
{
	// 코스튬 Window가 활성화 되어있을 때 돌린다.

	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pPlayer)
		return;

	CCharacter* pCharacter = pPlayer->Get_Character();

	_int iIndex = -1;

	for (_uint i = 0; i < m_CostumeClickedBtn.size(); ++i)
	{
		if (m_CostumeClickedBtn[i]->Get_Active())
			iIndex = i;
	}

	if (iIndex < 0)
		return;

	switch (iIndex)
	{
	case 0:
		pCharacter->Get_PartModel(PART_TYPE::BODY);
		break;

	case 1:
		pCharacter->Get_PartModel(PART_TYPE::HEAD);
		break;
	}

}

void CUI_Manager::Update_CostumeModel(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType, const wstring& strPartTag)
{
	CModel* pParts = CCharacter_Manager::GetInstance()->Get_PartModel(eCharacterType, ePartType, strPartTag);
	//CModel* pParts = CCharacter_Manager::GetInstance()->Get_PartModel(eCharacterType, ePartType, iIndex);
	if (nullptr == pParts)
		return;

	m_pDummy->Set_PartModel(ePartType, pParts);

}

void CUI_Manager::Set_CostumeModel()
{
	// 현재 Active되어있는 CostumeButton.. 혹은 Slot을 찾아내서
	// PartType을 파악하고

	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (pPlayer == nullptr)
		return;
	CCharacter* pCharacter = pPlayer->Get_Character();
	if (pCharacter == nullptr)
		return;

	_uint iIndex;

	for (auto& iter : m_CostumeClickedBtn)
	{
		if (iter->Get_Active()) // 선택이 되었으면 활성화 되어있다.
		{
			iIndex = iter->Get_CostumeType();
		}
	}

	PART_TYPE eType = PART_TYPE::PART_END;
	//_int iSlotIndex = -1;
	wstring strPartTag;

	if (iIndex == CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_CLOTH)
	{
		for (auto& iter : m_CostumeCloth)
		{
			if (iter->Get_Clicked())
			{
				//iSlotIndex = _int(iter->Get_SlotIndex());
				strPartTag = iter->Get_PartTag();
			}
		}
		eType = PART_TYPE::BODY;
	}
	else if (iIndex == CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIRACC)
	{
		for (auto& iter : m_CostumeHairAcc)
		{
			if (iter->Get_Clicked())
			{
				//iSlotIndex = _int(iter->Get_SlotIndex());
				strPartTag = iter->Get_PartTag();
			}
		}
		eType = PART_TYPE::HEAD;
	}

	if (PART_TYPE::PART_END == eType)
		return;

//	if (-1 == iSlotIndex)
//		return;

	pCharacter->Set_PartModel(eType,
		CCharacter_Manager::GetInstance()->Get_PartModel(pCharacter->Get_CharacterType(), eType, strPartTag));
}

void CUI_Manager::Set_MouseCursor(_uint iIndex)
{
	if (nullptr == m_pUICursor)
		return;

	if (0 > iIndex || 1 < iIndex)
		return;

	m_pUICursor->Set_TextureIndex(iIndex);
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

	if (nullptr != m_pSettingBG)
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

	if (nullptr != m_pCostumeBox)
	{
		if (m_pCostumeBox->Get_Active())
		{
			OnOff_CostumeWindow(false);
		}
	}

	if (nullptr != m_pEmoticonWindow)
	{
		if (m_pEmoticonWindow->Get_Active())
			OnOff_EmoticonWindow(false);
	}

	return S_OK;
}

HRESULT CUI_Manager::Using_BackButton()
{
	if (LEVELID::LEVEL_LOBBY != GI->Get_CurrentLevel())
	{
		if (nullptr != m_pCostumeBox)
		{
			if (m_pCostumeBox->Get_Active())
			{
				OnOff_CostumeWindow(false);
			}
		}

		if (nullptr != m_pInvenBox)
		{
			if (m_pInvenBox->Get_Active())
			{
				OnOff_Inventory(false);
			}
		}

		if (nullptr != m_SkillWindow[CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_SUBMENU])
		{
			if (m_SkillWindow[CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_SUBMENU]->Get_Active())
			{
				OnOff_SkillWindow(false);
			}
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

HRESULT CUI_Manager::OnOff_NickNameWindow(_bool bOnOff)
{
	if (nullptr == m_pSetNickBG)
		return E_FAIL;

	if (bOnOff)
	{
		m_pSetNickBG->Set_Active(true);
		m_pNicknamebox->Set_Active(true);
	}
	else
	{
		m_pSetNickBG->Set_Active(false);
		m_pNicknamebox->Set_Active(false);
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_GamePlaySetting(_bool bOnOff)
{
	if (bOnOff) // On
	{
		m_pPlayerStatus->Set_Active(true);
		for (auto& iter : m_ItemQuickslot)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}
		m_pBtnShowMenu->Set_Active(true);
		m_pBtnCamera->Set_Active(true);
		m_pBtnInven->Set_Active(true);
		m_pBtnQuest->Set_Active(true);
		m_pBtnShowMinimap->Set_Active(true);

		m_pSkillBG->Set_Active(true);
		for (auto& iter : m_ClassicSkill)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}
		for (auto& iter : m_ClassicFrame)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}
		for (auto& iter : m_SpecialSkill)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}
		for (auto& iter : m_SpecialFrame)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}

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
		for (auto& iter : m_ItemQuickslot)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		m_pBtnShowMenu->Set_Active(false);
		m_pBtnCamera->Set_Active(false);
		m_pBtnInven->Set_Active(false);
		m_pBtnQuest->Set_Active(false);
		m_pBtnShowMinimap->Set_Active(false);

		m_pSkillBG->Set_Active(false);
		for (auto& iter : m_ClassicSkill)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_ClassicFrame)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_SpecialSkill)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_SpecialFrame)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		m_pImajinnBG->Set_Active(false);
		m_pCameraAnnounce->Set_Active(false);
	
		OnOff_MonsterHP(false);

		// EXP Bar를 감춘다
		for (auto& iter : m_PlayerEXP)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		// MiniQuestPopup이 켜져있다면 끈다
		OnOff_QuestPopup(false);
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
		//m_pBtnClose->Set_Active(true); // Close버튼

		OnOff_CloseButton(true);

	}
	else // Off : 모든 Menu관련 창을 꺼야한다.
	{
		OnOff_CloseButton(false);

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

HRESULT CUI_Manager::OnOff_QuestPopup(_bool bOnOff)
{
	if (bOnOff)
	{
		for (auto iter : m_QuestPopUp)
		{
			if (iter != nullptr)
				iter->Set_Active(true);
		}
	}
	else
	{
		for (auto iter : m_QuestPopUp)
		{
			if (iter != nullptr)
				iter->Set_Active(false);
		}
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

HRESULT CUI_Manager::OnOff_BossHP(_bool bOnOff)
{
	if (bOnOff)
	{
		//if (!m_pBossHPBack->Get_Active())
		{
			m_pBossInfo->Set_Active(true);
			m_pBossHPBack->Set_Active(true);
			m_pBossHPBar->Set_Active(true);
		}
	}
	else
	{
		//if (m_pBossHPBack->Get_Active())
		{
			m_pBossInfo->Set_Active(false);
			m_pBossHPBack->Set_Active(false);
			m_pBossHPBar->Set_Active(false);
		}
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
		if (!m_pDefaultBG->Get_Active())
		{
			OnOff_MainMenu(false);

			// 시작할때 더미 캐릭터가 지금 플레이어와 같은 의상을 착용하도록 세팅 해주어야 한다.
			CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
			if (pPlayer == nullptr)
				return E_FAIL;
			CCharacter* pCharacter = pPlayer->Get_Character();
			if (pCharacter == nullptr)
				return E_FAIL;
			m_pDummy->Set_PartModel(PART_TYPE::BODY, pCharacter->Get_PartModel(PART_TYPE::BODY));
			m_pDummy->Set_PartModel(PART_TYPE::HEAD, pCharacter->Get_PartModel(PART_TYPE::HEAD));

			m_pDefaultBG->Set_Active(true);
			for (auto& iter : m_CostumeBtn)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			m_pCostumeBox->Set_Active(true);
			// 의상 등 버튼을 누를때 Active가 활성화되도록 우선 수정한다.
			//for (auto& iter : m_CostumeItem)
			//{
			//	if (nullptr != iter)
			//		iter->Set_Active(true);
			//}

			m_pTabMenuTitle->Set_TextType(CUI_Text_TabMenu::UI_MENUTITLE::TITLE_COSTUME);
			m_pTabMenuTitle->Set_Active(true);

			m_pDummy->Set_Active(true);

			//OnOff_CloseButton(true);
		}
	}
	else
	{
		//OnOff_GamePlaySetting(true);
		//OnOff_CloseButton(false);
		m_pDummy->Set_Active(false);
		m_pTabMenuTitle->Set_Active(false);

		OnOff_CostumeSlot(0, false); // false에는 숫자가 영향을 미치지않음. 함수 인자 순서 수정 필요함.

		m_pCostumeBox->Set_Active(false);
		for (auto& iter : m_CostumeBtn)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_CostumeClickedBtn)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		m_pDefaultBG->Set_Active(false);

		OnOff_MainMenu(true);
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_CostumeSlot(_uint iSection, _bool bOnOff)
{
	// enum UI_COSTUMEBTN_TYPE { COSTUMEBTN_UNCLICKED, COSTUMEBTN_CLICKED, SKILLBTNTYPE_END };
	
	if (bOnOff)
	{

		// 옷을 켠거면 ,,
		switch (iSection)
		{
		case 0:
			for (auto& iter : m_CostumeCloth)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			for (auto& iter : m_CostumeHairAcc)
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			break;

		case 2:
			for (auto& iter : m_CostumeCloth)
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			for (auto& iter : m_CostumeHairAcc)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			break;
		}
	}
	else
	{
		for (auto& iter : m_CostumeCloth)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_CostumeHairAcc)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_SkillWindowSlot(_uint iMenuType, _bool bOnOff)
{
	// 	enum UI_SKILLSUBBTN { SUBMENU_CLASSIC, SUBMENU_ACTIVE, SUBMENU_PASSIVE, SKILLSUBBTN_END };

	if (bOnOff) // On
	{
		switch (iMenuType)
		{
		case 0:
			for (auto& iter : m_ClassSkillSlot)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			for (auto& iter : m_SpecialSkillSlot) // Active
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			break;

		case 1:
			for (auto& iter : m_SpecialSkillSlot) // Active
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			for (auto& iter : m_ClassSkillSlot)
			{
				if (nullptr != iter)
				{
					if(iter->Get_Active())
						iter->Set_Active(false);
				}
			}
			break;

		case 2:
			for (auto& iter : m_SpecialSkillSlot) // Active
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			for (auto& iter : m_ClassSkillSlot)
			{
				if (nullptr != iter)
				{
					if (iter->Get_Active())
						iter->Set_Active(false);
				}
			}
			break;
		}
	}
	else
	{
//		for (auto& iter : m_CostumeItem)
//		{
//			if (nullptr != iter)
//				iter->Set_Active(false);
//		}
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_Announce(_int iMagicNum, _bool bOnOff)
{
	if (0 > iMagicNum || 2 < iMagicNum)
		return E_FAIL;
	
	if (m_pCameraAnnounce->Get_Active())
		return E_FAIL;
	else
	{
		m_pCameraAnnounce->Set_Active(bOnOff, iMagicNum);
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_Inventory(_bool bOnOff)
{
	if (bOnOff)
	{
		if (!m_pDefaultBG->Get_Active())
		{
			OnOff_GamePlaySetting(false);
			m_pDefaultBG->Set_Active(true);
			m_pInvenBox->Set_Active(true);

			for (auto& iter : m_InvenBtn)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}

			for (auto& iter : m_InvenSlots)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}

			m_pTabMenuTitle->Set_TextType(CUI_Text_TabMenu::UI_MENUTITLE::TITLE_INVEN);
			m_pTabMenuTitle->Set_Active(true);
		}
	}
	else
	{
		m_pTabMenuTitle->Set_Active(false);

		for (auto& iter : m_InvenBtn)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_InvenClickedBtn)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_InvenSlots)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		m_pInvenBox->Set_Active(false);
		m_pDefaultBG->Set_Active(false);
		OnOff_GamePlaySetting(true);
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_SkillWindow(_bool bOnOff)
{
	if (bOnOff)
	{
		if (!m_pDefaultBG->Get_Active())
		{
			OnOff_MainMenu(false);
			m_pDefaultBG->Set_Active(true);

			for (auto& iter : m_SkillWindow)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			m_SkillWindow[CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_ARROW]->Set_Active(false);

			for (auto& iter : m_SkillBtn)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}

			m_pSkillDesc->Set_iDescIndex(-1);
			m_pSkillDesc->Set_Active(true);

			//m_pCostumeBox->Set_Active(true);
			//m_pTabMenuTitle->Set_TextType(CUI_Text_TabMenu::UI_MENUTITLE::TITLE_COSTUME);
			//m_pTabMenuTitle->Set_Active(true);
		}
	}
	else
	{
		//m_pTabMenuTitle->Set_Active(false);
		//OnOff_CostumeSlot(false);
		//m_pCostumeBox->Set_Active(false);
		// 
		//Test Code
		m_pSkillDesc->Set_Active(false);

		for (auto& iter : m_SpecialSkillSlot)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_ClassSkillSlot)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		for (auto& iter : m_SkillBtn)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_SkillClickedBtn)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_SkillWindow)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		m_pDefaultBG->Set_Active(false);
		OnOff_MainMenu(true);
	}


	return S_OK;
}

HRESULT CUI_Manager::OnOff_EmoticonWindow(_bool bOnOff)
{
	if (nullptr == m_pEmoticonWindow)
		return E_FAIL;

	if (bOnOff)
	{
		if (!m_pEmoticonWindow->Get_Active())
			m_pEmoticonWindow->Set_Active(true);
	}
	else
	{
		if (m_pEmoticonWindow->Get_Active())
			m_pEmoticonWindow->Set_Active(false);
	}

	return S_OK;
}

HRESULT CUI_Manager::OnOff_EmoticonBalloon(_bool bOnOff)
{
	if (nullptr == m_pBalloon)
		return E_FAIL;

	if (bOnOff) // 켤때
	{
		m_pBalloon->Set_Active(true);
	}
	else // 끌때
	{
		m_pBalloon->Set_Alpha(true);
	}

	return S_OK;
}

void CUI_Manager::Set_EmoticonType(_uint iIndex)
{
	if (nullptr == m_pBalloon)
		return;

	m_pBalloon->Set_EmoticonType(iIndex);
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

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_Imajinn"),
		CUI_Loading_Imajinn::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Fade"),
		CUI_Fade::Create(m_pDevice, m_pContext), LAYER_UI)))
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

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_LineBox"),
		CUI_Costume_LineBox::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_Cloth"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_UNCLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_CLOTH), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_Hair"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_UNCLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIR), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_HairAcc"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_UNCLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIRACC), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_ExteriorDeco"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_UNCLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_EXTERIORDECO), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_FaceDeco"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_UNCLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_FACEDECO), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_UnClicked_Weapon"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_UNCLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_WEAPON), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_Cloth"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_CLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_CLOTH), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_Hair"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_CLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIR), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_HairAcc"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_CLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIRACC), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_ExteriorDeco"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_CLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_EXTERIORDECO), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_FaceDeco"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_CLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_FACEDECO), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_CostumeBtn_Clicked_Weapon"),
		CUI_Costume_Btn::Create(m_pDevice, m_pContext,
			CUI_Costume_Btn::UI_COSTUMEBTN_TYPE::COSTUMEBTN_CLICKED, CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_WEAPON), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_ChangeBtn"),
		CUI_Costume_ChangeBtn::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_AnnounceInstall"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Costume_Announce", CUI_Basic::UI_BASIC::COSTUME_INSTALL), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Common_BackBtn"),
		CUI_BtnBack::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_Classic_First"),
		CUI_SkillSection_ClassicSkill::Create(m_pDevice, m_pContext, CUI_SkillSection_ClassicSkill::UI_CLASSICSKILL::SKILL_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_Classic_Second"),
		CUI_SkillSection_ClassicSkill::Create(m_pDevice, m_pContext, CUI_SkillSection_ClassicSkill::UI_CLASSICSKILL::SKILL_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_Classic_Third"),
		CUI_SkillSection_ClassicSkill::Create(m_pDevice, m_pContext, CUI_SkillSection_ClassicSkill::UI_CLASSICSKILL::SKILL_THIRD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_Special_First"),
		CUI_SkillSection_SpecialSkill::Create(m_pDevice, m_pContext, CUI_SkillSection_SpecialSkill::UI_SPECIALSKILL::SKILL_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_Special_Second"),
		CUI_SkillSection_SpecialSkill::Create(m_pDevice, m_pContext, CUI_SkillSection_SpecialSkill::UI_SPECIALSKILL::SKILL_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_Special_Third"),
		CUI_SkillSection_SpecialSkill::Create(m_pDevice, m_pContext, CUI_SkillSection_SpecialSkill::UI_SPECIALSKILL::SKILL_THIRD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Item_QuickSlot_First"),
		CUI_QuickSlot_Item::Create(m_pDevice, m_pContext, CUI_QuickSlot_Item::UI_QUICKSLOT_ITEM::QUICKITEM_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Item_QuickSlot_Second"),
		CUI_QuickSlot_Item::Create(m_pDevice, m_pContext, CUI_QuickSlot_Item::UI_QUICKSLOT_ITEM::QUICKITEM_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Item_QuickSlot_Third"),
		CUI_QuickSlot_Item::Create(m_pDevice, m_pContext, CUI_QuickSlot_Item::UI_QUICKSLOT_ITEM::QUICKITEM_THIRD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_CostumeSection_First"),
		CUI_Costume_ItemSlot::Create(m_pDevice, m_pContext, CUI_Costume_ItemSlot::UI_COSTUME_SECTION::COSTUMESECTION_CLOTH,
			CUI_Costume_ItemSlot::UI_COSTUME_SLOT::COSTUMESLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_CostumeSection_Second"),
		CUI_Costume_ItemSlot::Create(m_pDevice, m_pContext, CUI_Costume_ItemSlot::UI_COSTUME_SECTION::COSTUMESECTION_CLOTH, 
			CUI_Costume_ItemSlot::UI_COSTUME_SLOT::COSTUMESLOT_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_CostumeSection_Third"),
		CUI_Costume_ItemSlot::Create(m_pDevice, m_pContext, CUI_Costume_ItemSlot::UI_COSTUME_SECTION::COSTUMESECTION_CLOTH, 
			CUI_Costume_ItemSlot::UI_COSTUME_SLOT::COSTUMESLOT_THIRD), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_HairAccSection_First"),
		CUI_Costume_ItemSlot::Create(m_pDevice, m_pContext, CUI_Costume_ItemSlot::UI_COSTUME_SECTION::COSTUMESECTION_HAIRACC,
			CUI_Costume_ItemSlot::UI_COSTUME_SLOT::COSTUMESLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_HairAccSection_Second"),
		CUI_Costume_ItemSlot::Create(m_pDevice, m_pContext, CUI_Costume_ItemSlot::UI_COSTUME_SECTION::COSTUMESECTION_HAIRACC,
			CUI_Costume_ItemSlot::UI_COSTUME_SLOT::COSTUMESLOT_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_HairAccSection_Third"),
		CUI_Costume_ItemSlot::Create(m_pDevice, m_pContext, CUI_Costume_ItemSlot::UI_COSTUME_SECTION::COSTUMESECTION_HAIRACC,
			CUI_Costume_ItemSlot::UI_COSTUME_SLOT::COSTUMESLOT_THIRD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Top"),
		CUI_PopupQuest::Create(m_pDevice, m_pContext, CUI_PopupQuest::UI_QUESTPOPUP::POPUPFRAME_TOP), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Bottom"),
		CUI_PopupQuest::Create(m_pDevice, m_pContext, CUI_PopupQuest::UI_QUESTPOPUP::POPUPFRAME_BOTTOM), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Window"),
		CUI_PopupQuest::Create(m_pDevice, m_pContext, CUI_PopupQuest::UI_QUESTPOPUP::POPUPWINDOW), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_ClassicSKill_Frame"),
		CUI_SkillSection_Frame::Create(m_pDevice, m_pContext, CUI_SkillSection_Frame::UI_SKILLFRAME_TYPE::FRAME_CLASSIC), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SpecialSKill_Frame_First"),
		CUI_SkillSection_Frame::Create(m_pDevice, m_pContext,
			CUI_SkillSection_Frame::UI_SKILLFRAME_TYPE::FRAME_SPECIAL,
			CUI_SkillSection_Frame::UI_SPECIALSKILL_TYPE::FRAME_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SpecialSKill_Frame_Second"),
		CUI_SkillSection_Frame::Create(m_pDevice, m_pContext,
			CUI_SkillSection_Frame::UI_SKILLFRAME_TYPE::FRAME_SPECIAL,
			CUI_SkillSection_Frame::UI_SPECIALSKILL_TYPE::FRAME_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SpecialSKill_Frame_Third"),
		CUI_SkillSection_Frame::Create(m_pDevice, m_pContext,
			CUI_SkillSection_Frame::UI_SKILLFRAME_TYPE::FRAME_SPECIAL,
			CUI_SkillSection_Frame::UI_SPECIALSKILL_TYPE::FRAME_THIRD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Announced_Camera"),
		CUI_Announced::Create(m_pDevice, m_pContext, CUI_Announced::ANNOUNCE_CAMERA), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar"),
		CUI_MonsterHP_World::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar_ArrowLeft"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Monster_WorldHP_ArrowLeft", CUI_Basic::TARGETARROW_LEFT), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar_ArrowRight"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Monster_WorldHP_ArrowRight", CUI_Basic::TARGETARROW_RIGHT), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_Window"),
		CUI_Emoticon_Window::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_SpeechBalloon"),
		CUI_Emoticon_SpeechBalloon::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_Btn_First"),
		CUI_Emoticon_Button::Create(m_pDevice, m_pContext, CUI_Emoticon_Button::UI_EMOTICON_BTN::EMOTIONBTN_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_Btn_Second"),
		CUI_Emoticon_Button::Create(m_pDevice, m_pContext, CUI_Emoticon_Button::UI_EMOTICON_BTN::EMOTIONBTN_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_Btn_Third"),
		CUI_Emoticon_Button::Create(m_pDevice, m_pContext, CUI_Emoticon_Button::UI_EMOTICON_BTN::EMOTIONBTN_THIRD), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_Btn_Fourth"),
		CUI_Emoticon_Button::Create(m_pDevice, m_pContext, CUI_Emoticon_Button::UI_EMOTICON_BTN::EMOTIONBTN_FOURTH), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_Btn_Fifth"),
		CUI_Emoticon_Button::Create(m_pDevice, m_pContext, CUI_Emoticon_Button::UI_EMOTICON_BTN::EMOTIONBTN_FIFTH), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_Btn_Sixth"),
		CUI_Emoticon_Button::Create(m_pDevice, m_pContext, CUI_Emoticon_Button::UI_EMOTICON_BTN::EMOTIONBTN_SIXTH), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_Btn_Seventh"),
		CUI_Emoticon_Button::Create(m_pDevice, m_pContext, CUI_Emoticon_Button::UI_EMOTICON_BTN::EMOTIONBTN_SEVENTH), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_Btn_Eighth"),
		CUI_Emoticon_Button::Create(m_pDevice, m_pContext, CUI_Emoticon_Button::UI_EMOTICON_BTN::EMOTIONBTN_EIGHTH), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Emoticon_BallonEmoticon"),
		CUI_Emoticon_BalloonEmoticon::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_LineBox"),
		CUI_Inventory_LineBox::Create(m_pDevice, m_pContext, CUI_Inventory_LineBox::UI_INVENDECOTYPE::INVEN_LINEBOX), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Decoline"),
		CUI_Inventory_LineBox::Create(m_pDevice, m_pContext, CUI_Inventory_LineBox::UI_INVENDECOTYPE::INVEN_DECOLINE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_Weapon"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_UNCLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_WEAPON), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_Armor"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_UNCLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_ARMOR), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_Acc"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_UNCLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_ACC), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_CrystalBall"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_UNCLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_CRYSTALBALL), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_Etc1"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_UNCLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_ETC1), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_UnClicked_Etc2"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_UNCLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_ETC2), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_Weapon"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_CLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_WEAPON), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_Armor"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_CLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_ARMOR), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_Acc"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_CLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_ACC), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_CrystalBall"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_CLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_CRYSTALBALL), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_Etc1"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_CLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_ETC1), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_InvenBtn_Clicked_Etc2"),
		CUI_Inventory_TabBtn::Create(m_pDevice, m_pContext,
			CUI_Inventory_TabBtn::UI_INVENBTN_TYPE::INVENBTN_CLICKED, CUI_Inventory_TabBtn::UI_INVENTABBTN::INVEN_ETC2), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Necklace"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_NECKLACE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Sword1"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_SWORD1), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Earring"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_EARRING), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Sword2"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_SWORD2), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Ring"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_RING), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Sword3"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_SWORD3), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Helmet"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_HELMET), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Armor"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_ARMOR), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Gloves"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_GLOVES), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Boots"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_BOOTS), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Crystal1"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_LOCK_CRYSTAL1), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Inventory_Slot_Crystal2"),
		CUI_Inventory_Slot::Create(m_pDevice, m_pContext, CUI_Inventory_Slot::UI_INVENSLOT_TYPE::INVENSLOT_LOCK_CRYSTAL2), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_Unclicked_ClassicBtn"),
		CUI_SkillWindow_Btn::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_Btn::UI_SKILLBTN_TYPE::SKILLBTN_UNCLICKED, CUI_SkillWindow_Btn::UI_SKILLSUBBTN::SUBMENU_CLASSIC), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_Clicked_ClassicBtn"),
		CUI_SkillWindow_Btn::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_Btn::UI_SKILLBTN_TYPE::SKILLBTN_CLICKED, CUI_SkillWindow_Btn::UI_SKILLSUBBTN::SUBMENU_CLASSIC), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_Unclicked_PassiveBtn"),
		CUI_SkillWindow_Btn::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_Btn::UI_SKILLBTN_TYPE::SKILLBTN_UNCLICKED, CUI_SkillWindow_Btn::UI_SKILLSUBBTN::SUBMENU_PASSIVE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_Clicked_PassiveBtn"),
		CUI_SkillWindow_Btn::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_Btn::UI_SKILLBTN_TYPE::SKILLBTN_CLICKED, CUI_SkillWindow_Btn::UI_SKILLSUBBTN::SUBMENU_PASSIVE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_Unclicked_ActiveBtn"),
		CUI_SkillWindow_Btn::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_Btn::UI_SKILLBTN_TYPE::SKILLBTN_UNCLICKED, CUI_SkillWindow_Btn::UI_SKILLSUBBTN::SUBMENU_ACTIVE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_Clicked_ActiveBtn"),
		CUI_SkillWindow_Btn::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_Btn::UI_SKILLBTN_TYPE::SKILLBTN_CLICKED, CUI_SkillWindow_Btn::UI_SKILLSUBBTN::SUBMENU_ACTIVE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_SubMenuBg"),
		CUI_SkillWindow_LineBox::Create(m_pDevice, m_pContext, CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_SUBMENU), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_LineBox"),
		CUI_SkillWindow_LineBox::Create(m_pDevice, m_pContext, CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_LINEBOX), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_Select_Arrow"),
		CUI_SkillWindow_LineBox::Create(m_pDevice, m_pContext, CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_ARROW), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_Title"),
		CUI_SkillWindow_LineBox::Create(m_pDevice, m_pContext, CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_TITLE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_ClassSkill_First"),
		CUI_SkillWindow_SkillSlot::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILL_CLASS,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SLOT::SKILLSLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_ClassSkill_Second"),
		CUI_SkillWindow_SkillSlot::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILL_CLASS,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SLOT::SKILLSLOT_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_ClassSkill_Third"),
		CUI_SkillWindow_SkillSlot::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILL_CLASS,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SLOT::SKILLSLOT_THIRD), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_ClassSkill_Burst"),
		CUI_SkillWindow_SkillSlot::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILL_BURST,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SLOT::SKILLSLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_ClassSkill_Separator"),
		CUI_SkillWindow_SkillSlot::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILL_SEPARATOR,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SLOT::SKILLSLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_ActiveSkill_First"),
		CUI_SkillWindow_SkillSlot::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILL_ACTIVE,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SLOT::SKILLSLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_ActiveSkill_Second"),
		CUI_SkillWindow_SkillSlot::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILL_ACTIVE,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SLOT::SKILLSLOT_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_ActiveSkill_Third"),
		CUI_SkillWindow_SkillSlot::Create(m_pDevice, m_pContext,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SECTION::SKILL_ACTIVE,
			CUI_SkillWindow_SkillSlot::UI_SKILLMENU_SLOT::SKILLSLOT_THIRD), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillWindow_SkillDesc"),
		CUI_SkillWindow_LineBox::Create(m_pDevice, m_pContext, CUI_SkillWindow_LineBox::UI_SKILLWINDOW::SKWINDOW_DESC), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Common_Btn_ShowMinimap"),
		CUI_Btn_Minimap::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_BossHP_Information"),
		CUI_BossHP_Background::Create(m_pDevice, m_pContext,
			CUI_BossHP_Background::UI_BOSSHPBACK::BOSS_INFO), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_BossHP_Barframe"),
		CUI_BossHP_Background::Create(m_pDevice, m_pContext,
			CUI_BossHP_Background::UI_BOSSHPBACK::BOSS_BARFRAME), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_BossHP_GaugeBar"),
		CUI_BossHP_Bar::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Milepost_Flag"),
		CUI_Milepost::Create(m_pDevice, m_pContext, CUI_Milepost::UI_MILEPOST::MILEPOST_FLAG), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Milepost_Arrow"),
		CUI_Milepost::Create(m_pDevice, m_pContext, CUI_Milepost::UI_MILEPOST::MILEPOST_ARROW), LAYER_UI)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Manager::Ready_UILobbyPrototypes()
{
	/////////////////
	// GameObjects //
	/////////////////

	// Character NickName Frame
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_NicknameFrame"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Lobby_NicknameFrame", CUI_Basic::UI_BASIC::UILOBBY_NICKFRAME), LAYER_UI)))
		return E_FAIL;

	// PopUp
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_Announce"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Lobby_NicknameFrame", CUI_Basic::UI_BASIC::UILOBBY_ANNOUNCE), LAYER_UI)))
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

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Btn_Back"),
		CUI_BtnBack::Create(m_pDevice, m_pContext), LAYER_UI)))
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



	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DefaultBackground_Cloud"),
		CUI_Default_BackCloud::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;



	// Set Nickname Window
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SetNickname_Window"),
		CUI_SetNickname_Window::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Btn_SetNickname"),
		CUI_BasicButton::Create(m_pDevice, m_pContext, L"UI_Btn_Basic_SetNickname", CUI_BasicButton::UIBUTTON_TYPE::BUTTON_SETNICKNAME), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SetNickname_Textbox"),
		CUI_SetNickname_Textbox::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SetNickname_Dice"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_Btn_Basic_SetNickname_Dice", CUI_Basic::UI_BASIC::UILOBBY_DICE), LAYER_UI)))
		return E_FAIL;



	return S_OK;
}

#pragma endregion

void CUI_Manager::Set_CharacterType()
{
	// 캐릭터 타입에 따라 변경되어야 하는 UI들에게 CharacterType을 알려준다.

	// For Main UI(GamePlay Default Setting)
	if (nullptr != m_pSkillBG)
		m_pSkillBG->Set_CharacterType(m_eCurPlayer);

	for (auto& iter : m_SpecialFrame)
	{
		if (nullptr != iter)
			iter->Set_CharacterType(m_eCurPlayer);
	}
	for (auto& iter : m_SpecialSkill)
	{
		if (nullptr != iter)
			iter->Set_CharacterType(m_eCurPlayer);
	}
	for (auto& iter : m_ClassicSkill)
	{
		if (nullptr != iter)
			iter->Set_CharacterType(m_eCurPlayer);
	}
	
	// For Costume Window
	for (auto& iter : m_CostumeCloth)
	{
		if (nullptr != iter)
			iter->Set_CharacterType(m_eCurPlayer);
	}
	for (auto& iter : m_CostumeHairAcc)
	{
		if (nullptr != iter)
			iter->Set_CharacterType(m_eCurPlayer);
	}

	// For SkillWindow
	m_pSkillDesc->Set_CharacterType(m_eCurPlayer);
	for (auto& iter : m_ClassSkillSlot)
	{
		if (nullptr != iter)
			iter->Set_SkillSlot(m_eCurPlayer);
	}
	for (auto& iter : m_SpecialSkillSlot)
	{
		if (nullptr != iter)
			iter->Set_SkillSlot(m_eCurPlayer);
	}
}

void CUI_Manager::Set_ElementalType()
{
	// 무기 속성에 따른 색 변경 등이 필요한 경우에 사용한다.
	for (auto& iter : m_ClassicFrame)
	{
		if (nullptr != iter)
			iter->Set_ElementalType(m_eElemental);
	}

}

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

	Safe_Release(m_pCostumeBox);
	Safe_Release(m_pCameraAnnounce);
	Safe_Release(m_pCostumeAnnounce);

	Safe_Release(m_pEmoticonWindow);
	Safe_Release(m_pBalloon);

	Safe_Release(m_pInvenBox);
	Safe_Release(m_pSkillDesc);

	Safe_Release(m_pSetNickBG);
	Safe_Release(m_pNicknamebox);
	Safe_Release(m_pBtnShowMinimap);

	Safe_Release(m_pBossInfo);
	Safe_Release(m_pBossHPBack);
	Safe_Release(m_pBossHPBar);

	Safe_Release(m_pDummy);

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

	for (auto& pBtn : m_CostumeBtn)
		Safe_Release(pBtn);
	m_CostumeBtn.clear();

	for (auto& pBtn : m_CostumeClickedBtn)
		Safe_Release(pBtn);
	m_CostumeClickedBtn.clear();

	for (auto& pClassic : m_ClassicSkill)
		Safe_Release(pClassic);
	m_ClassicSkill.clear();

	for (auto& pSpecial : m_SpecialSkill)
		Safe_Release(pSpecial);
	m_SpecialSkill.clear();

	for (auto& pSlot : m_ItemQuickslot)
		Safe_Release(pSlot);
	m_ItemQuickslot.clear();

	for (auto& pItemSlot : m_CostumeCloth)
		Safe_Release(pItemSlot);
	m_CostumeCloth.clear();

	for (auto& pItemSlot : m_CostumeHairAcc)
		Safe_Release(pItemSlot);
	m_CostumeHairAcc.clear();

	for (auto& pPopup : m_QuestPopUp)
		Safe_Release(pPopup);
	m_QuestPopUp.clear();

	for (auto& pFrame : m_ClassicFrame)
		Safe_Release(pFrame);
	m_ClassicFrame.clear();

	for (auto& pFrame : m_SpecialFrame)
		Safe_Release(pFrame);
	m_SpecialFrame.clear();

	for (auto& pBtn : m_InvenBtn)
		Safe_Release(pBtn);
	m_InvenBtn.clear();

	for (auto& pBtn : m_InvenClickedBtn)
		Safe_Release(pBtn);
	m_InvenClickedBtn.clear();

	for (auto& pSlot : m_InvenSlots)
		Safe_Release(pSlot);
	m_InvenSlots.clear();

	for (auto& pBtn : m_SkillBtn)
		Safe_Release(pBtn);
	m_SkillBtn.clear();

	for (auto& pBtn : m_SkillClickedBtn)
		Safe_Release(pBtn);
	m_SkillClickedBtn.clear();

	for (auto& pWindow : m_SkillWindow)
		Safe_Release(pWindow);
	m_SkillWindow.clear();

	for (auto& pSlot : m_ClassSkillSlot)
		Safe_Release(pSlot);
	m_ClassSkillSlot.clear();

	for (auto& pSlot : m_SpecialSkillSlot)
		Safe_Release(pSlot);
	m_SpecialSkillSlot.clear();

	for (auto& pMilepost : m_Milepost)
		Safe_Release(pMilepost);
	m_Milepost.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
