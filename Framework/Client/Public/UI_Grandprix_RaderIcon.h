#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Grandprix_RaderIcon final : public CUI
{
public:
	enum UI_RADERICON { ICON_SWORDMAN, ICON_ENGINEER, ICON_GHOST, ICON_CAMERA, RADERICON_END };

protected:
	CUI_Grandprix_RaderIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Grandprix_RaderIcon(const CUI_Grandprix_RaderIcon& rhs);
	virtual ~CUI_Grandprix_RaderIcon() = default;

public:
	void Set_Owner(class CGameObject* pOwner, _bool bIsCamera = false);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();
	virtual HRESULT Render_Minimap() override;

private:
	UI_RADERICON m_eType = { RADERICON_END };

	class CGameObject* m_pOwner = { nullptr };
	_uint m_iPass = { 20 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Rotation_Icon(_float fTimeDelta);

public:
	static CUI_Grandprix_RaderIcon* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
