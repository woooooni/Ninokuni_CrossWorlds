#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Follow final : public CCamera
{
private:
	CCamera_Follow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Follow(const CCamera_Follow& rhs);
	virtual ~CCamera_Follow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Tick_Basic(_float fTimeDelta);

protected:
	virtual HRESULT Ready_Components() override;

public:
	void Set_OffsetDistance(_float fOffsetDitance) { m_fOffsetDistance = fOffsetDitance; }

private:
	void Follow(_float fTimeDelta);

private:
	_float m_fOffsetDistance = 5.f;
	_float3 m_vOffsetPosition = { 0.f, 2.f, -5.f };
	_float3 m_vAngle = {};
	_float4 m_vDestPos = {};
	_float m_fCamSpeed = 7.f;

public:
	static CCamera_Follow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END