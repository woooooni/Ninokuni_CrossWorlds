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

public:
	enum GRANDPRIX_ENEMY { ENGINEER, ENEMY1, ENEMY2, ENEMY3, ENEMY4, ENEMY5, ENEMY6, ENEMY7, ENEMY8, ENEMY_END };

private:
	CUIMinigame_Manager();
	virtual ~CUIMinigame_Manager() = default;

public: // Getter & Setter
	void Set_HPOwner(CGameObject* pOwner, GRANDPRIX_ENEMY eEnemyID);

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);

public: // Prototypes
	HRESULT Ready_MinigameUI_Prototypes(LEVELID eID);

public: // Clone
	HRESULT Ready_MinigameUI_GameObject(LEVELID eID);
	HRESULT Ready_MinigameUI_ToLayer(LEVELID eID);

public:
	void Tick_Minigame(LEVELID eID, _float fTimeDelta);
	void LateTick_Minigame(LEVELID eID, _float fTimeDelta);

public: // Level Evermore
	void OnOff_TowerDefence_Select(_bool bOnOff);
	void OnOff_Grandprix(_bool bOnOff);

private: // Prototypes
	HRESULT Ready_MinigameUI_Evermore();

private: // clone
	HRESULT Ready_TowerDence();
	HRESULT Ready_Granprix();

private:
	void Tick_Grandprix(_float fTimeDelta);
	void LateTick_Grandprix(_float fTimeDelta);

private: // Evermore Tower Defence
	class CUI_Minigame_Basic* m_pMenu = { nullptr };
	vector<class CUI_Minigame_TowerSelect*> m_TowerSelect;
	class CUI_Minigame_Timer* m_pTimer = { nullptr };
	class CUI_Minigame_Basic* m_pStartBtn = { nullptr };
	class CUI_Minigame_Basic* m_pGold = { nullptr };

private: // Evermore Granprix
	class CUI_Minigame_Basic* m_pCloud = { nullptr };
	vector <class CUI_Minigame_EnemyInfo*> m_EnemyHP;
	vector <class CUI_Minigame_ClassSkill*> m_Skill;
	_bool m_bCountStart = { false };
	_uint m_iCountIndex = { 0 };
	vector <class CUI_Minigame_Basic*> m_Counts;

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

public:
	virtual void Free() override;
};

END