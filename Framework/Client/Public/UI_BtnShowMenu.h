#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_BtnShowMenu final : public CUI
{
protected:
	CUI_BtnShowMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_BtnShowMenu(const CUI_BtnShowMenu& rhs);
	virtual ~CUI_BtnShowMenu() = default;

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
	_bool m_bHide = { false }; // 숨겨졌는가? -> 다른 창이 활성화 되었을때 사용할 예정

	// 움직일 포지션 필요함.

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_BtnShowMenu* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
