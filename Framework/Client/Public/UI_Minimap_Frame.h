#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minimap_Frame final : public CUI
{
public:
	enum UI_MINIMAP { MINIMAP_FRAME, MINIMAP_ARROW, MINIMAP_BACKGROUND, MINIMAP_END };

protected:
	CUI_Minimap_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MINIMAP eType);
	CUI_Minimap_Frame(const CUI_Minimap_Frame& rhs);
	virtual ~CUI_Minimap_Frame() = default;

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
	UI_MINIMAP m_eType = { MINIMAP_END };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_Minimap_Frame* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_MINIMAP eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
