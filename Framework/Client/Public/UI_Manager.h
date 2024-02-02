#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CUI_Manager : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;
	
public: // Get/Set
	class CUI_Fade* Get_Fade();
	_bool			Get_MainMenuActive();
	class CCharacter* Get_Character();

	void			Set_Textable(_bool bTextable) { m_bUpdate = bTextable; }
	void			Set_RandomNick(const wstring& strRandom);
	void			Set_UserName();
	void			Set_MonsterDescForUI(class CMonster* pOwner, _bool bActive = true);
	void			Set_BossActive(_bool bBossActive) { m_bBossActive = bBossActive; }

	_bool			Is_DefaultSettingOn();
	_bool			Is_NicknameSettingComplete();
	_bool			Is_QuestRewardWindowOff();
	_bool			Is_LoadingDone();
	_bool			Is_MinimapOn();

	void			Set_UIClicked(_bool bClicked) { m_bEvent = bClicked; }
	_bool			Is_UIClicked() { return m_bEvent; }

	_float2			Get_ProjectionPosition(class CTransform* pTransform);

	void			Set_MainDialogue(_tchar* pszName, _tchar* pszText);
	void			Set_MiniDialogue(wstring strName, wstring strContents);
	void			Set_BattleDialogue(wstring strContents);

	void			Set_QuestPopup(const wstring& strQuestType, const wstring& strTitle, const wstring& strContents);  // 병합시 오류 방지. 임시로 살려둠.
	void			Set_QuestPopup(void* pArg);
	void			Update_QuestPopup(const wstring& strPreTitle, const wstring& strQuestType, const wstring& strTitle, const wstring& strContents); // 병합시 오류 방지. 임시로 살려둠.
	void			Update_QuestPopup(const wstring& strPreTitle, void* pArg);
	void			Clear_QuestPopup(const wstring& strTitle);
	void			Resize_QuestPopup();
	_int			Get_QuestNum();
	void			Get_QuestInfo();

	void			Set_QuestDestSpot(_int iWindow);
	void			Calculate_QuestDestSpot(const wstring& strContents, _float4 vDestPos);

	_int			Get_SelectedCharacter();
	_float			Get_DistanceofMovement_SkillBG();
	_bool			Get_MovementComplete_SkillBG();

	// For InGame Setting
	void			Set_DefaultGraphicSetting(); // 그래픽 기본 세팅으로 전환해줌
	_bool			Is_SettingFirst(); // 그래픽 세팅을 처음 하는지
	void			Set_GraphicSettingState(_bool bisFirst); // 그래픽 세팅을 한 번이라도 했다면 사용한다.
	_bool			Get_AbleToControlRadio();
	void			Set_AbleToControlRadio(_bool bControl);

	_bool			Is_Dialog_Active();

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);
	void	LateTick(_float fTImeDelta);

public: // For Ready Prototypes
	HRESULT Ready_UIPrototypes(LEVELID eID);

public: // For Clone
	HRESULT Ready_Cursor();
	HRESULT Ready_Veils(LEVELID eID);
	HRESULT Ready_Dummy();
	HRESULT Ready_Loadings();
	HRESULT Ready_LobbyUIs();
	HRESULT Ready_GameObject(LEVELID eID); // + Ready_Cursor, Ready_Veil 정리 필요함.
	HRESULT Ready_GameObjectToLayer(LEVELID eID);
	HRESULT Ready_BossHPBar(class CBoss* pBoss, void* pArg = nullptr);
	HRESULT UI_WndProcHandler(UINT message, WPARAM wParam, LPARAM lParam);
	
public:
	void Ready_CharacterTypeForUI(CHARACTER_TYPE eType) {
		m_eCurPlayer = eType;
		Set_CharacterType();
	}
	void Ready_ElementalTypeForUI(ELEMENTAL_TYPE eType) {
		m_eElemental = eType;
		Set_ElementalType();
	}

public:
	HRESULT Tick_UIs(LEVELID eID, _float fTimeDelta);
	HRESULT Tick_LobbyLevel(_float fTimeDelta);
	HRESULT Tick_GamePlayLevel(_float fTimeDelta);

	HRESULT LateTick_GamePlayLevel(_float fTimeDelta);
	void	LateTick_Dummy(_float fTimeDelta);

	HRESULT Render_GamePlayLevel();
	void	Render_Dummy();

public:
	void	Tick_Fade(_float fTimeDelta);
	void	LateTick_Fade(_float fTimeDelta);
	void	Render_Fade();
	_bool	Is_FadeFinished();

public: // MinimapManager 이사
	_bool		Is_InMinimap(class CTransform* pTransform);
	_float2		Calculate_PositionForMinimap(class CTransform* pTransform);

