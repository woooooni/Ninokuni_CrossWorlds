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
	void Set_TextureIndex(_uint iIndex) { m_iTextureIndex = iIndex; }

	void Set_Owner(CGameObject* pOwner);
	class CGameObject* Get_Owner() { return m_pOwner; }

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
