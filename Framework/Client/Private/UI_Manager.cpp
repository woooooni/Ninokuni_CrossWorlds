#include "stdafx.h"
#include "UI_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Effect.h"

#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"
#include "Character_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Monster.h"
#include "Weapon.h"
#include "Weapon_Manager.h"
#include "UI_Dummy_Weapon.h"

#include "Mirror.h"
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
#include "UI_MapName_Text.h"
#include "UI_Quest_Reward.h"
#include "UI_Boss_NameTag.h"
#include "UI_Setting_Icon.h"
#include "UI_SubMenu_Shop.h"
#include "UI_Text_TabMenu.h"
#include "UI_BtnInventory.h"
#include "UI_PlayerEXPBar.h"
#include "UI_World_NPCTag.h"
#include "UI_Vignette_Ice.h"
#include "UI_Vignette_Fire.h"
#include "UI_Minimap_Frame.h"
#include "UI_MonsterHP_Bar.h"
#include "UI_MenuSeparator.h"
#include "UI_Dialog_Window.h"
#include "UI_BtnQuickQuest.h"
#include "UI_World_NameTag.h"
#include "UI_CostumeTab_Map.h"
#include "UI_Inventory_Slot.h"
#include "UI_Setting_Slider.h"
#include "UI_Setting_Window.h"
#include "UI_BtnShowSetting.h"
#include "UI_WindowWorldMap.h"
#include "UI_QuickSlot_Item.h"
#include "UI_CharacterDummy.h"
#include "UI_World_AnimalTag.h"
#include "UI_Dummy_Swordsman.h"
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
#include "UI_Tutorial_Window.h"
#include "UI_Inventory_TabBtn.h"
#include "UI_Costume_ItemSlot.h"
#include "UI_Loading_MainLogo.h"
#include "UI_Btn_WorldMapIcon.h"
#include "UI_Quest_Reward_Item.h"
#include "UI_World_Interaction.h"
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
#include "UI_InGame_Setting_Tab.h"
#include "UI_SkillSection_Frame.h"
#include "UI_Default_Background.h"
#include "UI_BtnCharacterSelect.h"
#include "UI_SetNickname_Window.h"
#include "UI_Loading_Background.h"
#include "UI_WeaponSection_Slot.h"
#include "UI_InGame_Setting_Slot.h"
#include "UI_Dialog_BattleWindow.h"
#include "UI_SetNickname_Textbox.h"
#include "UI_SkillWindow_LineBox.h"
#include "UI_Loading_ProgressBar.h"
#include "UI_Loading_Information.h"
#include "UI_MonsterHP_Elemental.h"
#include "UI_ImajinnSection_Slot.h"
#include "UI_WeaponSection_Weapon.h"
#include "UI_SkillSection_BtnRoll.h"
#include "UI_SkillSection_BtnJump.h"
#include "UI_MonsterHP_Background.h"
#include "UI_SkillWindow_SkillSlot.h"
#include "UI_Loading_CharacterLogo.h"
#include "UI_InGame_Setting_Window.h"
#include "UI_World_NPCSpeechBalloon.h"
#include "UI_WeaponSection_Selected.h"
#include "UI_ImajinnSection_Vehicle.h"
#include "UI_Emoticon_SpeechBalloon.h"
#include "UI_InGame_Setting_OpenBtn.h"
#include "UI_SkillSection_Background.h"
#include "UI_ImajinnSection_Emoticon.h"
#include "UI_InGame_Setting_RadioBtn.h"
#include "UI_Emoticon_BalloonEmoticon.h"
#include "UI_MonsterHP_ElementalFrame.h"
#include "UI_SkillSection_Interaction.h"
#include "UI_SkillSection_ClassicSkill.h"
#include "UI_ImajinnSection_Background.h"
#include "UI_SkillSection_SpecialSkill.h"
#include "UI_InGame_Setting_RadioGroup.h"
#include "UI_SkillSection_CoolTimeFrame.h"
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

CCharacter* CUI_Manager::Get_Character()
{
	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pPlayer)
		return nullptr;

	CCharacter* pCharacter = pPlayer->Get_Character();
	if (nullptr == pCharacter)
		return nullptr;

	return pCharacter;
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

void CUI_Manager::Set_MonsterDescForUI(CMonster* pOwner, _bool bActive)
{
	// 몬스터의 정보를 받아 상단 HPBar UI를 세팅한다.
	// 플레이어의 타겟이 있는지 확인하고 nullptr이면 Active False 후 return;

	if (nullptr == pOwner)
		return;
	if (pOwner->Is_Dead() || pOwner->Is_ReserveDead())
		return;

	m_pHPBarOwner = pOwner;

//	CMonster::MONSTER_STAT StatDesc = {};
//	ZeroMemory(&StatDesc, sizeof(CMonster::MONSTER_STAT));
//
//	memcpy(&StatDesc, &(m_pHPBarOwner->Get_Stat()), sizeof(CMonster::MONSTER_STAT));

	m_pMonsterElemental->Set_MonsterLevel(m_pHPBarOwner->Get_Stat().iLv);
	m_pMonsterHPBack->Set_MonsterName(m_pHPBarOwner->Get_KorName());
	m_pMonsterHPBar->Set_MonsterInfo(m_pHPBarOwner);
	m_pMonsterElemental->Set_ElementalType(m_pHPBarOwner->Get_Stat().eElementType);

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



void CUI_Manager::Set_MainDialogue(_tchar* pszName, _tchar* pszText)
{
	if (nullptr == m_pDialogWindow)
		return;

	m_pDialogWindow->Set_Name(pszName);
	m_pDialogWindow->Set_Text(pszText);

	if (false == m_pDialogWindow->Get_Active())
	{
		//CCamera_Manager::GetInstance()->Get_CurCamera()->Set_CanInput(false);
		m_pDialogWindow->Set_Active(true);
		OnOff_GamePlaySetting(false);
	}
}

void CUI_Manager::Set_MiniDialogue(wstring strName, wstring strContents)
{
	if (nullptr == m_pDialogMini)
		return;

	m_pDialogMini->Set_Name(strName);
	m_pDialogMini->Set_Contents(strContents);
}

void CUI_Manager::Set_BattleDialogue(wstring strContents)
{
	if (nullptr == m_pDialogBattle)
		return;

	m_pDialogBattle->Set_Contents(strContents);
}

void CUI_Manager::Set_QuestPopup(const wstring& strQuestType, const wstring& strTitle, const wstring& strContents)
{
	if (m_QuestPopUp[0] == nullptr)
		return;

	if (-1 == m_QuestPopUp[0]->Get_NumOfQuest() || 5 <= m_QuestPopUp[0]->Get_NumOfQuest())
		return;

	m_QuestPopUp[0]->Set_Contents(strQuestType, strTitle, strContents);

	// 퀘스트 개수를 알아낸 다음 그에 맞게 상태 갱신을 해준다.
	Resize_QuestPopup();
}

void CUI_Manager::Update_QuestPopup(const wstring& strPreTitle, const wstring& strQuestType, const wstring& strTitle, const wstring& strContents)
{
	// 퀘스트를 찾아서 그 퀘스트의 내용을 수정한다.
	if (m_QuestPopUp[0] == nullptr)
		return;

	if (0 >= m_QuestPopUp[0]->Get_NumOfQuest())
		return;

	m_QuestPopUp[0]->Update_QuestContents(strPreTitle, strQuestType, strTitle, strContents);
	Resize_QuestPopup();
}

void CUI_Manager::Clear_QuestPopup(const wstring& strTitle)
{
	if (m_QuestPopUp[0] == nullptr)
		return;

	if (0 > m_QuestPopUp[0]->Get_NumOfQuest())
		return;

	m_QuestPopUp[0]->Clear_Quest(strTitle);

	Resize_QuestPopup();
}

void CUI_Manager::Resize_QuestPopup()
{
	// 퀘스트 개수를 알아낸 다음 그에 맞게 상태 갱신을 해준다.
	_int iNum = m_QuestPopUp[0]->Get_NumOfQuest();

	if (0 > iNum || 4 < iNum)
		return;

	if (0 == m_QuestPopUp[0]->Get_NumOfQuest())
	{
		m_pBtnQuest->Set_TextureIndex(0); // 텍스처 인덱스를 0으로 바꾸고
		for (auto& iter : m_QuestPopUp)
			iter->Set_Active(false);

		return;
	}

	if (0 < m_QuestPopUp[0]->Get_NumOfQuest() && false == m_pBtnQuest->Get_Active())
		return;

	switch (iNum)
	{
	case 1:
		// 켜야될 것
		if (!m_pBtnQuest->Get_Active())
			return;

		//if (!m_QuestPopUp[0]->Get_Active())
			m_QuestPopUp[0]->Set_Active(true);
		//if (!m_QuestPopUp[4]->Get_Active())
			m_QuestPopUp[4]->Set_Active(true);
		//if (!m_QuestPopUp[8]->Get_Active())
			m_QuestPopUp[8]->Set_Active(true);

		//꺼야될 것
		m_QuestPopUp[1]->Set_Active(false);
		m_QuestPopUp[5]->Set_Active(false);
		m_QuestPopUp[2]->Set_Active(false);
		m_QuestPopUp[6]->Set_Active(false);
		m_QuestPopUp[3]->Set_Active(false);
		m_QuestPopUp[7]->Set_Active(false);
		break;

	case 2:
		if (!m_pBtnQuest->Get_Active())
			return;
		//켜야될 것
			m_QuestPopUp[0]->Set_Active(true);
			m_QuestPopUp[4]->Set_Active(true);
			m_QuestPopUp[8]->Set_Active(true);
			m_QuestPopUp[1]->Set_Active(true);
			m_QuestPopUp[5]->Set_Active(true);

		//꺼야될 것
		m_QuestPopUp[2]->Set_Active(false);
		m_QuestPopUp[6]->Set_Active(false);
		m_QuestPopUp[3]->Set_Active(false);
		m_QuestPopUp[7]->Set_Active(false);
		break;

	case 3:
		if (!m_pBtnQuest->Get_Active())
			return;
		//켜야될 것
		m_QuestPopUp[0]->Set_Active(true);
		m_QuestPopUp[4]->Set_Active(true);
		m_QuestPopUp[8]->Set_Active(true);
		m_QuestPopUp[1]->Set_Active(true);
		m_QuestPopUp[5]->Set_Active(true);
		m_QuestPopUp[2]->Set_Active(true);
		m_QuestPopUp[6]->Set_Active(true);

		//꺼야될 것
		m_QuestPopUp[3]->Set_Active(false);
		m_QuestPopUp[7]->Set_Active(false);
		break;

	case 4:
		if (!m_pBtnQuest->Get_Active())
			return;
		//켜야될 것
		m_QuestPopUp[0]->Set_Active(true);
		m_QuestPopUp[4]->Set_Active(true);
		m_QuestPopUp[8]->Set_Active(true);
		m_QuestPopUp[1]->Set_Active(true);
		m_QuestPopUp[5]->Set_Active(true);
		m_QuestPopUp[2]->Set_Active(true);
		m_QuestPopUp[6]->Set_Active(true);
		m_QuestPopUp[3]->Set_Active(true);
		m_QuestPopUp[7]->Set_Active(true);
		break;
	}

	m_pBtnQuest->Set_TextureIndex(1);
	m_QuestPopUp[8]->Move_BottomFrame(iNum);
}

_int CUI_Manager::Get_QuestNum()
{
	if (nullptr == m_QuestPopUp[0])
		return -1;

	return m_QuestPopUp[0]->Get_NumOfQuest();
}

_int CUI_Manager::Get_SelectedCharacter()
{
	if (_uint(LEVELID::LEVEL_LOBBY) != GI->Get_CurrentLevel())
		return -1;

	for (_uint i = 0; i < CUI_BtnCharacterSelect::UI_SELECTBTN_CHARACTER::UICHARACTERBTN_END; i++)
	{
		if (m_ClickedPlayer[i]->Get_Active())
			return i;
	}

	return -1;
}

_float CUI_Manager::Get_DistanceofMovement_SkillBG()
{
	if (nullptr == m_pSkillBG)
		return 0.f;

	if (true == m_pSkillBG->Get_Active())
		return m_pSkillBG->Get_Distance();

	return 0.f;
}

_bool CUI_Manager::Get_MovementComplete_SkillBG()
{
	if (nullptr == m_pSkillBG)
		return false;

	return m_pSkillBG->Get_MovementComplete();
}

_bool CUI_Manager::Is_Dialog_Active()
{
	if (nullptr == m_pDialogWindow)
		return false;

	return m_pDialogWindow->Get_Active();
}

_bool CUI_Manager::Is_DefaultSettingOn()
{
	// 게임 기본 세팅이 켜져있는지 확인해준다 -> UI매니저에서 Clone되지 않은 객체의 OnOff를 제어하기 위해서.
	if (nullptr == m_pPlayerStatus)
		return false;

	return m_pPlayerStatus->Get_Active();
}

_bool CUI_Manager::Is_NicknameSettingComplete()
{
	// 닉네임 세팅창이 켜져있다면 false를 return한다.

	if (m_pSetNickBG->Get_Active())
		return false;
	else
		return true;
}

_bool CUI_Manager::Is_QuestRewardWindowOff()
{
	if (nullptr == m_QuestReward[0])
		return false;

	return m_QuestReward[0]->Get_Active();
}

_bool CUI_Manager::Is_LoadingDone()
{
	CGameObject* pProgressBar = nullptr;
	pProgressBar = GI->Find_GameObject(LEVELID::LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("UI_Loading_Progress_Bar"));
	if (nullptr == pProgressBar)
		return false;

	if (nullptr == dynamic_cast<CUI_Loading_ProgressBar*>(pProgressBar))
		return false;

	return dynamic_cast<CUI_Loading_ProgressBar*>(pProgressBar)->Is_LoadingDone();
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

	if (nullptr != m_pUIVeil)
		m_pUIVeil->Tick(fTimeDelta);
}

void CUI_Manager::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pUICursor)
	{
		m_pUICursor->LateTick(fTimeDelta);
		m_pUICursor->Render(); // Temp
	}

	if (nullptr != m_pUIVeil)
	{
		m_pUIVeil->LateTick(fTimeDelta);
		m_pUIVeil->Render();
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

HRESULT CUI_Manager::Ready_Veils(LEVELID eID)
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

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Fade"), &UIDesc, &pVeil)))
			return E_FAIL;

		m_pUIFade = dynamic_cast<CUI_Fade*>(pVeil);
		if (nullptr == m_pUIFade)
			return E_FAIL;

		Safe_AddRef(m_pUIFade);
	}
	else
	{
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pUIFade)))
			return E_FAIL;
		Safe_AddRef(m_pUIFade);
	}

	return S_OK;
}

