#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_MainMenu final : public CUI
{
public:
	enum UI_MAINMENU_TYPE
	{ MAINBTN_CHARACTER, MAINBTN_EQUIPMENT, MAINBTN_IMAJINN, MAINBTN_RECORD, MAINBTN_GROWTH,
		MAINBTN_COMMUNITY, MAINBTN_DUNGEON, MAINBTN_CHALLENGE, MAINBTN_BATTLE, MAINBTN_SHOP,
		MAINMENU_BG, UIMAINMENU_END };

protected:
	CUI_MainMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag, UI_MAINMENU_TYPE eUIType);
	CUI_MainMenu(const CUI_MainMenu& rhs);
	virtual ~CUI_MainMenu() = default;

public: // Get/Set 함수
	virtual void Set_Active(_bool bActive) override;
	_bool Get_Active() { return m_bActive; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual void On_MouseEnter(_float fTimeDelta) override;
	virtual void On_Mouse(_float fTimeDelta) override;
	virtual void On_MouseExit(_float fTimeDelta) override;

public:
	void Reset_InitializeInfo();

private: 
	UI_MAINMENU_TYPE m_eUIType = { UI_MAINMENU_TYPE::UIMAINMENU_END };

	_bool m_bActive = { false }; // 활성화되었다
	_bool m_bClicked = { false }; // 선택되었다
	_bool m_bArrived = { false }; // 목표 위치까지 도달했다

	_float2 m_vOnPosition = _float2(0.f, 0.f); // Active된 순간 UI의 시작위치
	_float2 m_vGoalPosition = _float2(0.f, 0.f);
	_float m_fSpeed = { 0.f };

	_int m_iTextureIndex = { -1 };
	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT Ready_Buttons();

	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_MainMenu* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,
		const wstring& strObjectTag, UI_MAINMENU_TYPE eUIType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
