#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_BtnCharacterSelect final : public CUI
{
public:
	enum UI_SELECTBTN_CHARACTER
	{ BTN_ROGUE, BTN_DESTROYER, BTN_WITCH, BTN_ENGINEER, BTN_SWORDMAN, UICHARACTERBTN_END };
	enum UI_SELECTBTN_TYPE
	{ BTN_UNCLIKED, BTN_CLICKED, BTNTYPE_END };

protected:
	CUI_BtnCharacterSelect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag, UI_SELECTBTN_TYPE eBtnType, UI_SELECTBTN_CHARACTER ePlayerType);
	CUI_BtnCharacterSelect(const CUI_BtnCharacterSelect& rhs);
	virtual ~CUI_BtnCharacterSelect() = default;

public: // Get/Set 함수
	virtual void Set_Active(_bool bActive) override;

	void Set_Clicked(_bool bClick) { m_bClicked = bClick; }
	_bool Is_Clicked() { return m_bClicked; }

	void Set_Arrive(_bool bArrive) { m_bArrived = bArrive; }
	_bool Is_Arrived() { return m_bArrived; }

	void Set_Move(_bool bMove);
	_bool Get_IsMoved() { return m_bMoveStart; }

	_bool Get_MoveEnd() { return m_bMoveEnd; }

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
	UI_SELECTBTN_CHARACTER m_ePlayerType = { UI_SELECTBTN_CHARACTER::UICHARACTERBTN_END };
	UI_SELECTBTN_TYPE m_eTextureType = { UI_SELECTBTN_TYPE::BTNTYPE_END };

	_bool m_bClicked = { false }; // 선택되었다
	_bool m_bArrived = { false }; // 목표 위치까지 도달했다
	_bool m_bMoveStart = { false }; // 원위치로 돌아가게 제어하는 불변수
	_bool m_bMoveEnd = { false };
	_bool m_bUpdateDummy = { false };

	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMaxSize = _float2(0.f, 0.f);

	_float2 m_vOriginPosition = _float2(0.f, 0.f);
	_float2 m_vGoalPosition = _float2(0.f, 0.f);

	_int m_iTextureIndex = { -1 };
	_float m_fTimeAcc = { 0.f };

private:
	class CUI_Dummy_Swordsman* m_pSwordsman = { nullptr };
	class CUI_Dummy_Engineer* m_pEngineer = { nullptr };
	class CUI_Dummy_Destroyer* m_pDestroyer = { nullptr };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT Ready_UnclickedTexture();
	HRESULT Ready_ClickedTexture();
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_BtnCharacterSelect* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,
		const wstring& strObjectTag, UI_SELECTBTN_TYPE eBtnType, UI_SELECTBTN_CHARACTER ePlayerType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
