#pragma once

#include "Client_Defines.h"
#include "VehicleFlying_Projectile.h"

BEGIN(Client)

class CEnemy_Biplane_BulletBall final : public CVehicleFlying_Projectile
{
private:
	explicit CEnemy_Biplane_BulletBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEnemy_Biplane_BulletBall(const CEnemy_Biplane_BulletBall& rhs);
	virtual ~CEnemy_Biplane_BulletBall() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;

public:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;

public:
	void Set_OffsetPosition(Vec4 vOffset) { m_vOffsetPos = vOffset; }

private:
	void Update_Rotaion(_float fTimeDelta);
	void Fire_Bullet();

private:
	_float m_fAccEffect = 0.f;
	_float m_fGenEffectTime = 0.5f;

private:

	_float m_fAccFireBullet = 0.f;
	_float m_fFireBulletTime = 0.05f;

private:
	Vec4 m_vOffsetPos = {};

private:
	Vec3 m_vRotationAngle = {};


public:
	static CEnemy_Biplane_BulletBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

