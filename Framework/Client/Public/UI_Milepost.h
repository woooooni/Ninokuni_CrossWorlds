#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Milepost final : public CUI
{
public:
	enum UI_MILEPOST { MILEPOST_FLAG, MILEPOST_ARROW, MILEPOST_END };

protected:
	CUI_Milepost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MILEPOST eType);
	CUI_Milepost(const CUI_Milepost& rhs);
	virtual ~CUI_Milepost() = default;

public:
	void Set_TargetPosition(_float4 vPosition) {
		m_vTargetPos = vPosition;
		m_bGoal = true;
		m_bActive = true;
	}
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_MILEPOST m_eType = { MILEPOST_END };

	_float4 m_vTargetPos = _float4(0.f, 0.f, 0.f, 0.f);
	_float2 m_vCurrentPos = _float2(0.f, 0.f);
	_float m_fDistance = { 0.f };
	_bool m_bGoal = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Rotation_Arrow();

public:
	static CUI_Milepost* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_MILEPOST eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
