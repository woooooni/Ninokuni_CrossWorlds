#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_MonsterHP_Elemental final : public CUI
{
protected:
	CUI_MonsterHP_Elemental(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MonsterHP_Elemental(const CUI_MonsterHP_Elemental& rhs);
	virtual ~CUI_MonsterHP_Elemental() = default;

public:
	_int Get_ElementalType() { return m_iTextureIndex; }
	void Set_ElementalType(_uint iType) { m_iTextureIndex = iType; }

	void Set_MonsterLevel(_uint iLevel) { m_iLevel = iLevel; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_int m_iTextureIndex = { -1 };
	_uint m_iLevel = { 0 };

	_float2 m_vTextPosition = _float2(g_iWinSizeX * 0.5f - 135.f, 60.f);

	_float4 m_vFontColor = _float4(0.965f, 1.f, 0.878f, 1.f);
	_float4 m_vOutlineColor = _float4(0.075f, 0.2f, 0.314f, 1.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_MonsterHP_Elemental* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
