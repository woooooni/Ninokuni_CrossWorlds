#pragma once

#include "MonsterProjectile.h"

BEGIN(Client)

class CGlanix_IceBall final : public CMonsterProjectile
{
private:
	CGlanix_IceBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CGlanix_IceBall(const CGlanix_IceBall& rhs);
	virtual ~CGlanix_IceBall() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
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

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();

private:
	CGlanix* m_pGlanix = nullptr;

	_float m_fTime = 0.f;
	_float m_fDelteTime = 0.f;

	Vec3 m_vInitLook = {};
	Matrix m_RotationMatrix = Matrix::Identity;

	_bool m_bFirst = true;

private:
	_bool   m_bStartBallMove = false;
	_matrix m_fStartBallMatrix = {};
	_float  m_fEffectAcc = 0.f;

public:
	static CGlanix_IceBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