public:
	void	Update_SetNickname(const wstring& strNickname, _bool bUpdate = true);
	void	Update_LobbyBtnState(_uint iIndex);
	void	Update_CostumeBtnState(_uint iIndex);
	void	Update_InvenBtnState(_uint iIndex);
	void	Update_SkillBtnState(class CTransform* pTransform, _uint iIndex);
	void	Update_SkillSlotState(_uint iSectionType, _uint iSlotIndex);
	void	Update_SkillSection(_uint iSkillType, _uint iSectionType);
	void	Update_ClothSlotState(_uint iSectionType, _uint iSlotIndex);
	void	Update_CostumeBtn();
	void	Update_WeaponSelectionIcon(_uint iSlotNum);
	void	Update_IceVignette();
	void	Update_SettingButton(_uint iGroupType);
	void	Update_SettingGraphicRadio(_uint iGroupType, _uint iBtnType);
	void	Reset_SettingCamera();
	void	Update_PlayerSlot(CHARACTER_TYPE eType);

	void	Update_CostumeModel(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType, const wstring& strPartTag);
	void	Update_CostumeWeaponModel(const CHARACTER_TYPE& eCharacterType, const wstring& strPartTag);
	void	Set_CostumeModel();
	void	TakeOff_CostumeModel();

	void	Set_MouseCursor(_uint iIndex);
	void	Hide_MouseCursor(_bool bHide);

public:
	HRESULT		Using_CloseButton();
	HRESULT		Using_BackButton();

	void		Use_JumpBtn();
	void		Use_RollBtn();
	void		Use_AttackBtn();
	void		Use_ClassSkillSlot(_uint iSlotNum);
	void		Use_ActiveSkillSlot(_uint iSlotNum);
	void		Use_BurstSkillSlot(_uint iSlotNum);
	
	void		Change_ElementalType(ELEMENTAL_TYPE eElementalType);

public:
	HRESULT OnOff_Veil(_bool bOnOff);
	
public: // Lobby
	HRESULT OnOff_SettingWindow(_bool bOnOff);
	HRESULT OnOff_NickNameWindow(_bool bOnOff);
	void	OnOff_LobbyUIs(_bool bOnOff);

	HRESULT OnOff_GamePlaySetting(_bool bOnOff);
	void	OnOff_GamePlaySetting_ExceptInfo(_bool bOnOff);
	void	Hide_GamePlaySetting(_bool bHide);
	HRESULT OnOff_MainMenu(_bool bOnOff);
	HRESULT OnOff_SubMenu(_bool bOnOff, _uint iMagicNum);
	HRESULT Off_OtherSubBtn(_uint iMagicNum);
	void	OnOff_MapName(_bool bOnOff, const wstring& strMapName = TEXT(""));

	HRESULT OnOff_Announce(_int iMagicNum, _bool bOnOff);
	HRESULT OnOff_DialogWindow(_bool bOnOff, _uint iMagicNum);
	HRESULT OnOff_QuestWindow(_bool bOnOff);
	HRESULT OnOff_QuestPopup(_bool bOnOff);
	HRESULT OnOff_WorldMap(_bool bOnOff);
	HRESULT OnOff_MiniMap(_bool bOnOff);

	void	OnOff_IceVignette(_bool bOnOff);
	void	On_FireVignette();

	HRESULT OnOff_CloseButton(_bool bOnOff);
	void	OnOff_LevelUp(_bool bOnOff, _uint iLevel = 0);

	HRESULT OnOff_MonsterHP(_bool bOnOff, ELEMENTAL_TYPE eType = ELEMENTAL_TYPE::ELEMENTAL_END);
	HRESULT OnOff_BossHP(_bool bOnOff);
	void	OnOff_BossNameTag(_bool bOnOff);

	HRESULT OnOff_CostumeWindow(_bool bOnOff);
	HRESULT OnOff_CostumeSlot(_uint iSection, _bool bOnOff);
	
	void	OnOff_InGameSetWindow(_bool bOnOff);
	void	OnOff_SettingSlot(_uint iSection, _bool bOnOff);

	HRESULT OnOff_SkillWindow(_bool bOnOff);
	HRESULT OnOff_SkillWindowSlot(_uint iMenuType, _bool bOnOff);
	HRESULT OnOff_Inventory(_bool bOnOff);

	void	Set_EmoticonType(_uint iIndex);
	HRESULT OnOff_EmoticonWindow(_bool bOnOff);
	HRESULT OnOff_EmoticonBalloon(_bool bOnOff);

	void OnOff_TextUI(_bool bOnOff);

	void OnOff_QuestRewards(_bool bOnOff, const wstring& strTitle = TEXT(""));
	void Set_AlphaToItems();
	void Show_RewardItems();

	void Show_AddItem(ITEM_TYPE eItemType, ITEM_CODE eItemCode, _uint iCount);

