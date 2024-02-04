#pragma once

#include "Vehicle.h"

BEGIN(Client)
class CVehicle_Flying abstract : public CVehicle
{
public:
	typedef struct tagGrandprixEnemyDesc : public CVehicle::VEHICLE_DESC
	{
		_bool bIsEnemy = false;

		_float fMaxHP = 50000.f;
		_float fCurHP = 50000.f;
	
	} PLANE_STAT;

protected:
	CVehicle_Flying(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, OBJ_TYPE eObjType);
	CVehicle_Flying(const CVehicle_Flying& rhs);
	virtual ~CVehicle_Flying() = default;

public:
	const PLANE_STAT& Get_Stat() { return m_tStat; }

	void Set_Routes();

	vector<Vec4>* Get_Routes() { return &m_RoutePoints; }
	Vec4 Get_RoutePoint(_uint iIndex) { return m_RoutePoints[iIndex]; }
	_uint Get_CurIndex() { return m_iRouteIndex; }
	void Set_CurIndex(_uint iIndex) { m_iRouteIndex = iIndex; }

	// 엔지니어로 옮기자
	vector<Vec4>* Get_TakeOffRoutes() { return &m_TakeOffPoints; }
	Vec4 Get_TakeOffRoutePoint(_uint iIndex) { return m_TakeOffPoints[iIndex]; }
	_uint Get_CurTakeOffIndex() { return m_iTakeOffIndex; }
	void Set_CurTakeOffIndex(_uint iIndex) { m_iTakeOffIndex = iIndex; }

	_bool Is_Pass(_uint iIndex) { return m_bPass[iIndex]; }
	void Set_Pass(_uint iIndex, _bool bPass) { m_bPass[iIndex] = bPass; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	HRESULT Render_ShadowDepth();

public:
	void Set_Infinite(_bool bInfinite, _float fTime)
	{
		m_bInfinite = bInfinite;
		m_fAccInfinite = 0.f;
		m_fInfiniteTime = fTime;
	}

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
	virtual HRESULT	Ready_Colliders();
	virtual HRESULT Ready_States();
	HRESULT Ready_Routes();

protected:
	PLANE_STAT m_tStat;

	Vec4 m_vStartPos = {};

	vector<Vec4> m_RoutePoints;
	vector<Vec4> m_TakeOffPoints;

	_uint m_iRouteIndex = { 0 };
	_uint m_iTakeOffIndex = { 0 };

	_float m_fAccEffect = { 0.f };
	_bool m_bPass[13];

protected:
	_bool m_bInfinite = false;
	_float m_fInfiniteTime = 0.f;
	_float m_fAccInfinite = 0.f;

public:
	virtual void Free() override;
};
END

