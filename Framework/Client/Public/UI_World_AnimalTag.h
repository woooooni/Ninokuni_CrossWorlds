#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_World_AnimalTag final : public CUI
{
protected:
	CUI_World_AnimalTag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_World_AnimalTag(const CUI_World_AnimalTag& rhs);
	virtual ~CUI_World_AnimalTag() = default;

public:
	void Set_Owner(class CGameObject* pOwner, const wstring& strNameTag, _float fOffsetY, _bool bIsUnique = false);
	void Set_TextOnOff(_bool bOnOff) { m_bAddText = bOnOff; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CAnimals* m_pOwner = { nullptr };
	wstring m_strName;

	_float2 m_vOwnerPos = _float2(0.f, 0.f);
	_float2 m_vTextPos = _float2(0.f, 0.f);

	_float m_fTimeAcc = { 0.f };
	_float m_fOffsetY = { 0.f };
	_float m_fOriginY = { 0.f };

	_bool m_bAddText = { false };
	_bool m_bIsUnique = { false };
	_float4 m_vColor = _float4(0.f, 0.f, 0.f, 0.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();

public:
	static CUI_World_AnimalTag* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
