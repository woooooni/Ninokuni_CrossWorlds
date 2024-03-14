#pragma once

#include "Vehicle_Flying.h"

BEGIN(Client)
class CVehicle_Flying_EnemyBiplane final : public CVehicle_Flying
{
public:
	enum BIPLANE_TRAIL { LEFT_WING, RIGHT_WING, TAIL, BIPLANE_TRAIL_END };

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
	void Start_Trail(BIPLANE_TRAIL eTrailType = BIPLANE_TRAIL::BIPLANE_TRAIL_END);
	void Generate_Trail(const wstring& strDiffuseTextureName, const wstring& strAlphaTextureName, const wstring& strDistortionTextureName, const _float4& vColor, _uint iVertexCount, BIPLANE_TRAIL eTrailType = BIPLANE_TRAIL::BIPLANE_TRAIL_END);
	void Stop_Trail(BIPLANE_TRAIL eTrailType = BIPLANE_TRAIL::BIPLANE_TRAIL_END);

	class CTrail* Get_Trail(BIPLANE_TRAIL eTrailType)
	{
		if (eTrailType >= BIPLANE_TRAIL::BIPLANE_TRAIL_END)
			return nullptr;

		return m_pTrails[eTrailType];
	}

public:
	const Vec2& Get_Trace_StartEnd_Distance() { return m_vTraceStartEndDistance; }
	const Vec2& Get_Attack_StartEnd_Distance() { return m_vAttackStartEndDistance; }

public:
	class CGameObject* Get_Target() { return m_pTarget; }

public:
	void Set_Infinite(_bool bInfinite, _float fInfiniteTime);


private:
	virtual HRESULT Ready_Components();
	virtual HRESULT	Ready_Colliders();
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Trails();

private:
	void Update_RiderState();
	_bool Check_Use_Skill();

private:
	void On_Damaged(const COLLISION_INFO& tInfo);

private:
	class CUI_Grandprix_RaderIcon* m_pRaderIcon = { nullptr };
	class CUI_Minigame_WorldHP* m_pHP = { nullptr };
	class CUI_Minigame_Aim* m_pAim = { nullptr };
	_bool m_bUseRigidbody = { true };

private:
	class CGameObject* m_pTarget = nullptr;

private:
	_bool m_bLaunch_Pattern1 = false;
	_bool m_bLaunch_Pattern2 = false;
	_bool m_bLaunch_Pattern3 = false;


private:
	class CTrail* m_pTrails[BIPLANE_TRAIL::BIPLANE_TRAIL_END] = {};

private:
	_bool m_bInfinite = true;
	_float m_fInfiniteTime = 99999.f;
	_float m_fAccInfinite = 0.f;

private:
	Vec2 m_vTraceStartEndDistance = { 70.f, 100.f };
	Vec2 m_vAttackStartEndDistance = { 15.f, 20.f };

private:
	_float4 m_vDissolveColor = _float4(0.427f, 0.894f, 1.f, 1.f);
	_float  m_fDissolveTotal = 10.f;
	_float  m_fDissolveDuration = 5.f;
	_float  m_fDissolveWeight = 0.f;
	_float  m_fDissolveSpeed = 5.f;

	class CTexture* m_pDissolveTexture = nullptr;

	_float3 m_vBloomPower = _float3(0.f, 0.f, 0.f);

public:
	static CVehicle_Flying_EnemyBiplane* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

