#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Costume_ItemSlot final : public CUI
{
public:
	enum UI_COSTUME_SLOT { COSTUMESLOT_FIRST, COSTUMESLOT_SECOND, COSTUMESLOT_THIRD, COSTUMESLOT_END };

protected:
	CUI_Costume_ItemSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_COSTUME_SLOT eType);
	CUI_Costume_ItemSlot(const CUI_Costume_ItemSlot& rhs);
	virtual ~CUI_Costume_ItemSlot() = default;

public:
	virtual void Set_Active(_bool bActive) override;

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

private:
	UI_COSTUME_SLOT m_eType = { COSTUMESLOT_END };

	_bool m_bArrived = { false };
	_float2 m_vArrivedPosition = _float2(0.f, 0.f);
	_float2 m_vStartPosition = _float2(0.f, 0.f);

	_float m_fSpeed = { 0.f };
	_float m_fLimit = { 0.f };

	_bool m_bClicked = { false };
	_float m_fTimeAcc = { 0.f };
	_int m_iPass = { 9 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_Costume_ItemSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_COSTUME_SLOT eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
