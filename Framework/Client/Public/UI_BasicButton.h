#pragma once

#include "UI.h"

// 툴에서도 사용할 Class

BEGIN(Client)
class CUI_BasicButton final : public CUI
{
public:
	enum UIBUTTON_TYPE // 버튼의 기능
	{ BUTTON_CHANGESCENE, UIBUTTONTYPE_END };

protected:
	CUI_BasicButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UIBUTTON_TYPE eType);
	CUI_BasicButton(const CUI_BasicButton& rhs);
	virtual ~CUI_BasicButton() = default;

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
	UIBUTTON_TYPE m_eType = { UIBUTTON_TYPE::UIBUTTONTYPE_END };

	_bool m_bFinish = { false };
	_bool m_bResizeStart = { false };

	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMinSize = _float2(0.f, 0.f);
	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_BasicButton* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strObjectTag, UIBUTTON_TYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