HRESULT CUI_Manager::Ready_Dummy()
{
	CGameObject* pDummy = nullptr;

	switch (g_eLoadCharacter)
	{
	case Client::SWORDMAN_CH:
	{
		pDummy = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_CharacterDummy_Swordman"), LAYER_TYPE::LAYER_CHARACTER);
		if (nullptr == pDummy)
			return E_FAIL;
		m_pDummy_Swordman = dynamic_cast<CUI_CharacterDummy*>(pDummy);
	}
	break;
	case Client::DESTROYER_CH:
	{
		pDummy = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_CharacterDummy_Destroyer"), LAYER_TYPE::LAYER_CHARACTER);
		if (nullptr == pDummy)
			return E_FAIL;
		m_pDummy_Destroyer = dynamic_cast<CUI_CharacterDummy*>(pDummy);
	}
	break;
	case Client::ENGINEER_CH:
	{
		pDummy = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_CharacterDummy_Engineer"), LAYER_TYPE::LAYER_CHARACTER);
		if (nullptr == pDummy)
			return E_FAIL;
		m_pDummy_Engineer = dynamic_cast<CUI_CharacterDummy*>(pDummy);
	}
	break;
	case Client::ALL_CH:
	{
		pDummy = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_CharacterDummy_Swordman"), LAYER_TYPE::LAYER_CHARACTER);
		if (nullptr == pDummy)
			return E_FAIL;
		m_pDummy_Swordman = dynamic_cast<CUI_CharacterDummy*>(pDummy);

		pDummy = nullptr;
		pDummy = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_CharacterDummy_Destroyer"), LAYER_TYPE::LAYER_CHARACTER);
		if (nullptr == pDummy)
			return E_FAIL;
		m_pDummy_Destroyer = dynamic_cast<CUI_CharacterDummy*>(pDummy);

		pDummy = nullptr;
		pDummy = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_CharacterDummy_Engineer"), LAYER_TYPE::LAYER_CHARACTER);
		if (nullptr == pDummy)
			return E_FAIL;
		m_pDummy_Engineer = dynamic_cast<CUI_CharacterDummy*>(pDummy);
	}
	break;
	default:
		break;
	}

	CGameObject* pMap = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Map_CostumRoom"), LAYER_TYPE::LAYER_PROP);
	if (nullptr == pMap)
		return E_FAIL;
	m_pCustomMap = dynamic_cast<CUI_CostumeTab_Map*>(pMap);

	CGameObject* pMirror = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Map_Mirror"), LAYER_TYPE::LAYER_PROP);
	if (nullptr == pMirror)
		return E_FAIL;
	m_pCostumeMirror = dynamic_cast<CMirror*>(pMirror);

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

	//UI_Loading_MainLogo
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 1024.f * 0.2f;
	UIDesc.fCY = 512.f * 0.2f;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f) - 25.f;
	UIDesc.fY = (UIDesc.fCY * 0.5f) + 25.f;

	CGameObject* pMainLogo = nullptr;
	if (FAILED(GI->Add_GameObject(LEVEL_LOADING, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Loading_MainLogo_Text"), &UIDesc, &pMainLogo)))
		return E_FAIL;
	if (nullptr == pMainLogo)
		return E_FAIL;

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

HRESULT CUI_Manager::Ready_GameObject(LEVELID eID)
{
	if(FAILED(Ready_Dummy()))
		return E_FAIL;

	if (FAILED(Ready_Veils(eID)))
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
//	UIDesc.fCX = 820.f * 0.35f;
//	UIDesc.fCY = 348.f * 0.4f;
	UIDesc.fCX = 1000.f * 0.4f;
	UIDesc.fCY = 348.f * 0.45f;
	UIDesc.fX = g_iWinSizeX * 0.5f + 50.f;
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
	UIDesc.fX = g_iWinSizeX * 0.5f - 225.f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f + fOffset);

	CGameObject* pPortrait = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Dialog_PortraitFrame"), &UIDesc, &pPortrait)))
		return E_FAIL;
	m_Portrait.push_back(dynamic_cast<CUI_Dialog_Portrait*>(pPortrait));
	if (nullptr == pPortrait)
		return E_FAIL;
	Safe_AddRef(pPortrait);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	fOffset = 83.f;
	UIDesc.fCX = 256.f * 0.8f;
	UIDesc.fCY = 256.f * 0.8f;
	UIDesc.fX = g_iWinSizeX * 0.5f - 225.f;
	UIDesc.fY = g_iWinSizeY - (UIDesc.fCY * 0.5f + fOffset);

	pPortrait = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Dialog_PortraitCharacter"), &UIDesc, &pPortrait)))
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

	_float2 vButtonSize = _float2(300.f * 0.5f * 0.8f, 153.f * 0.5f * 0.8f);

	UIDesc.fCX = vButtonSize.x;
	UIDesc.fCY = vButtonSize.y;
	UIDesc.fX += 70.f;
	UIDesc.fY += 280.f;

	pBtn = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_ChangeBtn"), &UIDesc, &pBtn)))
		return E_FAIL;
	m_pCostumeChange = dynamic_cast<CUI_Costume_ChangeBtn*>(pBtn);
	if (nullptr == m_pCostumeChange)
		return E_FAIL;
	Safe_AddRef(m_pCostumeChange);

#pragma endregion

#pragma region CLASSIC_SKILLS

	m_ClassicSkill.reserve(3);
	m_ClassicFrame.reserve(3);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 256.f * 0.24f;
	UIDesc.fCY = 256.f * 0.24f;
	UIDesc.fX = g_iWinSizeX - 245.5f;
	UIDesc.fY = g_iWinSizeY - 158.f;

	CGameObject* pClassicSkill = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Classic_First"), &UIDesc, &pClassicSkill)))
		return E_FAIL;
	m_ClassicSkill.push_back(dynamic_cast<CUI_SkillSection_ClassicSkill*>(pClassicSkill));
	if (nullptr == pClassicSkill)
		return E_FAIL;
	Safe_AddRef(pClassicSkill);

	UIDesc.fCX = 256.f * 0.23f;
	UIDesc.fCY = 256.f * 0.23f;
	CGameObject* pClassicFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_ClassicSKill_Frame"), &UIDesc, &pClassicFrame)))
		return E_FAIL;
	m_ClassicFrame.push_back(dynamic_cast<CUI_SkillSection_Frame*>(pClassicFrame));
	if (nullptr == pClassicFrame)
		return E_FAIL;
	Safe_AddRef(pClassicFrame);

	UIDesc.fCX = 256.f * 0.24f;
	UIDesc.fCY = 256.f * 0.24f;
	UIDesc.fX = g_iWinSizeX - 220.5f;
	UIDesc.fY = g_iWinSizeY - 227.f;
	pClassicSkill = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Classic_Second"), &UIDesc, &pClassicSkill)))
		return E_FAIL;
	m_ClassicSkill.push_back(dynamic_cast<CUI_SkillSection_ClassicSkill*>(pClassicSkill));
	if (nullptr == pClassicSkill)
		return E_FAIL;
	Safe_AddRef(pClassicSkill);

	UIDesc.fCX = 256.f * 0.23f;
	UIDesc.fCY = 256.f * 0.23f;
	pClassicFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_ClassicSKill_Frame"), &UIDesc, &pClassicFrame)))
		return E_FAIL;
	m_ClassicFrame.push_back(dynamic_cast<CUI_SkillSection_Frame*>(pClassicFrame));
	if (nullptr == pClassicFrame)
		return E_FAIL;
	Safe_AddRef(pClassicFrame);

	UIDesc.fCX = 256.f * 0.24f;
	UIDesc.fCY = 256.f * 0.24f;
	UIDesc.fX = g_iWinSizeX - 153.f;
	UIDesc.fY = g_iWinSizeY - 251.f;
	pClassicSkill = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_SkillSection_Classic_Third"), &UIDesc, &pClassicSkill)))
		return E_FAIL;
	m_ClassicSkill.push_back(dynamic_cast<CUI_SkillSection_ClassicSkill*>(pClassicSkill));
	if (nullptr == pClassicSkill)
		return E_FAIL;
	Safe_AddRef(pClassicSkill);

	UIDesc.fCX = 256.f * 0.23f;
	UIDesc.fCY = 256.f * 0.23f;
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
	m_CostumeWeapon.reserve(2);

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
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_WeaponSection_First"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_CostumeWeapon.push_back(dynamic_cast<CUI_Costume_ItemSlot*>(pSlot));
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
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Costume_WeaponSection_Second"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_CostumeWeapon.push_back(dynamic_cast<CUI_Costume_ItemSlot*>(pSlot));
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

	m_QuestPopUp.reserve(9);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = 400.f * 0.7f;
	UIDesc.fCY = 120.f * 0.65f;
	UIDesc.fX = 210.f;
	UIDesc.fY = 171.f;
	fOffset = 70.f;

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Window"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	UIDesc.fY += fOffset;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Window"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	UIDesc.fY += fOffset;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Window"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	UIDesc.fY += fOffset;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Window"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	//UIDesc.fCX = 300.f * 0.65f;
	UIDesc.fCX = 400.f * 0.65f;
	UIDesc.fCY = 32.f * 0.65f;
	UIDesc.fX = 210.f;
	UIDesc.fY = 135.f;

	CGameObject* pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Top"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pFrame));
	if (nullptr == pFrame)
		return E_FAIL;
	Safe_AddRef(pFrame);

	UIDesc.fX += 5.f;
	UIDesc.fY += fOffset;
	pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Separator_First"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pFrame));
	if (nullptr == pFrame)
		return E_FAIL;
	Safe_AddRef(pFrame);

	UIDesc.fY += fOffset;
	pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Separator_Second"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pFrame));
	if (nullptr == pFrame)
		return E_FAIL;
	Safe_AddRef(pFrame);

	UIDesc.fY += fOffset;
	pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Separator_Third"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pFrame));
	if (nullptr == pFrame)
		return E_FAIL;
	Safe_AddRef(pFrame);

	UIDesc.fX -= 5.f;
	UIDesc.fY += fOffset;
	pFrame = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Bottom"), &UIDesc, &pFrame)))
		return E_FAIL;
	m_QuestPopUp.push_back(dynamic_cast<CUI_PopupQuest*>(pFrame));
	if (nullptr == pFrame)
		return E_FAIL;
	Safe_AddRef(pFrame);

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

	_float fY = 755.f;
	//_float fY = 55.f;
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


	m_Minimap.reserve(2);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 300.f * 0.6f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX - 30.f);
	UIDesc.fY = UIDesc.fCY - 30.f;

	CGameObject* pMinimap = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Minimap_Frame"), &UIDesc, &pMinimap)))
		return E_FAIL;
	m_Minimap.push_back(dynamic_cast<CUI_Minimap_Frame*>(pMinimap));
	if (nullptr == pMinimap)
		return E_FAIL;
	Safe_AddRef(pMinimap);

	pMinimap = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Minimap_FrameArrow"), &UIDesc, &pMinimap)))
		return E_FAIL;
	m_Minimap.push_back(dynamic_cast<CUI_Minimap_Frame*>(pMinimap));
	if (nullptr == pMinimap)
		return E_FAIL;
	Safe_AddRef(pMinimap);


	m_WeaponIcon.reserve(3);

	fOffset = 85.f;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 256.f * 0.15f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = 1380.f;
	UIDesc.fY = 575.f;

	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_First"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_WeaponIcon.push_back(dynamic_cast<CUI_WeaponSection_Weapon*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);

	UIDesc.fX += fOffset;
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_Second"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_WeaponIcon.push_back(dynamic_cast<CUI_WeaponSection_Weapon*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);

	UIDesc.fX += fOffset;
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_Third"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_WeaponIcon.push_back(dynamic_cast<CUI_WeaponSection_Weapon*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);


	m_WeaponElemental.reserve(3);
	fOffset = 85.f;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 64.f * 0.5f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = 1382.f;
	UIDesc.fY = 540.f;

	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_Elemental_First"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_WeaponElemental.push_back(dynamic_cast<CUI_WeaponSection_Weapon*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);

	UIDesc.fX += fOffset;
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_Elemental_Second"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_WeaponElemental.push_back(dynamic_cast<CUI_WeaponSection_Weapon*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);

	UIDesc.fX += fOffset;
	pSlot = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_Elemental_Third"), &UIDesc, &pSlot)))
		return E_FAIL;
	m_WeaponElemental.push_back(dynamic_cast<CUI_WeaponSection_Weapon*>(pSlot));
	if (nullptr == pSlot)
		return E_FAIL;
	Safe_AddRef(pSlot);


	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 600.f * 0.5f;
	UIDesc.fCY = 250.f * 0.5f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = 200.f;

	CGameObject* pNameTag = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Boss_NameTag"), &UIDesc, &pNameTag)))
		return E_FAIL;
	m_pBossNameTag = dynamic_cast<CUI_Boss_NameTag*>(pNameTag);
	if (nullptr == m_pBossNameTag)
		return E_FAIL;
	Safe_AddRef(m_pBossNameTag);


	m_QuestReward.reserve(3);

	CUI::UI_INFO TitleDesc = {};
	ZeroMemory(&TitleDesc, sizeof(CUI::UI_INFO));
	TitleDesc.fCX = 1400.f * 0.65f;
	TitleDesc.fCY = 296.f * 0.65f;
	TitleDesc.fX = g_iWinSizeX * 0.5f;
	TitleDesc.fY = 200.f;

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Quest_Reward_Title"), &TitleDesc, &pWindow)))
		return E_FAIL;
	m_QuestReward.push_back(dynamic_cast<CUI_Quest_Reward*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);


	CUI::UI_INFO WindowDesc = {};
	ZeroMemory(&WindowDesc, sizeof(CUI::UI_INFO));
	WindowDesc.fCX = 1400.f * 0.65f;
	WindowDesc.fCY = 82.f * 0.65f;
	WindowDesc.fX = g_iWinSizeX * 0.5f;
	WindowDesc.fY = TitleDesc.fY + (TitleDesc.fCY * 0.5f);

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Quest_Reward_Window"), &WindowDesc, &pWindow)))
		return E_FAIL;
	m_QuestReward.push_back(dynamic_cast<CUI_Quest_Reward*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);


	CUI::UI_INFO BottomDesc = {};
	ZeroMemory(&BottomDesc, sizeof(CUI::UI_INFO));
	BottomDesc.fCX = 1400.f * 0.65f;
	BottomDesc.fCY = 296.f * 0.65f;
	BottomDesc.fX = g_iWinSizeX * 0.5f;
	BottomDesc.fY = 200.f;

	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Quest_Reward_Bottom"), &BottomDesc, &pWindow)))
		return E_FAIL;
	m_QuestReward.push_back(dynamic_cast<CUI_Quest_Reward*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Default_TutorialWindow"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_pTutorial = dynamic_cast<CUI_Tutorial_Window*>(pWindow);
	if (nullptr == m_pTutorial)
		return E_FAIL;
	Safe_AddRef(m_pTutorial);

	m_QuestItems.reserve(4);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 64.f * 0.5f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX * 0.5f - (UIDesc.fCX * 0.5f + 5.f);
	UIDesc.fY = 285.f;
	CGameObject* pItem = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Quest_Reward_Item"), &UIDesc, &pItem)))
		return E_FAIL;
	m_QuestItems.push_back(dynamic_cast<CUI_Quest_Reward_Item*>(pItem));
	if (nullptr == pItem)
		return E_FAIL;
	Safe_AddRef(pItem);
	pItem->Set_ObjectTag(TEXT("UI_Quest_Reward_Item_First"));

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 64.f * 0.5f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX * 0.5f + (UIDesc.fCX * 0.5f + 5.f);
	UIDesc.fY = 285.f;
	pItem = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Quest_Reward_Item"), &UIDesc, &pItem)))
		return E_FAIL;
	m_QuestItems.push_back(dynamic_cast<CUI_Quest_Reward_Item*>(pItem));
	if (nullptr == pItem)
		return E_FAIL;
	Safe_AddRef(pItem);
	pItem->Set_ObjectTag(TEXT("UI_Quest_Reward_Item_Second"));
	m_QuestItems[1]->Set_Type(CUI_Quest_Reward_Item::UI_QUESTREWARD_ITEM::REWARD_COIN); // Gara

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 400.f * 0.5f;
	UIDesc.fCY = 93.f * 0.5f;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.15f;
	CGameObject* pText = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_MapName_Text_Renewal"), &UIDesc, &pText)))
		return E_FAIL;
	m_pMapNameText = dynamic_cast<CUI_MapName_Text*>(pText);
	if (nullptr == m_pMapNameText)
		return E_FAIL;
	Safe_AddRef(m_pMapNameText);


	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 325.f;
	UIDesc.fCY = 163.f;
	UIDesc.fX = UIDesc.fCX * 0.5f + 10.f;
	UIDesc.fY = 250.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Dialog_BattleWindow"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_pDialogBattle = dynamic_cast<CUI_Dialog_BattleWindow*>(pWindow);
	if (nullptr == m_pDialogBattle)
		return E_FAIL;
	Safe_AddRef(m_pDialogBattle);


	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Vignette_Ice"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_pIceVignette = dynamic_cast<CUI_Vignette_Ice*>(pWindow);
	if (nullptr == m_pIceVignette)
		return E_FAIL;
	Safe_AddRef(m_pIceVignette);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = g_iWinSizeX;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Vignette_Fire"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_pFireVignette = dynamic_cast<CUI_Vignette_Fire*>(pWindow);
	if (nullptr == m_pFireVignette)
		return E_FAIL;
	Safe_AddRef(m_pFireVignette);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 600.f;
	UIDesc.fCY = g_iWinSizeY;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_Window"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_pInGameSetting = dynamic_cast<CUI_InGame_Setting_Window*>(pWindow);
	if (nullptr == m_pInGameSetting)
		return E_FAIL;
	Safe_AddRef(m_pInGameSetting);

	m_GraphicSlot.reserve(3);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 461.f;
	UIDesc.fCY = 27.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 15.f;
	UIDesc.fY = 120.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_Graphic_Slot_First"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_GraphicSlot.push_back(dynamic_cast<CUI_InGame_Setting_Slot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 461.f;
	UIDesc.fCY = 55.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 15.f;
	UIDesc.fY = 180.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_Graphic_Slot_Second"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_GraphicSlot.push_back(dynamic_cast<CUI_InGame_Setting_Slot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 461.f;
	UIDesc.fCY = 377.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 15.f;
	UIDesc.fY = 410.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_Graphic_Slot_Third"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_GraphicSlot.push_back(dynamic_cast<CUI_InGame_Setting_Slot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	m_CameraSlot.reserve(6);
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 461.f;
	UIDesc.fCY = 27.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 15.f;
	UIDesc.fY = 120.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_First"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_CameraSlot.push_back(dynamic_cast<CUI_InGame_Setting_Slot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 461.f;
	UIDesc.fCY = 55.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 15.f;
	UIDesc.fY = 180.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_Second"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_CameraSlot.push_back(dynamic_cast<CUI_InGame_Setting_Slot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 461.f;
	UIDesc.fCY = 159.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 15.f;
	UIDesc.fY = 295.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_Third"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_CameraSlot.push_back(dynamic_cast<CUI_InGame_Setting_Slot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 461.f;
	UIDesc.fCY = 55.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 15.f;
	UIDesc.fY = 415.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_Fourth"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_CameraSlot.push_back(dynamic_cast<CUI_InGame_Setting_Slot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 461.f;
	UIDesc.fCY = 55.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 15.f;
	UIDesc.fY = 480.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_Fifth"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_CameraSlot.push_back(dynamic_cast<CUI_InGame_Setting_Slot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 461.f;
	UIDesc.fCY = 55.f;
	UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 15.f;
	UIDesc.fY = 545.f;
	pWindow = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_Sixth"), &UIDesc, &pWindow)))
		return E_FAIL;
	m_CameraSlot.push_back(dynamic_cast<CUI_InGame_Setting_Slot*>(pWindow));
	if (nullptr == pWindow)
		return E_FAIL;
	Safe_AddRef(pWindow);

	fOffset = 10.f;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fCX = 64.f * 0.8f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX - (UIDesc.fCX * 0.5f) - fOffset - 5.f;
	UIDesc.fY = (UIDesc.fCY * 1.5f) + fOffset * 6.f;

	pButton = nullptr;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Ingame_Setting_OpenBtn"), &UIDesc, &pButton)))
		return E_FAIL;
	m_pBtnInGameSetting = dynamic_cast<CUI_InGame_Setting_OpenBtn*>(pButton);
	if (nullptr == m_pBtnInGameSetting)
		return E_FAIL;
	Safe_AddRef(m_pBtnInGameSetting);


	return S_OK;
}

HRESULT CUI_Manager::Ready_GameObjectToLayer(LEVELID eID)
{
	switch (g_eLoadCharacter)
	{
	case Client::SWORDMAN_CH:
	{
		if (nullptr == m_pDummy_Swordman)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pDummy_Swordman)))
			return E_FAIL;
		Safe_AddRef(m_pDummy_Swordman);
	}
	break;
	case Client::DESTROYER_CH:
	{
		if (nullptr == m_pDummy_Destroyer)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pDummy_Destroyer)))
			return E_FAIL;
		Safe_AddRef(m_pDummy_Destroyer);
	}
	break;
	case Client::ENGINEER_CH:
	{
		if (nullptr == m_pDummy_Engineer)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pDummy_Engineer)))
			return E_FAIL;
		Safe_AddRef(m_pDummy_Engineer);
	}
	break;
	case Client::ALL_CH:
	{
		if (nullptr == m_pDummy_Swordman)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pDummy_Swordman)))
			return E_FAIL;
		Safe_AddRef(m_pDummy_Swordman);

		if (nullptr == m_pDummy_Destroyer)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pDummy_Destroyer)))
			return E_FAIL;
		Safe_AddRef(m_pDummy_Destroyer);

		if (nullptr == m_pDummy_Engineer)
			return E_FAIL;
		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pDummy_Engineer)))
			return E_FAIL;
		Safe_AddRef(m_pDummy_Engineer);
	}
	break;
	default:
		break;
	}

	if (nullptr == m_pCustomMap)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pCustomMap)))
		return E_FAIL;
	Safe_AddRef(m_pCustomMap);

	if (nullptr == m_pCostumeMirror)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pCostumeMirror)))
		return E_FAIL;
	Safe_AddRef(m_pCostumeMirror);

	if (nullptr == m_pUIFade)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pUIFade)))
		return E_FAIL;
	Safe_AddRef(m_pUIFade);

	if (nullptr == m_pUIMapName)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pUIMapName)))
		return E_FAIL;
	Safe_AddRef(m_pUIMapName);

	for (auto& iter : m_LevelUp)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pPlayerStatus)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pPlayerStatus)))
		return E_FAIL;
	Safe_AddRef(m_pPlayerStatus);

	if (nullptr == m_pBtnShowMenu)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBtnShowMenu)))
		return E_FAIL;
	Safe_AddRef(m_pBtnShowMenu);

	if (nullptr == m_pBtnCamera)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBtnCamera)))
		return E_FAIL;
	Safe_AddRef(m_pBtnCamera);

	if (nullptr == m_pBtnShowMinimap)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBtnShowMinimap)))
		return E_FAIL;
	Safe_AddRef(m_pBtnShowMinimap);

	if (nullptr == m_pBtnInven)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBtnInven)))
		return E_FAIL;
	Safe_AddRef(m_pBtnInven);

	if (nullptr == m_pBtnQuest)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBtnQuest)))
		return E_FAIL;
	Safe_AddRef(m_pBtnQuest);

	if (nullptr == m_pWindowQuest)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pWindowQuest)))
		return E_FAIL;
	Safe_AddRef(m_pWindowQuest);

	if (nullptr == m_pMainBG)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pMainBG)))
		return E_FAIL;
	Safe_AddRef(m_pMainBG);

	for (auto& iter : m_MainMenuBtn)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pBtnClose)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBtnClose)))
		return E_FAIL;
	Safe_AddRef(m_pBtnClose);

	for (auto& iter : m_SubMenuChar)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_MenuSeparator)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_SubMenuEquip)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_SubMenuPet)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_SubMenuShop)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pWorldMapBG)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pWorldMapBG)))
		return E_FAIL;
	Safe_AddRef(m_pWorldMapBG);

	if (nullptr == m_pSkillBG)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pSkillBG)))
		return E_FAIL;
	Safe_AddRef(m_pSkillBG);

	if (nullptr == m_pImajinnBG)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pImajinnBG)))
		return E_FAIL;
	Safe_AddRef(m_pImajinnBG);

	if (nullptr == m_pDefaultBG)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pDefaultBG)))
		return E_FAIL;
	Safe_AddRef(m_pDefaultBG);

	for (auto& iter : m_PlayerEXP)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pMonsterHPBack)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pMonsterHPBack)))
		return E_FAIL;
	Safe_AddRef(m_pMonsterHPBack);

	if (nullptr == m_pMonsterHPBar)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pMonsterHPBar)))
		return E_FAIL;
	Safe_AddRef(m_pMonsterHPBar);

	if (nullptr == m_pMonsterFrame)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pMonsterFrame)))
		return E_FAIL;
	Safe_AddRef(m_pMonsterFrame);

	if (nullptr == m_pMonsterElemental)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pMonsterElemental)))
		return E_FAIL;
	Safe_AddRef(m_pMonsterElemental);

	if (nullptr == m_pDialogWindow)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pDialogWindow)))
		return E_FAIL;
	Safe_AddRef(m_pDialogWindow);

	if (nullptr == m_pDialogMini)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pDialogMini)))
		return E_FAIL;
	Safe_AddRef(m_pDialogMini);

	for (auto& iter : m_Portrait)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pTabMenuTitle)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pTabMenuTitle)))
		return E_FAIL;
	Safe_AddRef(m_pTabMenuTitle);

	for (auto& iter : m_Portrait)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_CostumeBtn)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_CostumeClickedBtn)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pCostumeBox)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pCostumeBox)))
		return E_FAIL;
	Safe_AddRef(m_pCostumeBox);

	if (nullptr == m_pCostumeChange)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pCostumeChange)))
		return E_FAIL;
	Safe_AddRef(m_pCostumeChange);

	for (auto& iter : m_ClassicSkill)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_ClassicFrame)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_SpecialSkill)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_SpecialFrame)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_ItemQuickslot)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_CostumeCloth)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_CostumeHairAcc)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_CostumeWeapon)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_QuestPopUp)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pCameraAnnounce)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pCameraAnnounce)))
		return E_FAIL;
	Safe_AddRef(m_pCameraAnnounce);

	if (nullptr == m_pEmoticonWindow)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pEmoticonWindow)))
		return E_FAIL;
	Safe_AddRef(m_pEmoticonWindow);

	if (nullptr == m_pBalloon)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBalloon)))
		return E_FAIL;
	Safe_AddRef(m_pBalloon);

	if (nullptr == m_pInvenBox)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pInvenBox)))
		return E_FAIL;
	Safe_AddRef(m_pInvenBox);

	for (auto& iter : m_InvenBtn)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_InvenClickedBtn)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_InvenSlots)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_SkillWindow)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_SkillBtn)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_SkillClickedBtn)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_ClassSkillSlot)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_SpecialSkillSlot)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pSkillDesc)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pSkillDesc)))
		return E_FAIL;
	Safe_AddRef(m_pSkillDesc);

	if (nullptr == m_pCostumeAnnounce)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pCostumeAnnounce)))
		return E_FAIL;
	Safe_AddRef(m_pCostumeAnnounce);

	if (nullptr == m_pBossInfo)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBossInfo)))
		return E_FAIL;
	Safe_AddRef(m_pBossInfo);

	if (nullptr == m_pBossHPBack)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBossHPBack)))
		return E_FAIL;
	Safe_AddRef(m_pBossHPBack);

	if (nullptr == m_pBossHPBar)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBossHPBar)))
		return E_FAIL;
	Safe_AddRef(m_pBossHPBar);

	for (auto& iter : m_Milepost)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_Minimap)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_WeaponIcon)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_WeaponElemental)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pBossNameTag)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBossNameTag)))
		return E_FAIL;
	Safe_AddRef(m_pBossNameTag);

	for (auto& iter : m_QuestReward)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pTutorial)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pTutorial)))
		return E_FAIL;
	Safe_AddRef(m_pTutorial);

	for (auto& iter : m_QuestItems)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pMapNameText)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pMapNameText)))
		return E_FAIL;
	Safe_AddRef(m_pMapNameText);

	if (nullptr == m_pDialogBattle)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pDialogBattle)))
		return E_FAIL;
	Safe_AddRef(m_pDialogBattle);

	if (nullptr == m_pIceVignette)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pIceVignette)))
		return E_FAIL;
	Safe_AddRef(m_pIceVignette);

	if (nullptr == m_pFireVignette)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pFireVignette)))
		return E_FAIL;
	Safe_AddRef(m_pFireVignette);

	if (nullptr == m_pInGameSetting)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pInGameSetting)))
		return E_FAIL;
	Safe_AddRef(m_pInGameSetting);

	for (auto& iter : m_GraphicSlot)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	for (auto& iter : m_CameraSlot)
	{
		if (nullptr == iter)
			return E_FAIL;

		if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, iter)))
			return E_FAIL;
		Safe_AddRef(iter);
	}

	if (nullptr == m_pBtnInGameSetting)
		return E_FAIL;
	if (FAILED(GI->Add_GameObject(eID, LAYER_TYPE::LAYER_UI, m_pBtnInGameSetting)))
		return E_FAIL;
	Safe_AddRef(m_pBtnInGameSetting);

	return S_OK;
}

