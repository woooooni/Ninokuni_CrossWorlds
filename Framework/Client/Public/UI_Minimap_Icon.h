#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minimap_Icon final : public CUI
{
public:
	enum UI_MINIMAPICON { ICON_PLAYER, ICON_MONSTER, ICON_PORTAL, ICON_NPC, MINIMAPICON_END };

protected:
	CUI_Minimap_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Minimap_Icon(const CUI_Minimap_Icon& rhs);
	virtual ~CUI_Minimap_Icon() = default;

public:
	void Set_Owner(class CGameObject* pOwner);
//	void Set_Position(_float2 vPos);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();
	virtual HRESULT Render_Minimap() override;

private:
	UI_MINIMAPICON m_eType = { MINIMAPICON_END };

	class CGameObject* m_pOwner = { nullptr };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Rotation_Icon(_float fTimeDelta);

public:
	static CUI_Minimap_Icon* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
