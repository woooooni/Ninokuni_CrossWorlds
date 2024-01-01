#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_BossHP_Background final : public CUI
{
public:
	enum UI_BOSSHPBACK { BOSS_INFO, BOSS_BARFRAME, BOSSHPBACK_END };

protected:
	CUI_BossHP_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_BOSSHPBACK eType);
	CUI_BossHP_Background(const CUI_BossHP_Background& rhs);
	virtual ~CUI_BossHP_Background() = default;

public:
	void Set_Owner(class CBoss* pBoss);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CBoss* m_pOwner = { nullptr };
	UI_BOSSHPBACK m_eUIType = { BOSSHPBACK_END };

	wstring m_strName = L"";
	_uint m_iTextureIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_BossHP_Background* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_BOSSHPBACK eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
