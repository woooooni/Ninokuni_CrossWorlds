#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SubMenu_Imajinn final : public CUI
{
public:
	enum UI_SUBIMAJINN_TYPE
	{ PETBTN_IMAJINN, PETBTN_TRAINING, PETBTN_TOY, PETBTN_CRADLE, PETBTN_RETURN, PETBTN_DUNGEON,
	PETBTN_UNITETREE, PETBTN_ULTIMATE, PETMENU_END };
	// 이마젠 동행, 훈련, 장난감, 부화하기, 놓아주기, 이마젠의 숲
	// 결속의 나무, 궁극 성장

protected:
	CUI_SubMenu_Imajinn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag, UI_SUBIMAJINN_TYPE eUIType);
	CUI_SubMenu_Imajinn(const CUI_SubMenu_Imajinn& rhs);
	virtual ~CUI_SubMenu_Imajinn() = default;

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
	UI_SUBIMAJINN_TYPE m_eUIType = { UI_SUBIMAJINN_TYPE::PETMENU_END };

	_bool m_bActive = { false }; // 활성화되었다
	_bool m_bClicked = { false }; // 선택되었다
	_bool m_bArrived = { false }; // 목표 위치까지 도달했다
	_bool m_bUsable = { false }; // 사용할 수 있는 아이콘인가? -> 기능이 있는가
	_bool m_bUpdateAlpha = { false };

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
	static CUI_SubMenu_Imajinn* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,
		const wstring& strObjectTag, UI_SUBIMAJINN_TYPE eUIType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