HRESULT CUI_Manager::Ready_BossHPBar(CBoss* pBoss, void* pArg)
{
	if (nullptr == pBoss)
		return E_FAIL;

	m_pBossInfo->Set_Owner(pBoss);
	m_pBossHPBar->Set_Owner(pBoss);

	m_bBossActive = true;

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

	case LEVELID::LEVEL_KINGDOMHALL:
		Tick_GamePlayLevel(fTimeDelta);
		break;

	case LEVELID::LEVEL_EVERMORE:
		Tick_GamePlayLevel(fTimeDelta);
		break;

	case LEVELID::LEVEL_ICELAND:
		Tick_GamePlayLevel(fTimeDelta);
		break;

	default:
		Tick_GamePlayLevel(fTimeDelta);
		break;
	}

	return S_OK;
}

HRESULT CUI_Manager::Tick_LobbyLevel(_float fTimeDelta)
{
	if (false == m_pSetNickBG->Get_Active())
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
	}

	return S_OK;
}

HRESULT CUI_Manager::Tick_GamePlayLevel(_float fTimeDelta)
{
	switch (m_eCurPlayer)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (nullptr != m_pDummy_Swordman)
			m_pDummy_Swordman->Tick(fTimeDelta);
		break;
	case CHARACTER_TYPE::DESTROYER:
		if (nullptr != m_pDummy_Destroyer)
			m_pDummy_Destroyer->Tick(fTimeDelta);
		break;
	case CHARACTER_TYPE::ENGINEER:
		if (nullptr != m_pDummy_Engineer)
			m_pDummy_Engineer->Tick(fTimeDelta);
		break;
	default:
		break;
	}
	m_pCustomMap->Tick(fTimeDelta);
	m_pCostumeMirror->Tick(fTimeDelta);

	if (nullptr != m_pUIMapName)
	{
		if (m_pUIMapName->Get_Active())
		{
			// MapName이 Active상태라면
			if (nullptr != m_pMapNameText)
			{
				if (!m_pMapNameText->Get_Active())
					m_pMapNameText->Set_Active(true);
				else
					m_pMapNameText->Set_Alpha(m_pUIMapName->Get_Alpha());
			}
		}
		else
		{
			if (nullptr != m_pMapNameText)
			{
				if (m_pMapNameText->Get_Active())
					m_pMapNameText->Set_Active(false);
			}
		}
	}

	if (m_pCostumeBox->Get_Active())
		Update_CostumeBtn();

	else
	{
		if (m_pCostumeAnnounce->Get_Active())
			m_pCostumeAnnounce->Set_Active(false);
	}

	if (KEY_TAP(KEY::F9))
	{
		if (nullptr != m_pTutorial)
		{
			if(true == m_pTutorial->Get_Active())
				m_pTutorial->Set_Active(false);
			else
				m_pTutorial->Set_Active(true);
		}
	}

	// 퀘스트 팝업창 켜지도록 함. Gara
