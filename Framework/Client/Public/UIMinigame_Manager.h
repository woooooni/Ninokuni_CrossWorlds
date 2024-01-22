#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CUIMinigame_Manager : public CBase
{
	DECLARE_SINGLETON(CUIMinigame_Manager)

private:
	CUIMinigame_Manager();
	virtual ~CUIMinigame_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);


public: // Prototypes
	HRESULT Ready_MinigameUI_Prototypes(LEVELID eID);

public: // Clone
	HRESULT Ready_MinigameUI_GameObject(LEVELID eID);
	HRESULT Ready_MinigameUIToLayer(LEVELID eID);

public: // OnOff
	void OnOff_TowerDefence_Select(_bool bOnOff);


private: // Prototypes
	HRESULT Ready_MinigameUI_Evermore();

private: // clone
	HRESULT Ready_TowerDence();


private: // Evermore Tower Defence
	class CUI_Minigame_Basic* m_pMenu = { nullptr };
	vector<class CUI_Minigame_TowerSelect*> m_TowerSelect;
	class CUI_Minigame_Timer* m_pTimer = { nullptr };
	class CUI_Minigame_Basic* m_pStartBtn = { nullptr };
	class CUI_Minigame_Basic* m_pGold = { nullptr };

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

public:
	virtual void Free() override;
};

END