#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SubMenu_Character final : public CUI
{
public:
	enum UI_SUBCHARACTER_TYPE
	{ CHARBTN_RANK, CHARBTN_SKILL, CHARBTN_VEHICLE, CHARBTN_COSTUME, CHARBTN_DECK, CHARBTN_TITLE,
		CHARBTN_STYLE, CHARBTN_BATTLE, CHARACTERMENU_END };
	// 클래스 랭크, 스킬, 탈 것, 코스튬, 덱, 칭호
	// 배틀 스타일, 배틀 랭크

protected:
	CUI_SubMenu_Character(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag, UI_SUBCHARACTER_TYPE eUIType);
	CUI_SubMenu_Character(const CUI_SubMenu_Character& rhs);
	virtual ~CUI_SubMenu_Character() = default;

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
	UI_SUBCHARACTER_TYPE m_eUIType = { UI_SUBCHARACTER_TYPE::CHARACTERMENU_END };

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
	static CUI_SubMenu_Character* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,
		const wstring& strObjectTag, UI_SUBCHARACTER_TYPE eUIType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