//	if (!m_QuestPopUp[0]->Get_Active())
//	{
//		if (Is_FadeFinished() && Is_DefaultSettingOn())
//		{
//			if (1 <= Get_QuestNum())
//			{
//				OnOff_QuestPopup(true);
//			}
//		}
//	}

	return S_OK;
}

HRESULT CUI_Manager::LateTick_GamePlayLevel(_float fTimeDelta)
{
	switch (m_eCurPlayer)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (nullptr != m_pDummy_Swordman)
			m_pDummy_Swordman->LateTick(fTimeDelta);
		break;
	case CHARACTER_TYPE::DESTROYER:
		if (nullptr != m_pDummy_Destroyer)
			m_pDummy_Destroyer->LateTick(fTimeDelta);
		break;
	case CHARACTER_TYPE::ENGINEER:
		if (nullptr != m_pDummy_Engineer)
			m_pDummy_Engineer->LateTick(fTimeDelta);
		break;
	default:
		break;
	}

	m_pCustomMap->LateTick(fTimeDelta);
	m_pCostumeMirror->LateTick(fTimeDelta);

	return S_OK;
}

void CUI_Manager::LateTick_Dummy(_float fTimeDelta)
{
	switch (m_eCurPlayer)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (nullptr != m_pDummy_Swordman)
			m_pDummy_Swordman->LateTick(fTimeDelta);
		break;
	case CHARACTER_TYPE::DESTROYER:
		if (nullptr != m_pDummy_Destroyer)
			m_pDummy_Destroyer->LateTick(fTimeDelta);
		break;
	case CHARACTER_TYPE::ENGINEER:
		if (nullptr != m_pDummy_Engineer)
			m_pDummy_Engineer->LateTick(fTimeDelta);
		break;
	default:
		break;
	}
	m_pCustomMap->LateTick(fTimeDelta);
	m_pCostumeMirror->LateTick(fTimeDelta);
}

HRESULT CUI_Manager::Render_GamePlayLevel()
{
	//m_pDummy_Swordman->Render();

	return S_OK;
}

void CUI_Manager::Render_Dummy()
{
	switch (m_eCurPlayer)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (nullptr != m_pDummy_Swordman)
			m_pDummy_Swordman->Render();
		break;
	case CHARACTER_TYPE::DESTROYER:
		if (nullptr != m_pDummy_Destroyer)
			m_pDummy_Destroyer->Render();
		break;
	case CHARACTER_TYPE::ENGINEER:
		if (nullptr != m_pDummy_Engineer)
			m_pDummy_Engineer->Render();
		break;
	default:
		break;
	}
	m_pCustomMap->Render();
	m_pCostumeMirror->Render();
	m_pCostumeMirror->Render_Reflect();
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
//	if (nullptr == m_pUIFade)
//		return false;
//
//	return m_pUIFade->Get_Finish();
	return m_bAddText;
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
	m_ClickedPlayer[iIndex]->Update_LobbyDummy();

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
		if (m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_WEAPON]->Get_Active())
			m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_WEAPON]->Set_Active(false);
	}
	else if (iIndex == _uint(CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIRACC))
	{
		if (m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_CLOTH]->Get_Active())
			m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_CLOTH]->Set_Active(false);
		if (m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_WEAPON]->Get_Active())
			m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_WEAPON]->Set_Active(false);
	}
	else if (iIndex == _uint(CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_WEAPON))
	{
		if (m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_CLOTH]->Get_Active())
			m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_CLOTH]->Set_Active(false);
		if (m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIRACC]->Get_Active())
			m_CostumeClickedBtn[CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_HAIRACC]->Set_Active(false);
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

void CUI_Manager::Update_SkillSection(_uint iSkillType, _uint iSectionType)
{
	/*
	m_ClassicSkill;
	m_SpecialSkill;
	*/
	switch (iSkillType)
	{
	case 0: // CLASSIC
		if (!m_ClassicSkill[iSectionType]->Is_Clicked())
			m_ClassicSkill[iSectionType]->Set_Clicked(true);
		break;

	case 1: // SPECIAL
		if (!m_SpecialSkill[iSectionType]->Is_Clicked())
			m_SpecialSkill[iSectionType]->Set_Clicked(true);
		break;
	}
}

