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

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_uint m_iTextureIndex = { 0 };
	_uint m_iPass = { 1 };

	wstring m_strName = L"";
	_float4 m_vFontColor = _float4(0.965f, 1.f, 0.878f, 1.f);
	_float4 m_vOutlineColor = _float4(0.075f, 0.2f, 0.314f, 1.f);
	_float2 m_vNamePosition = _float2(g_iWinSizeX * 0.5f - 24.f, 58.f);

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
