#pragma once

#include "MonsterProjectile.h"

BEGIN(Client)

class CClown_Wizard_DarkBall final : public CMonsterProjectile
{
private:
	CClown_Wizard_DarkBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CClown_Wizard_DarkBall(const CClown_Wizard_DarkBall& rhs);
	virtual ~CClown_Wizard_DarkBall() = default;

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
	class CClown_Wizard* m_pClown_Wizard = nullptr;

	_float m_fSpeed = 0.f;

	_float m_fTime = 0.f;
	_float m_fDelteTime = 0.f;

	Vec3 m_vInitLook = {};
	Matrix m_RotationMatrix = Matrix::Identity;

	_bool m_bCreate = false;

public:
	static CClown_Wizard_DarkBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