void CUI_Manager::Update_ClothSlotState(_uint iSectionType, _uint iSlotIndex)
{
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

	case 2: // Weapon
		m_CostumeWeapon[iSlotIndex]->Set_Clicked(true); // 선택되었다.

		for (_uint i = 0; i < m_CostumeWeapon.size(); ++i)
		{
			if (i == iSlotIndex)
				continue;

			m_CostumeWeapon[i]->Set_Clicked(false);
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

	CModel* pPart = nullptr;
	CWeapon* pWeapon = nullptr;

	_int iSlotIndex = -1;

	switch (iIndex)
	{
	case 0:
		pPart = pCharacter->Get_PartModel(PART_TYPE::BODY); // 현재 착용하고 있는 파츠
		if (nullptr == pPart)
			return;

		for (auto& iter : m_CostumeCloth)
		{
			if (pPart->Get_Name() == iter->Get_PartTag())
				iSlotIndex = iter->Get_SlotIndex(); // 현재 장착중인 아이템이 있는 슬롯 인덱스
		}
		if (0 <= iSlotIndex && m_CostumeCloth.size() >= iSlotIndex)
		{
			CUI::UI_INFO UIDesc = {};
			UIDesc = m_CostumeCloth[iSlotIndex]->Get_UI_Info();
			m_pCostumeAnnounce->Set_AnnouncePosition(_float2(UIDesc.fX + 100.f, UIDesc.fY + 15.f));

			for (auto& iter : m_CostumeCloth)
			{
				if (iter->Get_Clicked())
				{
					if (iSlotIndex == iter->Get_SlotIndex())
					{
						// 현재 장착하고있는 것과 코스튬 태그가 같은 슬롯이 Clicked되면 버튼의 인덱스를 변경한다.
						m_pCostumeChange->Set_TextureIndex(1);
					}
					else
					{
						// 그렇지 않으면 기존 '장착'버튼이 선택된다.
						m_pCostumeChange->Set_TextureIndex(0);
					}
				}
			}
		}
		break;

	case 2: // m_CostumeHairAcc
		pPart = pCharacter->Get_PartModel(PART_TYPE::HEAD);
		if (nullptr == pPart)
		{
			m_pCostumeAnnounce->Set_AnnouncePosition(_float2(9999.f, 9999.f));
			return;
		}

		for (auto& iter : m_CostumeHairAcc)
		{
			if (pPart->Get_Name() == iter->Get_PartTag())
				iSlotIndex = iter->Get_SlotIndex(); // 현재 장착중인 아이템이 있는 슬롯 인덱스
		}
		if (0 <= iSlotIndex && m_CostumeHairAcc.size() >= iSlotIndex)
		{
			CUI::UI_INFO UIDesc = {};
			UIDesc = m_CostumeHairAcc[iSlotIndex]->Get_UI_Info();
			m_pCostumeAnnounce->Set_AnnouncePosition(_float2(UIDesc.fX + 100.f, UIDesc.fY + 15.f));

			for (auto& iter : m_CostumeHairAcc)
			{
				if (iter->Get_Clicked())
				{
					if (iSlotIndex == iter->Get_SlotIndex())
					{
						// 현재 장착하고있는 것과 코스튬 태그가 같은 슬롯이 Clicked되면 버튼의 인덱스를 변경한다.
						m_pCostumeChange->Set_TextureIndex(1);
					}
					else
					{
						// 그렇지 않으면 기존 '장착'버튼이 선택된다.
						m_pCostumeChange->Set_TextureIndex(0);
					}
				}
			}
		}
		else if (iSlotIndex == -1)
		{
			m_pCostumeAnnounce->Set_AnnouncePosition(_float2(9999.f, 9999.f));
		}
		break;

	case 5: //m_CostumeWeapon
		pWeapon = pCharacter->Get_Weapon(); // 현재 착용하고 있는 무기
		if (nullptr == pWeapon)
			return;

		pPart = pWeapon->Get_WeaponModelCom();
		if (nullptr == pPart)
			return;

		for (auto& iter : m_CostumeWeapon)
		{
			if (pPart->Get_Name() == iter->Get_PartTag())
				iSlotIndex = iter->Get_SlotIndex(); // 현재 장착중인 아이템이 있는 슬롯 인덱스. 없으면 -1.
		}
		if (0 <= iSlotIndex && m_CostumeWeapon.size() >= iSlotIndex)
		{
			CUI::UI_INFO UIDesc = {};
			UIDesc = m_CostumeWeapon[iSlotIndex]->Get_UI_Info();
			m_pCostumeAnnounce->Set_AnnouncePosition(_float2(UIDesc.fX + 100.f, UIDesc.fY + 15.f));

			for (auto& iter : m_CostumeWeapon)
			{
				if (iter->Get_Clicked())
				{
					if (iSlotIndex == iter->Get_SlotIndex())
					{
						// 현재 장착하고있는 것과 코스튬 태그가 같은 슬롯이 Clicked되면 버튼의 인덱스를 변경한다.
						m_pCostumeChange->Set_TextureIndex(1);
					}
					else
					{
						// 그렇지 않으면 기존 '장착'버튼이 선택된다.
						m_pCostumeChange->Set_TextureIndex(0);
					}
				}
			}
		}
		else if (iSlotIndex == -1)
		{
			// 착용된 무기 스킨이 없다.
			m_pCostumeAnnounce->Set_AnnouncePosition(_float2(9999.f, 9999.f));
		}
		break;
	}
	
//	CCharacter_Manager::GetInstance()->Get_PartModel(pCharacter->Get_CharacterType(),
//		const PART_TYPE & ePartType, const wstring & strPartTag);
}

void CUI_Manager::Update_WeaponSelectionIcon(_uint iSlotNum)
{
	if (2 < iSlotNum || nullptr == m_pSkillBG)
		return;

	m_pSkillBG->Update_SelectionIcon(iSlotNum);
}

void CUI_Manager::Update_IceVignette()
{
	if (nullptr == m_pIceVignette)
		return;

	m_pIceVignette->Decrease_TextureIndex();
}

void CUI_Manager::Update_CostumeModel(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType, const wstring& strPartTag)
{
	CModel* pParts = CCharacter_Manager::GetInstance()->Get_PartModel(eCharacterType, ePartType, strPartTag);
	//CModel* pParts = CCharacter_Manager::GetInstance()->Get_PartModel(eCharacterType, ePartType, iIndex);
	if (nullptr == pParts)
		return;

	switch (m_eCurPlayer)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (nullptr != m_pDummy_Swordman)
			m_pDummy_Swordman->Set_PartModel(ePartType, pParts);
		break;
	case CHARACTER_TYPE::DESTROYER:
		if (nullptr != m_pDummy_Destroyer)
			m_pDummy_Destroyer->Set_PartModel(ePartType, pParts);
		break;
	case CHARACTER_TYPE::ENGINEER:
		if (nullptr != m_pDummy_Engineer)
			m_pDummy_Engineer->Set_PartModel(ePartType, pParts);
		break;
	default:
		break;
	}
}

void CUI_Manager::Update_CostumeWeaponModel(const CHARACTER_TYPE& eCharacterType, const wstring& strPartTag)
{
	// 장착 해제를 눌러도 계속 코스튬이 착용되는 오류가 발생함.

	CModel* pWeaponModel = nullptr;
	pWeaponModel = CWeapon_Manager::GetInstance()->Get_WeaponModel(eCharacterType, strPartTag);
	if (nullptr == pWeaponModel)
		return;
	
	// UI Dummy의 Weapon을 교체한다.
	CUI_Dummy_Weapon* pWeapon = nullptr;
	switch (m_eCurPlayer)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (nullptr != m_pDummy_Swordman)
			pWeapon = m_pDummy_Swordman->Get_Weapon();
		break;
	case CHARACTER_TYPE::DESTROYER:
		if (nullptr != m_pDummy_Destroyer)
			pWeapon = m_pDummy_Destroyer->Get_Weapon();
		break;
	case CHARACTER_TYPE::ENGINEER:
		if (nullptr != m_pDummy_Engineer)
			pWeapon = m_pDummy_Engineer->Get_Weapon();
		break;
	default:
		break;
	}
	if (nullptr == pWeapon)
		return;
	pWeapon->Set_WeaponModelCom(pWeaponModel);
}

void CUI_Manager::Set_CostumeModel()
{
	// 현재 Active되어있는 CostumeButton.. 혹은 Slot을 찾아내서
	// PartType을 파악하고
	CModel* pWeaponModel = nullptr;

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

	if (0 > iIndex || 5 < iIndex)
		return;

	wstring strPartTag = TEXT("");

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
		pCharacter->Set_PartModel(PART_TYPE::BODY,
			CCharacter_Manager::GetInstance()->Get_PartModel(pCharacter->Get_CharacterType(), PART_TYPE::BODY, strPartTag));
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
		pCharacter->Set_PartModel(PART_TYPE::HEAD,
			CCharacter_Manager::GetInstance()->Get_PartModel(pCharacter->Get_CharacterType(), PART_TYPE::HEAD, strPartTag));
	}
	else if (iIndex == CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_WEAPON)
	{
		for (auto& iter : m_CostumeWeapon)
		{
			if (iter->Get_Clicked())
			{
				strPartTag = iter->Get_PartTag();
			}
		}

		if (strPartTag == TEXT(""))
			return;

		// Weapon
		pWeaponModel = CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCurPlayer, strPartTag);
		if (nullptr == pWeaponModel)
			return;

		// UI Dummy의 Weapon을 교체한다.
		CUI_Dummy_Weapon* pWeapon = nullptr;
		switch (m_eCurPlayer)
		{
		case CHARACTER_TYPE::SWORD_MAN:
			if (nullptr != m_pDummy_Swordman)
				pWeapon = m_pDummy_Swordman->Get_Weapon();
			break;
		case CHARACTER_TYPE::DESTROYER:
			if (nullptr != m_pDummy_Destroyer)
				pWeapon = m_pDummy_Destroyer->Get_Weapon();
			break;
		case CHARACTER_TYPE::ENGINEER:
			if (nullptr != m_pDummy_Engineer)
				pWeapon = m_pDummy_Engineer->Get_Weapon();
			break;
		default:
			break;
		}
		if (nullptr == pWeapon)
			return;
		pWeapon->Set_WeaponModelCom(pWeaponModel);

		// Character의 Weapon을 교체한다.
		CWeapon* pRealWeapon = pCharacter->Get_Weapon();
		if (nullptr == pRealWeapon)
			return;
		pRealWeapon->Set_WeaponModelCom(pWeaponModel);
	}

//	if (PART_TYPE::PART_END == eType)
//		return;

	// 머리와 의상에 해당됨
//	pCharacter->Set_PartModel(eType,
//		CCharacter_Manager::GetInstance()->Get_PartModel(pCharacter->Get_CharacterType(), eType, strPartTag));
}

void CUI_Manager::TakeOff_CostumeModel()
{
	_uint iIndex;
	wstring strModelTag;
	ELEMENTAL_TYPE eElementalType = ELEMENTAL_TYPE::ELEMENTAL_END;

	for (auto& iter : m_CostumeClickedBtn)
	{
		if (iter->Get_Active()) // 선택이 되었으면 활성화 되어있다.
		{
			iIndex = iter->Get_CostumeType();
		}
	}

	if (iIndex != CUI_Costume_Btn::UI_COSTUMEBTN::COSTUME_WEAPON)
		return;

	CModel* pWeaponModel = nullptr;

	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (pPlayer == nullptr)
		return;
	CCharacter* pCharacter = pPlayer->Get_Character();
	if (pCharacter == nullptr)
		return;

	eElementalType = pCharacter->Get_ElementalType();
	if (eElementalType >= ELEMENTAL_TYPE::ELEMENTAL_END)
		return;

	switch (m_eCurPlayer)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (eElementalType == FIRE)
			strModelTag = TEXT("Sword_Fire02");
		else if (eElementalType == WATER)
			strModelTag = TEXT("Sword_Water02");
		else if (eElementalType == WOOD)
			strModelTag = TEXT("Sword_Wood02");
		else
			return;
		break;

	case CHARACTER_TYPE::DESTROYER:
		if (eElementalType == FIRE)
			strModelTag = TEXT("Hammer_Fire02");
		else if (eElementalType == WATER)
			strModelTag = TEXT("Hammer_Water02");
		else if (eElementalType == WOOD)
			strModelTag = TEXT("Hammer_Wood02");
		else
			return;
		break;

	case CHARACTER_TYPE::ENGINEER:
		break;
	}

	for (auto& iter : m_CostumeWeapon)
	{
		iter->Set_Clicked(false);
	}

	// Weapon
	pWeaponModel = CWeapon_Manager::GetInstance()->Get_WeaponModel(m_eCurPlayer, strModelTag);
	if (nullptr == pWeaponModel)
		return;

	// UI Dummy의 Weapon을 교체한다.
	CUI_Dummy_Weapon* pWeapon = nullptr;
	switch (m_eCurPlayer)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		if (nullptr != m_pDummy_Swordman)
			pWeapon = m_pDummy_Swordman->Get_Weapon();
		break;
	case CHARACTER_TYPE::DESTROYER:
		if (nullptr != m_pDummy_Destroyer)
			pWeapon = m_pDummy_Destroyer->Get_Weapon();
		break;
	case CHARACTER_TYPE::ENGINEER:
		if (nullptr != m_pDummy_Engineer)
			pWeapon = m_pDummy_Engineer->Get_Weapon();
		break;
	default:
		break;
	}
	if (nullptr == pWeapon)
		return;
	pWeapon->Set_WeaponModelCom(pWeaponModel);

	// Character의 Weapon을 교체한다.
	CWeapon* pRealWeapon = pCharacter->Get_Weapon();
	if (nullptr == pRealWeapon)
		return;
	pRealWeapon->Set_WeaponModelCom(pWeaponModel);

	// 장착해제버튼을 장착으로 바꾼다.
	m_pCostumeChange->Set_TextureIndex(0);
}

void CUI_Manager::Set_MouseCursor(_uint iIndex)
{
	if (nullptr == m_pUICursor)
		return;

	if (0 > iIndex || 1 < iIndex)
		return;

	m_pUICursor->Set_TextureIndex(iIndex);
}

void CUI_Manager::Hide_MouseCursor(_bool bHide)
{
	if (nullptr == m_pUICursor)
		return;

	if (bHide)
	{
		GI->Lock_Mouse();
		m_pUICursor->Set_Active(false);
	}
	else
	{
		GI->UnLock_Mouse();
		m_pUICursor->Set_Active(true);
	}
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

	if (nullptr != m_pInGameSetting)
	{
		if (m_pInGameSetting->Get_Active())
			OnOff_InGameSetWindow(false);

		OnOff_CloseButton(false); // ClostBtn도 없앤다.
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

		if (nullptr != m_pWorldMapBG)
		{
			if (m_pWorldMapBG->Get_Active())
				OnOff_WorldMap(false);
		}
	}

	return S_OK;
}

void CUI_Manager::Use_JumpBtn()
{
	if (nullptr == m_pSkillBG)
		return;

	CUI* pBtnJump = m_pSkillBG->Get_Child(TEXT("Prototype_GameObject_UI_SkillSection_BtnJump"));
	if (nullptr == pBtnJump)
		return;

	dynamic_cast<CUI_SkillSection_BtnJump*>(pBtnJump)->Set_Resizable(true);
}

void CUI_Manager::Use_RollBtn()
{
	if (nullptr == m_pSkillBG)
		return;

	CUI* pBtnRoll = m_pSkillBG->Get_Child(TEXT("Prototype_GameObject_UI_SkillSection_BtnRoll"));
	if (nullptr == pBtnRoll)
		return;

	dynamic_cast<CUI_SkillSection_BtnRoll*>(pBtnRoll)->Set_Resizable(true);
}

void CUI_Manager::Use_AttackBtn()
{
	if (nullptr == m_pSkillBG)
		return;

	CUI* pAttack = m_pSkillBG->Get_Child(TEXT("Prototype_GameObject_UI_SkillSection_DefaultAttack"));
	if (nullptr == pAttack)
		return;

	dynamic_cast<CUI_WeaponSection_DefaultWeapon*>(pAttack)->Set_ResizeStart(true);
}

