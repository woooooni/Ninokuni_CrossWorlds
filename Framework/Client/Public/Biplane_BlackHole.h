#pragma once

#include "Client_Defines.h"
#include "VehicleFlying_Projectile.h"

BEGIN(Client)

class CBiplane_BlackHole final : public CVehicleFlying_Projectile
{
private:
	CBiplane_BlackHole(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBiplane_BlackHole(const CBiplane_BlackHole& rhs);
	virtual ~CBiplane_BlackHole() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;

public:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;


private:
	_bool m_bArrive = false;

private:
	_float m_fMoveSpeed = 2.f;
	_float m_fAccArrive = 0.f;
	_float m_fArriveTime = 5.f;

	_float m_fColliderOnOffDamageTime = 1.f;
	_float m_fAccColliderOnOffDamage = 0.f;

	_float m_fGravityForce = 10.f;


public:
	static CBiplane_BlackHole* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

