#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Grandprix_Target final : public CUI
{
protected:
	CUI_Grandprix_Target(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Grandprix_Target(const CUI_Grandprix_Target& rhs);
	virtual ~CUI_Grandprix_Target() = default;

public:
	void Set_Target(class CVehicle_Flying* pOwner);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CVehicle_Flying* m_pOwner = { nullptr };

	_bool m_bRotate = { false };
	_float m_fTimeAcc = { 0.f };
	_float2 m_vOffset = _float2(0.f, 0.f);
	_float2 m_vOriginOffset = _float2(0.f, 0.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Grandprix_Target* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
