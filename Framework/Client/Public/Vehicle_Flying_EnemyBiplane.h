#pragma once

#include "Vehicle_Flying.h"

BEGIN(Client)
class CVehicle_Flying_EnemyBiplane final : public CVehicle_Flying
{
protected:
	CVehicle_Flying_EnemyBiplane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVehicle_Flying_EnemyBiplane(const CVehicle_Flying_EnemyBiplane& rhs);
	virtual ~CVehicle_Flying_EnemyBiplane() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	HRESULT Render_ShadowDepth();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

public:
	const Vec2& Get_Trace_StartEnd_Distance() { return m_vTraceStartEndDistance; }
	const Vec2& Get_Attack_StartEnd_Distance() { return m_vAttackStartEndDistance; }

public:
	class CGameObject* Get_Target() { return m_pTarget; }


private:
	virtual HRESULT Ready_Components();
	virtual HRESULT	Ready_Colliders();
	virtual HRESULT Ready_States();

private:
	void Update_RiderState();

private:
	class CUI_Grandprix_RaderIcon* m_pRaderIcon = { nullptr };
	class CUI_Minigame_WorldHP* m_pHP = { nullptr };
	class CUI_Minigame_Aim* m_pAim = { nullptr };
	_bool m_bUseRigidbody = { true };

private:
	class CGameObject* m_pTarget = nullptr;

private:
	Vec2 m_vTraceStartEndDistance = { 70.f, 100.f };
	Vec2 m_vAttackStartEndDistance = { 15.f, 20.f };



public:
	static CVehicle_Flying_EnemyBiplane* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

