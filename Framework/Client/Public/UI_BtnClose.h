#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_BtnClose final : public CUI
{
protected:
	CUI_BtnClose(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_BtnClose(const CUI_BtnClose& rhs);
	virtual ~CUI_BtnClose() = default;

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
	_bool m_bHide = { false };

	_float m_fTimeAcc = { 0.f };
	_bool m_bUsable = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Key_Input(_float fTimeDelta);

public:
	static CUI_BtnClose* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
