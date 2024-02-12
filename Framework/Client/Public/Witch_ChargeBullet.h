#pragma once

#include "MonsterProjectile.h"

BEGIN(Client)

class CWitch_ChargeBullet final : public CMonsterProjectile
{
private:
	CWitch_ChargeBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CWitch_ChargeBullet(const CWitch_ChargeBullet& rhs);
	virtual ~CWitch_ChargeBullet() = default;

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

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();

private:
	CGameObject* m_pWitch = nullptr;

	_float m_fSpeed = 0.f;

	_float m_fAccDelteTime = 0.f;
	_float m_fDelteTime = 0.f;

	Matrix m_RotationMatrix = Matrix::Identity;

public:
	static CWitch_ChargeBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

