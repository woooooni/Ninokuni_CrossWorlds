#pragma once

#include "Client_Defines.h"
#include "CurlingGame_Prop.h"

BEGIN(Client)

class CCurlingGame_Arrow final : public CCurlingGame_Prop
{
private:
	CCurlingGame_Arrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCurlingGame_Arrow(const CCurlingGame_Arrow& rhs);
	virtual ~CCurlingGame_Arrow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	void Start_Action();
	void Tick_Action();

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Colliders() override;

private:
	LERP_VEC3_DESC		m_tLerpScale = {};
	const Vec3			m_vStartScale = { 1.5f, 1.5f, 1.5f };
	const Vec3			m_vTargetScale = { 1.5f, 1.5f, 2.f };
	const _float		m_fLerpTime = 0.5f;
	_bool				m_bIncrease = false;

public:
	static CCurlingGame_Arrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;

public:
	virtual void Free() override;
};

END