private:
	CHARACTER_TYPE m_eCurPlayer = { CHARACTER_TYPE::SWORD_MAN };
	ELEMENTAL_TYPE m_eElemental = { ELEMENTAL_TYPE::FIRE };

private:
	_bool m_bEvent = { false };
	_bool m_bUpdate = { false };
	_bool m_bBossActive = { false };
	_bool m_bAddText = { false };

	wstring m_strNickname;
	wstring m_strResult;

	class CUI_CharacterDummy*	m_pDummy_Swordman = { nullptr };
	class CUI_CharacterDummy*	m_pDummy_Destroyer = { nullptr };
	class CUI_CharacterDummy*	m_pDummy_Engineer = { nullptr };
	class CUI_CostumeTab_Map*	m_pCustomMap = { nullptr };
	class CMirror*				m_pCostumeMirror = { nullptr };
	class CMonster*				m_pHPBarOwner = { nullptr };

private:
	class CUI_Default_Background*	m_pDefaultBG = { nullptr };
	class CUI_Cursor*				m_pUICursor = { nullptr };
	class CUI_Veil*					m_pUIVeil = { nullptr };
	class CUI_Fade*					m_pUIFade = { nullptr };
	class CUI_MapName*				m_pUIMapName = { nullptr };
	class CUI_Tutorial_Window*		m_pTutorial = { nullptr };
	class CUI_Basic*				m_pMapText = { nullptr };
	class CUI_MapName_Text*			m_pMapNameText = { nullptr };

	// For Setting Window
	class CUI_Setting_Window*		m_pSettingBG = { nullptr };
	// For Set Nickname
	class CUI_SetNickname_Window*	m_pSetNickBG = { nullptr };
	class CUI_SetNickname_Textbox*	m_pNicknamebox = { nullptr };

	class CUI_PlayerInfo*			m_pPlayerStatus = { nullptr };

	class CUI_BtnShowMenu*				m_pBtnShowMenu = { nullptr };
	class CUI_Btn_Minimap*				m_pBtnShowMinimap = { nullptr };
	class CUI_BtnChangeCamera*			m_pBtnCamera = { nullptr };
	class CUI_BtnInventory*				m_pBtnInven = { nullptr };
	class CUI_BtnQuickQuest*			m_pBtnQuest = { nullptr };
	class CUI_BtnClose*					m_pBtnClose = { nullptr };
	class CUI_BtnShowSetting*			m_pBtnShowSetting = { nullptr };
	class CUI_InGame_Setting_OpenBtn*	m_pBtnInGameSetting = { nullptr };

	class CUI_MainMenu*				m_pMainBG = { nullptr }; // MainMenu Background (버튼과 따로 관리된다)
	class CUI_WindowQuest*			m_pWindowQuest = { nullptr };

	vector<class CUI_PopupQuest*>			m_QuestPopUp;
	vector<class CUI_Quest_Reward*>			m_QuestReward;
	vector<class CUI_Quest_Reward_Item*>	m_QuestItems;

	class CUI_Text_TabMenu*			m_pTabMenuTitle = { nullptr };

	// PlayerStatus
	vector<class CUI_PlayerEXPBar*>					m_PlayerEXP;
	vector<class CUI_SkillSection_ClassicSkill*>	m_ClassicSkill;
	vector<class CUI_SkillSection_Frame*>			m_ClassicFrame;
	vector<class CUI_SkillSection_SpecialSkill*>	m_SpecialSkill;
	vector<class CUI_SkillSection_Frame*>			m_SpecialFrame;
	vector<class CUI_SkillSection_CoolTimeFrame*>	m_CoolTimeFrame;
	vector<class CUI_QuickSlot_Item*>				m_ItemQuickslot;

	// MonsterHP (단일 HPBar -> UIManager에서 관리함)
	class CUI_MonsterHP_Background*			m_pMonsterHPBack = { nullptr };
	class CUI_MonsterHP_Bar*				m_pMonsterHPBar = { nullptr };
	class CUI_MonsterHP_ElementalFrame*		m_pMonsterFrame = { nullptr };
	class CUI_MonsterHP_Elemental*			m_pMonsterElemental = { nullptr };

	// BossHP (단일 HPBar -> UIManager에서 관리함)
	class CUI_BossHP_Background*			m_pBossInfo = { nullptr };
	class CUI_BossHP_Background*			m_pBossHPBack = { nullptr };
	class CUI_BossHP_Bar*					m_pBossHPBar = { nullptr };

	class CUI_Announced*					m_pCameraAnnounce = { nullptr };

	class CUI_Emoticon_Window*				m_pEmoticonWindow = { nullptr };
	class CUI_Emoticon_SpeechBalloon*		m_pBalloon = { nullptr };

	// MilePost
	vector<class CUI_Milepost*>				m_Milepost;

