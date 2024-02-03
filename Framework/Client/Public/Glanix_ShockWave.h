#pragma once

#include "MonsterProjectile.h"
#include "Glanix.h"

BEGIN(Client)
class CGlanix_ShockWave final : public CMonsterProjectile
{
protected:
	CGlanix_ShockWave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CGlanix_ShockWave(const CGlanix_ShockWave& rhs);
	virtual ~CGlanix_ShockWave() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();

private:
	CGlanix* m_pGlanix = nullptr;

	_float fSpeed = 0.f;

	_float m_fTime = 0.f;
	_float m_fDelteTime = 0.f;

	Vec3 m_vInitLook = {};
	Matrix m_RotationMatrix = Matrix::Identity;

	_bool m_bFirst = true;

private:
	_bool m_bEffect = false;

public:
	static CGlanix_ShockWave* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

