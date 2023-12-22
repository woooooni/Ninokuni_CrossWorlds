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
	void Set_Owner(CMonster* pOwner) { m_pOwner = pOwner; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CMonster* m_pOwner = { nullptr };
	_int m_iMaxHP = { 0 };
	_int m_iCurHP = { 0 };

	wstring m_strName = L"";
	wstring m_strSubName = L"";

	wstring m_strMaxHP = L"";
	wstring m_strCurHP = L"";

	_uint m_iTextureIndex = { 0 };
	_float m_fWorldCX = { 0.f };
	_float m_fWorldCY = { 0.f };

	_matrix m_UIWorldMatrix;

	// For Text
	_float2 m_vDefaultPosition = _float2(250.f, 39.f); // "/"
	_float2 m_vCurHPPosition = _float2(200.f, 39.f); // 최대 체력
	_float2 m_vMaxHPPosition = _float2(265.f, 39.f); // 현재 체력

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Set_Text();

public:
	static CUI_MonsterHP_World* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
