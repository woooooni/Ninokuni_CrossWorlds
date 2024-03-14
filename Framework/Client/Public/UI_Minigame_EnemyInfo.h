#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_EnemyInfo final : public CUI
{
protected:
	CUI_Minigame_EnemyInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Minigame_EnemyInfo(const CUI_Minigame_EnemyInfo& rhs);
	virtual ~CUI_Minigame_EnemyInfo() = default;

public:
	void Set_TextureIndex(_uint iIndex);

	void Set_Owner(CGameObject* pOwner);
	class CGameObject* Get_Owner() { return m_pOwner; }

	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CGameObject* m_pOwner = { nullptr };
	class CUI_Minigame_EnemyHP* m_pHP = { nullptr };

	_uint m_iTextureIndex = { 0 };
	_uint m_iPass = { 1 };

	_float2 m_vOriginPos = _float2(0.f, 0.f);
	_float2 m_vStartPos = _float2(0.f, 0.f);
	_bool m_bArrived = { false };
	_float m_fSpeed = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Minigame_EnemyInfo* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
