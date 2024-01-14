#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Quest_Reward final : public CUI
{
public:
	enum UI_QUESTREWARD { REWARD_TOP, REWARD_BOTTOM, REWARD_WINDOW, REWARD_END };

protected:
	CUI_Quest_Reward(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTREWARD eType);
	CUI_Quest_Reward(const CUI_Quest_Reward& rhs);
	virtual ~CUI_Quest_Reward() = default;

public:
	virtual void Set_Active(_bool bActive);
	_float Get_Alpha() { return m_fAlpha; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_QUESTREWARD m_eType = { REWARD_END };

	_float2 m_vOriginPosition = _float2(0.f, 0.f);
	_float2 m_vTargetPosition = _float2(0.f, 0.f);

	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMaxSize = _float2(0.f, 0.f);
	_float2 m_vTextPosition = _float2(0.f, 0.f);

	_float m_fSpeed = { 50.f };

	_bool m_bAlpha = { false };
	_bool m_bFinish = { false };

	_float m_fIncrement = { 0.f };
	_float m_fActiveTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Quest_Reward* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_QUESTREWARD eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
