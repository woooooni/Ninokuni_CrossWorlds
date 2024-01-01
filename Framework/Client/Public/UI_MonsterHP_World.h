#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_MonsterHP_World final : public CUI
{
protected:
	CUI_MonsterHP_World(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MonsterHP_World(const CUI_MonsterHP_World& rhs);
	virtual ~CUI_MonsterHP_World() = default;

public:
	class CMonster* Get_Owner() { return m_pOwner; }
	void Set_Owner(CMonster* pOwner, _int iElementalType, _float fOffsetY = 0.f);

	_bool Is_Target() { return m_bIsTarget; }
	void Set_Target(_bool bIsTarget) { m_bIsTarget = bIsTarget; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CMonster* m_pOwner = { nullptr };
	CTexture* m_pBackTextureCom = { nullptr };

	_bool m_bIsTarget = { false }; // 현재 플레이어의 Target인가?

	vector<class CUI_Basic*> m_Arrow;
	_float m_fMaxHP = { 0.f };
	_float m_fCurHP = { 0.f };

	wstring m_strName = L"";
	wstring m_strSubName = L"";

	_uint m_iTextureIndex = { 0 };
	_float m_fOffsetY = { 0.f };

	_float4 m_vSubNameColor = _float4(0.62f, 0.631f, 0.561f, 1.f);
	_float4 m_vNameColor = _float4(0.98f, 0.953f, 0.588f, 1.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Set_Text(_float2 ScreenPos);

public:
	static CUI_MonsterHP_World* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