void CUI_Manager::Use_ClassSkillSlot(_uint iSlotNum)
{
	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_TOOL)
		return;

	if (0 > iSlotNum || 2 < iSlotNum)
		return;

	m_ClassicSkill[iSlotNum]->Set_Clicked(true);
}

void CUI_Manager::Use_ActiveSkillSlot(_uint iSlotNum)
{
	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_TOOL)
		return;

	if (0 > iSlotNum || 2 < iSlotNum)
		return;

	m_SpecialSkill[iSlotNum]->Set_Clicked(true);
}

void CUI_Manager::Use_BurstSkillSlot(_uint iSlotNum)
{
	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_TOOL)
		return;

	if (nullptr == m_pSkillBG)
		return;

	m_pSkillBG->Use_BurstSkill(iSlotNum);
}

void CUI_Manager::Change_ElementalType(ELEMENTAL_TYPE eElementalType)
{
	if (ELEMENTAL_TYPE::ELEMENTAL_END <= eElementalType)
		return;

	m_eElemental = eElementalType;
	Set_ElementalType();
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

		OnOff_LobbyUIs(true);
	}

	return S_OK;
}

void CUI_Manager::OnOff_LobbyUIs(_bool bOnOff)
{
	if (bOnOff)
	{
		for (auto& iter : m_Basic)
			iter->Set_Active(true);
		for (auto& iter : m_Buttons)
			iter->Set_Active(true);
		for (auto& iter : m_UnclickedPlayer)
			iter->Set_Active(true);
		m_pBtnShowSetting->Set_Active(true);
	}
	else
	{
		for (auto& iter : m_Basic)
			iter->Set_Active(false);
		for (auto& iter : m_Buttons)
			iter->Set_Active(false);
		for (auto& iter : m_UnclickedPlayer)
			iter->Set_Active(false);
		m_pBtnShowSetting->Set_Active(false);
	}
}

HRESULT CUI_Manager::OnOff_GamePlaySetting(_bool bOnOff)
{
	if (bOnOff) // On
	{
		OnOff_TextUI(true);
		OnOff_QuestPopup(true);

		for (auto& iter : m_Milepost)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}
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
		m_pBtnInGameSetting->Set_Active(true);

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
		for (auto& iter : m_WeaponIcon)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}
		for (auto& iter : m_WeaponElemental)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}

		m_pImajinnBG->Set_Active(true);

		OnOff_MonsterHP(true);
		OnOff_BossHP(true);

		// EXP Bar를 보여준다.
		for (auto& iter : m_PlayerEXP)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}
	}
	else // Off
	{
		OnOff_TextUI(false);

		for (auto& iter : m_Milepost)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

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
		m_pBtnInGameSetting->Set_Active(false);

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
		for (auto& iter : m_WeaponIcon)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_WeaponElemental)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		m_pImajinnBG->Set_Active(false);
		m_pCameraAnnounce->Set_Active(false);
	
		OnOff_MonsterHP(false);
		OnOff_MiniMap(false);

		// EXP Bar를 감춘다
		for (auto& iter : m_PlayerEXP)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		// MiniQuestPopup이 켜져있다면 끈다
		OnOff_QuestPopup(false);
		OnOff_BossHP(false);
	}

	return S_OK;
}

void CUI_Manager::Hide_GamePlaySetting(_bool bHide)
{

	// Todo m_pBtnInGameSetting 추가해야함

	if (bHide) // 아이콘을 숨긴다
	{
		// Active false를 해야하는 UI
		OnOff_TextUI(false);
		OnOff_MonsterHP(false);
		OnOff_BossHP(false);
		for (auto& iter : m_Milepost)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		// Hide false를 해야하는 UI
		// 좌측
		m_pPlayerStatus->Hide_UI(true);
		for (auto& iter : m_ItemQuickslot)
		{
			if (nullptr != iter)
				iter->Hide_UI(true);
		}
		m_pBtnCamera->Hide_UI(true);
		m_pBtnQuest->Hide_UI(true);
		// 우측
		m_pBtnShowMenu->Hide_UI(true);
		m_pBtnInven->Hide_UI(true);
		m_pBtnShowMinimap->Hide_UI(true);
		m_pSkillBG->Hide_UI(true);
		for (auto& iter : m_ClassicSkill) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(true);
		}
		for (auto& iter : m_ClassicFrame) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(true);
		}
		for (auto& iter : m_SpecialSkill) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(true);
		}
		for (auto& iter : m_SpecialFrame) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(true);
		}
		for (auto& iter : m_WeaponIcon) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(true);
		}
		for (auto& iter : m_WeaponElemental) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(true);
		}
		//아래
		m_pImajinnBG->Hide_UI(true);
		for (auto& iter : m_PlayerEXP)
		{
			if (nullptr != iter)
				iter->Hide_UI(true);
		}
	}
	else // 아이콘을 드러낸다
	{
		// Active true를 해야하는 UI
		//OnOff_TextUI(true); Player Status로 이동
		OnOff_MonsterHP(true);
		OnOff_BossHP(true);
		for (auto& iter : m_Milepost)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}

		// Hide false를 해야하는 UI -> 숨기지 않는다
		// 좌측
		m_pPlayerStatus->Hide_UI(false);
		for (auto& iter : m_ItemQuickslot)
		{
			if (nullptr != iter)
				iter->Hide_UI(false);
		}
		m_pBtnCamera->Hide_UI(false);
		m_pBtnQuest->Hide_UI(false);
		// 우측
		m_pBtnShowMenu->Hide_UI(false);
		m_pBtnInven->Hide_UI(false);
		m_pBtnShowMinimap->Hide_UI(false);
		m_pSkillBG->Hide_UI(false);
		for (auto& iter : m_ClassicSkill) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(false);
		}
		for (auto& iter : m_ClassicFrame) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(false);
		}
		for (auto& iter : m_SpecialSkill) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(false);
		}
		for (auto& iter : m_SpecialFrame) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(false);
		}
		for (auto& iter : m_WeaponIcon) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(false);
		}
		for (auto& iter : m_WeaponElemental) // SkillBG랑 같이가도록 한다
		{
			if (nullptr != iter)
				iter->Hide_UI(false);
		}
		//아래
		m_pImajinnBG->Hide_UI(false);
		for (auto& iter : m_PlayerEXP)
		{
			if (nullptr != iter)
				iter->Hide_UI(false);
		}
	}
}

HRESULT CUI_Manager::OnOff_MainMenu(_bool bOnOff)
{
	if (nullptr == m_pMainBG)
		return E_FAIL;

	if (bOnOff) // On
	{
		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("UI_Fx_Comm_Slide_Open_MainMenu_1.mp3"), CHANNELID::SOUND_UI, GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

		// 텍스트 관련 제어
		OnOff_TextUI(false); // Off

		// 카메라 제어
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam)
			pFollowCam->Set_CanInput(false);

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
		// 텍스트 관련 제어
		OnOff_TextUI(true); // On

		// 카메라 제어
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr != pFollowCam)
			pFollowCam->Set_CanInput(true);

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
		if (!Is_DefaultSettingOn())
		{
			_int iNum = m_QuestPopUp[0]->Get_NumOfQuest();

			if (0 == iNum)
			{
				OnOff_QuestPopup(false);
				return S_OK;
			}

			if (1 <= iNum)
			{
				if (0 == m_pBtnQuest->Get_TextureIndex())
					m_pBtnQuest->Set_Click(true);

				if (m_QuestPopUp[0]->Get_Active())
					return S_OK;

					m_QuestPopUp[0]->Set_Active(true);
					m_QuestPopUp[4]->Set_Active(true);
					m_QuestPopUp[8]->Set_Active(true);

				if (2 <= iNum)
				{
						m_QuestPopUp[1]->Set_Active(true);
						m_QuestPopUp[5]->Set_Active(true);

					if (3 <= iNum)
					{
						m_QuestPopUp[2]->Set_Active(true);
						m_QuestPopUp[6]->Set_Active(true);

						if (4 <= iNum)
						{
							m_QuestPopUp[3]->Set_Active(true);
							m_QuestPopUp[7]->Set_Active(true);
						}
					}
				}

				m_QuestPopUp[8]->Move_BottomFrame(iNum);
			}
		}
	}
	else
	{
		for (auto iter : m_QuestPopUp)
		{
			if (iter != nullptr)
				iter->Set_Active(false);
		}

		if (1 == m_pBtnQuest->Get_TextureIndex())
			m_pBtnQuest->Set_Click(false);
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
		if (m_bBossActive)
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

void CUI_Manager::OnOff_BossNameTag(_bool bOnOff)
{
	if (m_pBossNameTag == nullptr)
		return;

	if (bOnOff)
	{
		if (!m_pBossNameTag->Get_Active())
		{
			m_pBossNameTag->Set_Active(true);
		}
	}
	else
	{
		if (m_pBossNameTag->Get_Active())
		{
			m_pBossNameTag->Set_Active(false);
		}
	}
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

void CUI_Manager::OnOff_LevelUp(_bool bOnOff, _uint iLevel)
{
	if (bOnOff)
	{
		m_LevelUp[1]->SetUp_Level(iLevel);

		for (auto& iter : m_LevelUp)
		{
			iter->Set_Active(true);
		}
	}
	else
	{
		for (auto& iter : m_LevelUp)
		{
			iter->Set_Active(false);
		}
	}
}

HRESULT CUI_Manager::OnOff_SubMenu(_bool bOnOff, _uint iMagicNum)
{
	if (0 > iMagicNum || 9 < iMagicNum)
		return E_FAIL;

	if (bOnOff) // true -> On
	{
		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("UI_Fx_Comm_Slide_Open_SubMenu_1.mp3"), CHANNELID::SOUND_UI, GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

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

void CUI_Manager::OnOff_MapName(_bool bOnOff, const wstring& strMapName)
{
	if (nullptr == m_pUIMapName)
		return;
	if (nullptr == m_pMapNameText)
		return;
	if (true == bOnOff && m_pUIMapName->Get_Active()) // MapName이 보여지는 중이라면 return한다.
		return;

	if (bOnOff) // On
	{
		// 맵네임 Type을 세팅하는 곳
		if (TEXT("남문 광장") == strMapName)
		{
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::EVERMORE_SOUTH);
		}
		else if (TEXT("동문 광장") == strMapName)
		{
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::EVERMORE_EARTH);
		}
		else if (TEXT("서문 광장") == strMapName)
		{
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::EVERMORE_WEST);
		}
		else if (TEXT("에스타바니아 왕성") == strMapName)
		{
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::KINGDOM);
		}
		else if (TEXT("코에루코 설원") == strMapName)
		{
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::ICELAND_DEFAULT);
		}
		else if (TEXT("만남의 나무") == strMapName)
		{
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::ICELAND_MEETINGTREE);
		}
		else if (TEXT("사무라 마을") == strMapName)
		{
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::ICELAND_SAMURAVILLAGE);
		}
		else if (TEXT("최후의 쉼터") == strMapName)
		{
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::ICELAND_SHELTER);
		}
		else if (TEXT("얼어붙은 유령 숲") == strMapName)
		{
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::ICELAND_GHOSTFOREST);
		}
		else if (TEXT("그늘진 저지대") == strMapName)
		{
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::ICELAND_SHADOW);
		}
		else
		{
			// Temp
			m_pMapNameText->Set_Type(CUI_MapName_Text::UI_MAPNAME_TEXT::ICELAND_ICEVILLAGE);
		}

		m_pUIMapName->Set_Active(true);
	}
	else // Off
	{
		// 사용 안할수도있음.
	}
}

