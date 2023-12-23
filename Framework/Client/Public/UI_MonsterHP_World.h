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

	_uint m_iTextureIndex = { 0 };
	_matrix m_UIWorldMatrix;

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
