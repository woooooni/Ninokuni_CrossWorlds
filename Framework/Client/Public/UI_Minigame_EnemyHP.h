#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_EnemyHP final : public CUI
{
protected:
	CUI_Minigame_EnemyHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Minigame_EnemyHP(const CUI_Minigame_EnemyHP& rhs);
	virtual ~CUI_Minigame_EnemyHP() = default;

public:
	void Set_Owner(CGameObject* pOwner);
//	class CGameObject* Get_Owner() { return m_pOwner; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	CTexture* m_pFXTextureCom = { nullptr };
	class CVehicle_Flying* m_pOwner = { nullptr };
	_uint m_iPass = { 14 };

	_bool m_bLerp = { false };
	_bool m_bAddText = { true };
	_float m_fMaxHP = { 0 };
	_float m_fPreHP = { 0 };
	_float m_fCurHP = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Minigame_EnemyHP* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