private:
	vector<class CUI_Basic*>				m_Basic;
	vector<class CUI_BasicButton*>			m_Buttons;

	vector<class CUI_LevelUp*>				m_LevelUp;

	vector<class CUI_BtnCharacterSelect*>	m_UnclickedPlayer;
	vector<class CUI_BtnCharacterSelect*>	m_ClickedPlayer;

	// Main + Sub Menu Btns
	vector<class CUI_MainMenu*>				m_MainMenuBtn;
	vector<class CUI_MenuSeparator*>		m_MenuSeparator;
	vector<class CUI_SubMenu_Character*>	m_SubMenuChar;
	vector<class CUI_SubMenu_Equipment*>	m_SubMenuEquip;
	vector<class CUI_SubMenu_Imajinn*>		m_SubMenuPet;
	vector<class CUI_SubMenu_Shop*>			m_SubMenuShop;

	// For World Map
	class CUI_WindowWorldMap*				m_pWorldMapBG = { nullptr };
	// For Skill Section
	class CUI_SkillSection_Background*		m_pSkillBG = { nullptr };
	// For Imajinn Section
	class CUI_ImajinnSection_Background*				m_pImajinnBG = { nullptr };
	vector<class CUI_ImajinnSection_PlayerSwitching*>	m_PlayerSlot;
	class CUI_ImajinnSection_Selected*					m_pPlayerSelected = { nullptr };
	// For Dialog
	class CUI_Dialog_Window*				m_pDialogWindow = { nullptr };
	class CUI_Dialog_MiniWindow*			m_pDialogMini = { nullptr };
	vector<class CUI_Dialog_Portrait*>		m_Portrait;
	class CUI_Dialog_BattleWindow*			m_pDialogBattle = { nullptr };
	// For Costume
	vector<class CUI_Costume_Btn*>			m_CostumeBtn;
	vector<class CUI_Costume_Btn*>			m_CostumeClickedBtn;
	class CUI_Costume_LineBox*				m_pCostumeBox = { nullptr };
	vector<class CUI_Costume_ItemSlot*>		m_CostumeCloth;
	vector<class CUI_Costume_ItemSlot*>		m_CostumeHairAcc;
	vector<class CUI_Costume_ItemSlot*>		m_CostumeWeapon;
	class CUI_Basic*						m_pCostumeAnnounce = { nullptr };
	class CUI_Costume_ChangeBtn*			m_pCostumeChange = { nullptr };
	// For Inven
	class CUI_Inventory_LineBox*			m_pInvenBox = { nullptr };
	vector<class CUI_Inventory_TabBtn*>		m_InvenBtn;
	vector<class CUI_Inventory_TabBtn*>		m_InvenClickedBtn;
	vector<class CUI_Inventory_Slot*>		m_InvenSlots;
	// For Skill
	vector<class CUI_SkillWindow_LineBox*>		m_SkillWindow;
	vector<class CUI_SkillWindow_Btn*>			m_SkillBtn;
	vector<class CUI_SkillWindow_Btn*>			m_SkillClickedBtn;
	vector<class CUI_SkillWindow_SkillSlot*>	m_ClassSkillSlot;
	vector<class CUI_SkillWindow_SkillSlot*>	m_SpecialSkillSlot;
	class CUI_SkillWindow_LineBox*				m_pSkillDesc = { nullptr };

	vector<class CUI_Minimap_Frame*>			m_MinimapFrame;

	class CUI_Minimap*							m_pMinimap = { nullptr };


	vector <class CUI_WeaponSection_Weapon*>	m_WeaponIcon;
	vector <class CUI_WeaponSection_Weapon*>	m_WeaponElemental;
	class CUI_WeaponSection_Recommend*			m_pRecommend = { nullptr };

	class CUI_Boss_NameTag*				m_pBossNameTag = { nullptr };

	// Vignette
	class CUI_Vignette_Ice*				m_pIceVignette = { nullptr };
	class CUI_Vignette_Fire*			m_pFireVignette = { nullptr };

	class CUI_InGame_Setting_Window*		m_pInGameSetting = { nullptr };
	vector <class CUI_InGame_Setting_Slot*> m_GraphicSlot;
	vector <class CUI_InGame_Setting_Slot*> m_CameraSlot;

	list<class CUI_AddItem*>		m_ItemPopup;

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	// Minimap Manager 이사
private:
	CRenderer* m_pRendererCom = { nullptr };

private:
	HRESULT		Ready_UIStaticPrototypes();
	HRESULT		Ready_UILobbyPrototypes();
	void		Set_CharacterType();
	void		Set_ElementalType();

public:
	virtual void Free() override;
};

END
