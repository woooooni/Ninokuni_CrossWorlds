#pragma once

#include "UI.h"

// Render 기능만 필요한 UI class

BEGIN(Client)
class CUI_Basic final : public CUI
{
public:
	enum UI_BASIC { UILOBBY_NICKFRAME, UIBASIC_END };

protected:
	CUI_Basic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_BASIC eType);
	CUI_Basic(const CUI_Basic& rhs);
	virtual ~CUI_Basic() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_BASIC m_eType = { UI_BASIC::UIBASIC_END };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Basic* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strObjectTag, UI_BASIC eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