HRESULT CUI_Manager::OnOff_DialogWindow(_bool bOnOff, _uint iMagicNum)
{
	if (iMagicNum == 0) // Normal
	{
		if (nullptr == m_pDialogWindow)
			return E_FAIL;

		if (bOnOff) // On
		{
			if (Is_DefaultSettingOn())
				OnOff_GamePlaySetting(false);

			if (!m_pDialogWindow->Get_Active())
				m_pDialogWindow->Set_Active(true);

		}
		else // Off
		{
			if (!Is_DefaultSettingOn())
			{
				//OnOff_GamePlaySetting(true); // 240119 카메라 쪽에서 열어주도록 수정 (찬)
			}
			
			if (m_pDialogWindow->Get_Active())
				m_pDialogWindow->Set_Active(false);
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

	else if (iMagicNum == 2)
	{
		if (nullptr == m_pDialogBattle)
			return E_FAIL;

		if (bOnOff) // On
		{
			if (!m_pDialogBattle->Get_Active())
				m_pDialogBattle->Set_Active(true);
		}
		else
		{
			if (m_pDialogBattle->Get_Active())
				m_pDialogBattle->Set_Active(false);
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

HRESULT CUI_Manager::OnOff_MiniMap(_bool bOnOff)
{
	if (bOnOff) // 켠다
	{
		for (auto& iter : m_Minimap)
		{
			if (nullptr != iter)
				iter->Set_Active(true);
		}
	}
	else // 끈다
	{
		for (auto& iter : m_Minimap)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}

		if (1 == m_pBtnShowMinimap->Get_TextureIndex())
			m_pBtnShowMinimap->Set_TextureIndex(0);
	}

	return S_OK;
}

void CUI_Manager::OnOff_IceVignette(_bool bOnOff)
{
	if (nullptr == m_pIceVignette)
		return;

	if (true == bOnOff)
	{
		if (false == m_pIceVignette->Get_Active())
			m_pIceVignette->Set_Active(true);
	}
	else
	{
		if (true == m_pIceVignette->Get_Active())
			m_pIceVignette->Set_Active(false);
	}
}

void CUI_Manager::On_FireVignette()
{
	if (nullptr == m_pIceVignette)
		return;

	if (!m_pIceVignette->Get_Active())
		return;

	m_pFireVignette->Set_Active(true);
}

HRESULT CUI_Manager::OnOff_CostumeWindow(_bool bOnOff)
{
	if (bOnOff)
	{
		if (!m_pDefaultBG->Get_Active())
		{
			OnOff_MainMenu(false);

			for (auto& iter : m_CostumeCloth)
			{
				if (nullptr != iter)
					iter->Update_PartsTag();
			}
			for (auto& iter : m_CostumeHairAcc)
			{
				if (nullptr != iter)
					iter->Update_PartsTag();
			}
			for (auto& iter : m_CostumeWeapon)
			{
				if (nullptr != iter)
					iter->Update_PartsTag();
			}

			// 시작할때 더미 캐릭터가 지금 플레이어와 같은 의상을 착용하도록 세팅 해주어야 한다.
			CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
			if (pPlayer == nullptr)
				return E_FAIL;
			CCharacter* pCharacter = pPlayer->Get_Character();
			if (pCharacter == nullptr)
				return E_FAIL;

			CWeapon* pWeapon = nullptr;
			CModel* pPart = nullptr;
			CUI_Dummy_Weapon* pDummyWeapon = nullptr;

			switch (pCharacter->Get_CharacterType())
			{
			case CHARACTER_TYPE::SWORD_MAN:
				if (nullptr != m_pDummy_Swordman)
				{
					m_pDummy_Swordman->Set_PartModel(PART_TYPE::BODY, pCharacter->Get_PartModel(PART_TYPE::BODY));
					m_pDummy_Swordman->Set_PartModel(PART_TYPE::HEAD, pCharacter->Get_PartModel(PART_TYPE::HEAD));
					// 무기도
					pWeapon = pCharacter->Get_Weapon(); // 현재 착용하고 있는 무기
					if (nullptr == pWeapon)
						return E_FAIL;
					pPart = pWeapon->Get_WeaponModelCom();
					if (nullptr == pPart)
						return E_FAIL;
					pDummyWeapon = m_pDummy_Swordman->Get_Weapon();
					if (nullptr == pDummyWeapon)
						return E_FAIL;
					pDummyWeapon->Set_WeaponModelCom(pPart);
				}
				break;
			case CHARACTER_TYPE::DESTROYER:
				if (nullptr != m_pDummy_Destroyer)
				{
					m_pDummy_Destroyer->Set_PartModel(PART_TYPE::BODY, pCharacter->Get_PartModel(PART_TYPE::BODY));
					m_pDummy_Destroyer->Set_PartModel(PART_TYPE::HEAD, pCharacter->Get_PartModel(PART_TYPE::HEAD));
					// 무기도
					pWeapon = pCharacter->Get_Weapon(); // 현재 착용하고 있는 무기
					if (nullptr == pWeapon)
						return E_FAIL;
					pPart = pWeapon->Get_WeaponModelCom();
					if (nullptr == pPart)
						return E_FAIL;
					pDummyWeapon = m_pDummy_Destroyer->Get_Weapon();
					if (nullptr == pDummyWeapon)
						return E_FAIL;
					pDummyWeapon->Set_WeaponModelCom(pPart);
				}
				break;
			case CHARACTER_TYPE::ENGINEER:
				if (nullptr != m_pDummy_Engineer)
				{
					m_pDummy_Engineer->Set_PartModel(PART_TYPE::BODY, pCharacter->Get_PartModel(PART_TYPE::BODY));
					m_pDummy_Engineer->Set_PartModel(PART_TYPE::HEAD, pCharacter->Get_PartModel(PART_TYPE::HEAD));
					// 무기도
					pWeapon = pCharacter->Get_Weapon(); // 현재 착용하고 있는 무기
					if (nullptr == pWeapon)
						return E_FAIL;
					pPart = pWeapon->Get_WeaponModelCom();
					if (nullptr == pPart)
						return E_FAIL;
					pDummyWeapon = m_pDummy_Engineer->Get_Weapon();
					if (nullptr == pDummyWeapon)
						return E_FAIL;
					pDummyWeapon->Set_WeaponModelCom(pPart);
				}
				break;
			default:
				break;
			}

			m_pDefaultBG->Set_Active(true);
			// 이게 옷장

			for (auto& iter : m_CostumeBtn)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			m_pCostumeBox->Set_Active(true);
			m_pCostumeChange->Set_Active(true);
			// 의상 등 버튼을 누를때 Active가 활성화되도록 우선 수정한다.
			//for (auto& iter : m_CostumeItem)
			//{
			//	if (nullptr != iter)
			//		iter->Set_Active(true);
			//}

			m_pTabMenuTitle->Set_TextType(CUI_Text_TabMenu::UI_MENUTITLE::TITLE_COSTUME);
			m_pTabMenuTitle->Set_Active(true);

			switch (pCharacter->Get_CharacterType())
			{
			case CHARACTER_TYPE::SWORD_MAN:
				if (nullptr != m_pDummy_Swordman)
					m_pDummy_Swordman->Set_Active(true);
				break;
			case CHARACTER_TYPE::DESTROYER:
				if (nullptr != m_pDummy_Destroyer)
					m_pDummy_Destroyer->Set_Active(true);
				break;
			case CHARACTER_TYPE::ENGINEER:
				if (nullptr != m_pDummy_Engineer)
					m_pDummy_Engineer->Set_Active(true);
				break;
			default:
				break;
			}
			m_pCustomMap->Set_Active(true);
			m_pCostumeMirror->Set_Active(true);
		}
	}
	else
	{
		CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
		if (pPlayer == nullptr)
			return E_FAIL;
		CCharacter* pCharacter = pPlayer->Get_Character();
		if (pCharacter == nullptr)
			return E_FAIL;

		switch (pCharacter->Get_CharacterType())
		{
		case CHARACTER_TYPE::SWORD_MAN:
			if (nullptr != m_pDummy_Swordman)
				m_pDummy_Swordman->Set_Active(false);
			break;
		case CHARACTER_TYPE::DESTROYER:
			if (nullptr != m_pDummy_Destroyer)
				m_pDummy_Destroyer->Set_Active(false);
			break;
		case CHARACTER_TYPE::ENGINEER:
			if (nullptr != m_pDummy_Engineer)
				m_pDummy_Engineer->Set_Active(false);
			break;
		default:
			break;
		}
		m_pCostumeMirror->Set_Active(false);
		m_pCustomMap->Set_Active(false);
		m_pTabMenuTitle->Set_Active(false);

		OnOff_CostumeSlot(0, false); // false에는 숫자가 영향을 미치지않음. 함수 인자 순서 수정 필요함.

		m_pCostumeChange->Set_Active(false);
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
	m_pCostumeChange->Set_TextureIndex(0);

	if (bOnOff)
	{
		switch (iSection)
		{
		case 0: // Cloth
			for (auto& iter : m_CostumeCloth)
			{
				if (nullptr != iter)
				{
					iter->Set_Active(true);
				}
			}
			for (auto& iter : m_CostumeHairAcc)
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			for (auto& iter : m_CostumeWeapon)
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			break;

		case 2: // HairAcc
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
			for (auto& iter : m_CostumeWeapon)
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			break;

		case 5: // Weapon
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
			for (auto& iter : m_CostumeWeapon)
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
		for (auto& iter : m_CostumeWeapon)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
	}

	return S_OK;
}

void CUI_Manager::OnOff_InGameSetWindow(_bool bOnOff)
{
	if (true == bOnOff)
	{
		OnOff_GamePlaySetting(false);
		m_pInGameSetting->Set_Active(true);

		if (nullptr != m_pBtnClose)
			m_pBtnClose->Set_Active(true);
	}
	else
	{
		for (auto& iter : m_GraphicSlot)
		{
			iter->Set_Active(false);
		}
		for (auto& iter : m_CameraSlot)
		{
			iter->Set_Active(false);
		}

		m_pInGameSetting->Set_Active(false);
		OnOff_GamePlaySetting(true);
	}
}

void CUI_Manager::OnOff_SettingSlot(_uint iSection, _bool bOnOff)
{
	if (bOnOff) // On
	{
		switch (iSection)
		{
		case 0: // 그래픽
			for (auto& iter : m_GraphicSlot)
			{
				if (nullptr != iter)
				{
					iter->Set_Active(true);
				}
			}
			for (auto& iter : m_CameraSlot)
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			break;

		case 1: // 게임
			for (auto& iter : m_GraphicSlot)
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			for (auto& iter : m_CameraSlot)
			{
				if (nullptr != iter)
					iter->Set_Active(true);
			}
			break;

		case 2: // 오디오
			for (auto& iter : m_GraphicSlot)
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			for (auto& iter : m_CameraSlot)
			{
				if (nullptr != iter)
					iter->Set_Active(false);
			}
			break;
		}
	}
	else
	{
		for (auto& iter : m_GraphicSlot)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
		for (auto& iter : m_CameraSlot)
		{
			if (nullptr != iter)
				iter->Set_Active(false);
		}
	}
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

			CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
			if (pPlayer == nullptr)
				return E_FAIL;
			CCharacter* pCharacter = pPlayer->Get_Character();
			if (pCharacter == nullptr)
				return E_FAIL;

			switch (pCharacter->Get_CharacterType())
			{
			case CHARACTER_TYPE::SWORD_MAN:
				if (nullptr != m_pDummy_Swordman)
					m_pDummy_Swordman->Set_Active(true);
				break;
			case CHARACTER_TYPE::DESTROYER:
				if (nullptr != m_pDummy_Destroyer)
					m_pDummy_Destroyer->Set_Active(true);
				break;
			case CHARACTER_TYPE::ENGINEER:
				if (nullptr != m_pDummy_Engineer)
					m_pDummy_Engineer->Set_Active(true);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
		if (pPlayer == nullptr)
			return E_FAIL;
		CCharacter* pCharacter = pPlayer->Get_Character();
		if (pCharacter == nullptr)
			return E_FAIL;

		switch (pCharacter->Get_CharacterType())
		{
		case CHARACTER_TYPE::SWORD_MAN:
			if (nullptr != m_pDummy_Swordman)
				m_pDummy_Swordman->Set_Active(false);
			break;
		case CHARACTER_TYPE::DESTROYER:
			if (nullptr != m_pDummy_Destroyer)
				m_pDummy_Destroyer->Set_Active(false);
			break;
		case CHARACTER_TYPE::ENGINEER:
			if (nullptr != m_pDummy_Engineer)
				m_pDummy_Engineer->Set_Active(false);
			break;
		default:
			break;
		}

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

		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_1.mp3"), CHANNELID::SOUND_UI,
			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));
	}
	else
	{
		if (m_pEmoticonWindow->Get_Active())
			m_pEmoticonWindow->Set_Active(false);

		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("UI_Fx_Comm_Close_1.mp3"), CHANNELID::SOUND_UI,
			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

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

void CUI_Manager::OnOff_TextUI(_bool bOnOff)
{
	// Text가 있는 UI들에게 _bool값을 전달한다.
	if (GI->Get_CurrentLevel() != LEVELID::LEVEL_LOBBY)
	{
		if (nullptr == m_pPlayerStatus)
			return;

		m_pPlayerStatus->Set_TextOnOff(bOnOff);
	}

	m_bAddText = bOnOff;
}

void CUI_Manager::OnOff_QuestRewards(_bool bOnOff, const wstring& strTitle)
{
	if (bOnOff)
	{
		m_QuestReward[0]->Set_Text(strTitle);

		for (auto& iter : m_QuestReward)
			iter->Set_Active(true);
	}
	else
	{
		for (auto& iter : m_QuestReward)
			iter->Set_Active(false);
		for (auto& iter : m_QuestItems)
			iter->Set_Active(false);
	}
}

void CUI_Manager::Set_AlphaToItems()
{
	if (!m_QuestReward[0]->Get_Active())
		return;

	_float fAlpha = m_QuestReward[0]->Get_Alpha();

	for (auto& iter : m_QuestItems)
	{
		iter->Set_Alpha(fAlpha);
	}

}

void CUI_Manager::Show_RewardItems()
{
	for (auto& iter : m_QuestItems)
		iter->Set_Active(true);
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
		CUI_Loading_ProgressBar::Create(m_pDevice, m_pContext, L"UI_Loading_Progress_Back", CUI_Loading_ProgressBar::UIPROG_BACK), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_Progress_Bar"),
		CUI_Loading_ProgressBar::Create(m_pDevice, m_pContext, L"UI_Loading_Progress_Bar", CUI_Loading_ProgressBar::UIPROG_BAR), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_Information"),
		CUI_Loading_Logo::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Loading_MainLogo_Text"),
		CUI_Loading_MainLogo::Create(m_pDevice, m_pContext, CUI_Loading_MainLogo::MAINLOGO_TEXT), LAYER_UI)))
		return E_FAIL;

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

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WorldMap_Icon_PlayerPortrait"),
		CUI_Basic::Create(m_pDevice, m_pContext, L"UI_WorldMap_Icon_Portrait", CUI_Basic::UI_BASIC::WORLDMAP_ICON), LAYER_UI)))
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
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialog_PortraitCharacter"),
		CUI_Dialog_Portrait::Create(m_pDevice, m_pContext, CUI_Dialog_Portrait::UI_PORTRAIT::PORTRAIT_CHARACTER), LAYER_UI)))
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
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_WeaponSection_First"),
		CUI_Costume_ItemSlot::Create(m_pDevice, m_pContext, CUI_Costume_ItemSlot::UI_COSTUME_SECTION::COSTUMESECTION_WEAPON,
			CUI_Costume_ItemSlot::UI_COSTUME_SLOT::COSTUMESLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Costume_WeaponSection_Second"),
		CUI_Costume_ItemSlot::Create(m_pDevice, m_pContext, CUI_Costume_ItemSlot::UI_COSTUME_SECTION::COSTUMESECTION_WEAPON,
			CUI_Costume_ItemSlot::UI_COSTUME_SLOT::COSTUMESLOT_SECOND), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Top"),
		CUI_PopupQuest::Create(m_pDevice, m_pContext, CUI_PopupQuest::UI_QUESTPOPUP::POPUPFRAME_TOP), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Bottom"),
		CUI_PopupQuest::Create(m_pDevice, m_pContext, CUI_PopupQuest::UI_QUESTPOPUP::POPUPFRAME_BOTTOM), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Window"),
		CUI_PopupQuest::Create(m_pDevice, m_pContext, CUI_PopupQuest::UI_QUESTPOPUP::POPUP_WINDOW), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Separator_First"),
		CUI_PopupQuest::Create(m_pDevice, m_pContext,
			CUI_PopupQuest::UI_QUESTPOPUP::POPUP_SEPARATOR,
			CUI_PopupQuest::UI_POPUP_SEPARATOR::SEPARATOR_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Separator_Second"),
		CUI_PopupQuest::Create(m_pDevice, m_pContext,
			CUI_PopupQuest::UI_QUESTPOPUP::POPUP_SEPARATOR,
			CUI_PopupQuest::UI_POPUP_SEPARATOR::SEPARATOR_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_QuestPopup_Frame_Separator_Third"),
		CUI_PopupQuest::Create(m_pDevice, m_pContext,
			CUI_PopupQuest::UI_QUESTPOPUP::POPUP_SEPARATOR,
			CUI_PopupQuest::UI_POPUP_SEPARATOR::SEPARATOR_THIRD), LAYER_UI)))
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

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_World_Interaction_Btn"),
		CUI_World_Interaction::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minimap_Frame"),
		CUI_Minimap_Frame::Create(m_pDevice, m_pContext, CUI_Minimap_Frame::UI_MINIMAP::MINIMAP_FRAME), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Minimap_FrameArrow"),
		CUI_Minimap_Frame::Create(m_pDevice, m_pContext, CUI_Minimap_Frame::UI_MINIMAP::MINIMAP_ARROW), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_World_NameTag"),
		CUI_World_NameTag::Create(m_pDevice, m_pContext, CUI_World_NameTag::UI_NAMETAG::NAMETAG_GAMEPLAY), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_First"),
		CUI_WeaponSection_Weapon::Create(m_pDevice, m_pContext,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT::WEAPON_FIRST,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT_TYPE::SLOT_WEAPON), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_Second"),
		CUI_WeaponSection_Weapon::Create(m_pDevice, m_pContext,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT::WEAPON_SECOND,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT_TYPE::SLOT_WEAPON), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_Third"),
		CUI_WeaponSection_Weapon::Create(m_pDevice, m_pContext,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT::WEAPON_THIRD,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT_TYPE::SLOT_WEAPON), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_Elemental_First"),
		CUI_WeaponSection_Weapon::Create(m_pDevice, m_pContext,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT::WEAPON_FIRST,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT_TYPE::SLOT_ELEMENTAL), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_Elemental_Second"),
		CUI_WeaponSection_Weapon::Create(m_pDevice, m_pContext,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT::WEAPON_SECOND,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT_TYPE::SLOT_ELEMENTAL), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSection_WeaponIcon_Elemental_Third"),
		CUI_WeaponSection_Weapon::Create(m_pDevice, m_pContext,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT::WEAPON_THIRD,
			CUI_WeaponSection_Weapon::UI_WEAPONSLOT_TYPE::SLOT_ELEMENTAL), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_CoolTimeFrame_First"),
		CUI_SkillSection_CoolTimeFrame::Create(m_pDevice, m_pContext,
			CUI_SkillSection_CoolTimeFrame::UI_COOLFRAME_TYPE::FRAME_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_CoolTimeFrame_Second"),
		CUI_SkillSection_CoolTimeFrame::Create(m_pDevice, m_pContext,
			CUI_SkillSection_CoolTimeFrame::UI_COOLFRAME_TYPE::FRAME_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillSection_CoolTimeFrame_Third"),
		CUI_SkillSection_CoolTimeFrame::Create(m_pDevice, m_pContext,
			CUI_SkillSection_CoolTimeFrame::UI_COOLFRAME_TYPE::FRAME_THIRD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_WeaponSection_Selected"),
		CUI_WeaponSection_Selected::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Boss_NameTag"),
		CUI_Boss_NameTag::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_NPC_Tag"),
		CUI_World_NPCTag::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Animal_Tag"),
		CUI_World_AnimalTag::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_NPC_SpeechBalloon"),
		CUI_World_NPCSpeechBalloon::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Quest_Reward_Title"),
		CUI_Quest_Reward::Create(m_pDevice, m_pContext, CUI_Quest_Reward::UI_QUESTREWARD::REWARD_TOP), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Quest_Reward_Window"),
		CUI_Quest_Reward::Create(m_pDevice, m_pContext, CUI_Quest_Reward::UI_QUESTREWARD::REWARD_WINDOW), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Quest_Reward_Bottom"),
		CUI_Quest_Reward::Create(m_pDevice, m_pContext, CUI_Quest_Reward::UI_QUESTREWARD::REWARD_BOTTOM), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Default_TutorialWindow"),
		CUI_Tutorial_Window::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Quest_Reward_Item"),
		CUI_Quest_Reward_Item::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_MapName_Text_Renewal"),
		CUI_MapName_Text::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Dialog_BattleWindow"),
		CUI_Dialog_BattleWindow::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Vignette_Ice"),
		CUI_Vignette_Ice::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Vignette_Fire"),
		CUI_Vignette_Fire::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Window"),
		CUI_InGame_Setting_Window::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Tab_Graphic"),
		CUI_InGame_Setting_Tab::Create(m_pDevice, m_pContext, CUI_InGame_Setting_Tab::UI_SETTINGTAB::SETTING_GRAPHIC), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Tab_Game"),
		CUI_InGame_Setting_Tab::Create(m_pDevice, m_pContext, CUI_InGame_Setting_Tab::UI_SETTINGTAB::SETTING_GAME), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Tab_Audio"),
		CUI_InGame_Setting_Tab::Create(m_pDevice, m_pContext, CUI_InGame_Setting_Tab::UI_SETTINGTAB::SETTING_AUDIO), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Graphic_Slot_First"),
		CUI_InGame_Setting_Slot::Create(m_pDevice, m_pContext,
			CUI_InGame_Setting_Slot::UI_SETTING_SECTION::SETTING_GRAPHIC,
			CUI_InGame_Setting_Slot::UI_SLOT_ORDER::SLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Graphic_Slot_Second"),
		CUI_InGame_Setting_Slot::Create(m_pDevice, m_pContext,
			CUI_InGame_Setting_Slot::UI_SETTING_SECTION::SETTING_GRAPHIC,
			CUI_InGame_Setting_Slot::UI_SLOT_ORDER::SLOT_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Graphic_Slot_Third"),
		CUI_InGame_Setting_Slot::Create(m_pDevice, m_pContext,
			CUI_InGame_Setting_Slot::UI_SETTING_SECTION::SETTING_GRAPHIC,
			CUI_InGame_Setting_Slot::UI_SLOT_ORDER::SLOT_THIRD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_First"),
		CUI_InGame_Setting_Slot::Create(m_pDevice, m_pContext,
			CUI_InGame_Setting_Slot::UI_SETTING_SECTION::SETTING_CAMERA,
			CUI_InGame_Setting_Slot::UI_SLOT_ORDER::SLOT_FIRST), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_Second"),
		CUI_InGame_Setting_Slot::Create(m_pDevice, m_pContext,
			CUI_InGame_Setting_Slot::UI_SETTING_SECTION::SETTING_CAMERA,
			CUI_InGame_Setting_Slot::UI_SLOT_ORDER::SLOT_SECOND), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_Third"),
		CUI_InGame_Setting_Slot::Create(m_pDevice, m_pContext,
			CUI_InGame_Setting_Slot::UI_SETTING_SECTION::SETTING_CAMERA,
			CUI_InGame_Setting_Slot::UI_SLOT_ORDER::SLOT_THIRD), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_Fourth"),
		CUI_InGame_Setting_Slot::Create(m_pDevice, m_pContext,
			CUI_InGame_Setting_Slot::UI_SETTING_SECTION::SETTING_CAMERA,
			CUI_InGame_Setting_Slot::UI_SLOT_ORDER::SLOT_FOURTH), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_Fifth"),
		CUI_InGame_Setting_Slot::Create(m_pDevice, m_pContext,
			CUI_InGame_Setting_Slot::UI_SETTING_SECTION::SETTING_CAMERA,
			CUI_InGame_Setting_Slot::UI_SLOT_ORDER::SLOT_FIFTH), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Camera_Slot_Sixth"),
		CUI_InGame_Setting_Slot::Create(m_pDevice, m_pContext,
			CUI_InGame_Setting_Slot::UI_SETTING_SECTION::SETTING_CAMERA,
			CUI_InGame_Setting_Slot::UI_SLOT_ORDER::SLOT_SIXTH), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_OpenBtn"),
		CUI_InGame_Setting_OpenBtn::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton"),
		CUI_InGame_Setting_RadioBtn::Create(m_pDevice, m_pContext, CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_ONBTN), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton"),
		CUI_InGame_Setting_RadioBtn::Create(m_pDevice, m_pContext, CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_OFFBTN), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_Natural"),
		CUI_InGame_Setting_RadioGroup::Create(m_pDevice, m_pContext, CUI_InGame_Setting_RadioGroup::UI_RADIOGROUP::GRAPHIC_NATURAL), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_Shadow"),
		CUI_InGame_Setting_RadioGroup::Create(m_pDevice, m_pContext, CUI_InGame_Setting_RadioGroup::UI_RADIOGROUP::GRAPHIC_SHADOW), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_Outline"),
		CUI_InGame_Setting_RadioGroup::Create(m_pDevice, m_pContext, CUI_InGame_Setting_RadioGroup::UI_RADIOGROUP::GRAPHIC_OUTLINE), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_Bloom"),
		CUI_InGame_Setting_RadioGroup::Create(m_pDevice, m_pContext, CUI_InGame_Setting_RadioGroup::UI_RADIOGROUP::GRAPHIC_BLOOM), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_Blur"),
		CUI_InGame_Setting_RadioGroup::Create(m_pDevice, m_pContext, CUI_InGame_Setting_RadioGroup::UI_RADIOGROUP::GRAPHIC_BLUR), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_SSAO"),
		CUI_InGame_Setting_RadioGroup::Create(m_pDevice, m_pContext, CUI_InGame_Setting_RadioGroup::UI_RADIOGROUP::GRAPHIC_SSAO), LAYER_UI)))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Ingame_Setting_RadioGroup_PBR"),
		CUI_InGame_Setting_RadioGroup::Create(m_pDevice, m_pContext, CUI_InGame_Setting_RadioGroup::UI_RADIOGROUP::GRAPHIC_PBR), LAYER_UI)))
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


	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_NameTag"),
		CUI_World_NameTag::Create(m_pDevice, m_pContext, CUI_World_NameTag::UI_NAMETAG::NAMETAG_LOBBY), LAYER_UI)))
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
	for (auto& iter : m_WeaponIcon)
	{
		if (nullptr != iter)
			iter->Set_TextureIndex(m_eCurPlayer);
	}
	for (auto& iter : m_WeaponElemental)
	{
		if (nullptr != iter)
			iter->Set_TextureIndex(m_eCurPlayer);
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
	Safe_Release(m_pTutorial);
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
	Safe_Release(m_pCostumeChange);

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

	Safe_Release(m_pDummy_Swordman);
	Safe_Release(m_pDummy_Destroyer);
	Safe_Release(m_pDummy_Engineer);
	Safe_Release(m_pCustomMap);
	Safe_Release(m_pCostumeMirror);
	Safe_Release(m_pBossNameTag);

	Safe_Release(m_pMapNameText);
	Safe_Release(m_pDialogBattle);

	Safe_Release(m_pIceVignette);
	Safe_Release(m_pFireVignette);

	Safe_Release(m_pInGameSetting);
	Safe_Release(m_pBtnInGameSetting);

	for (auto& pFrame : m_CoolTimeFrame)
		Safe_Release(pFrame);
	m_CoolTimeFrame.clear();

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

	for (auto& pItemSlot : m_CostumeWeapon)
		Safe_Release(pItemSlot);
	m_CostumeWeapon.clear();

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

	for (auto& pMinimap : m_Minimap)
		Safe_Release(pMinimap);
	m_Minimap.clear();

	for (auto& pIcon : m_WeaponIcon)
		Safe_Release(pIcon);
	m_WeaponIcon.clear();

	for (auto& pIcon : m_WeaponElemental)
		Safe_Release(pIcon);
	m_WeaponElemental.clear();

	for (auto& pWindow : m_QuestReward)
		Safe_Release(pWindow);
	m_QuestReward.clear();

	for (auto& pItem : m_QuestItems)
		Safe_Release(pItem);
	m_QuestItems.clear();

	for (auto& pSlot : m_GraphicSlot)
		Safe_Release(pSlot);
	m_GraphicSlot.clear();

	for (auto& pSlot : m_CameraSlot)
		Safe_Release(pSlot);
	m_GraphicSlot.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
