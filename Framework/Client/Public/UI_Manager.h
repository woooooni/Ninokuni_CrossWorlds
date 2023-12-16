#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CUI_Manager : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;
	
public: // Get/Set
	class CUI_Fade* Get_Fade();
	_bool Get_MainMenuActive();

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);
	void	LateTick(_float fTImeDelta);

public: // For Prototypes
	HRESULT Ready_UIPrototypes(LEVELID eID);

public: // For Clone Objects
	HRESULT Ready_Cursor();
	HRESULT Ready_Veils();
	HRESULT Ready_LobbyUIs();
	HRESULT Ready_CommonUIs(LEVELID eID); // 항상 쓰이는 UI들 (Cursor랑 Veil 옮길것)

public:
	HRESULT Tick_UIs(LEVELID eID, _float fTimeDelta);

	HRESULT Tick_LobbyLevel(_float fTimeDelta);
	HRESULT Tick_EvermoreLevel(_float fTimeDelta);

public:
	HRESULT OnOff_GamePlaySetting(_bool bOnOff); // 기본적으로 세팅되어있는 게임화면 UI들
	HRESULT OnOff_MainMenu(_bool bOnOff);
	HRESULT OnOff_QuestWindow(_bool bOnOff);

	HRESULT OnOff_CloseButton(_bool bOnOff);

	HRESULT OnOff_SubMenu(_bool bOnOff, _uint iMagicNum);

public: // For UI Tool
	HRESULT Save_UIData();
	void Load_UIData();

private:
	class CUI_Cursor* m_pUICursor = { nullptr };
	class CUI_Fade* m_pUIFade = { nullptr };
	class CUI_MapName* m_pUIMapName = { nullptr };
	class CUI_Basic* m_pMapText = { nullptr };

	class CUI_PlayerInfo* m_pPlayerStatus = { nullptr };

	class CUI_BtnShowMenu* m_pBtnShowMenu = { nullptr };
	class CUI_BtnChangeCamera* m_pBtnCamera = { nullptr };
	class CUI_BtnInventory* m_pBtnInven = { nullptr };
	class CUI_BtnQuickQuest* m_pBtnQuest = { nullptr };
	class CUI_BtnClose* m_pBtnClose = { nullptr };

	class CUI_MainMenu* m_pMainBG = { nullptr }; // MainMenu Background (버튼과 따로 관리된다)
	class CUI_WindowQuest* m_pWindowQuest = { nullptr };

private:
	vector<class CUI_Basic*> m_Basic;
	vector<class CUI_BasicButton*> m_Buttons;

	vector<class CUI_LevelUp*> m_LevelUp;

	vector<class CUI_BtnCharacterSelect*> m_UnclickedPlayer;
	vector<class CUI_BtnCharacterSelect*> m_ClickedPlayer;

	// Main + Sub Menu Btns
	vector<class CUI_MainMenu*> m_MainMenuBtn;
	vector<class CUI_SubMenu_Character*> m_SubMenuChar;
//	vector<class CUI_SubMenu_Equipment*> m_SubMenuEquip;
//	vector<class CUI_SubMenu_Imajinn*> m_SubMenuPet;
//	vector<class CUI_SubMenu_Shop*> m_SubMenuShop;

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	HRESULT Ready_UIStaticPrototypes();
	HRESULT Ready_UILobbyPrototypes();

public:
	virtual void Free() override;
};

END
