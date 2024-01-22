#pragma once

#include "DynamicObject.h"

BEGIN(Client)

class CAnimals abstract : public CDynamicObject
{
public:
	enum STATE { STATE_IDLE, STATE_LIFT, STATE_RUN, STATE_STAND, STATE_WALK, 
		STATE_FLY, STATE_SWIM, STATE_BACKFILP01, STATE_BACKFILP02, TATE_END };
protected:
	explicit CAnimals(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CAnimals(const CAnimals& rhs);
	virtual ~CAnimals() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public:
	virtual HRESULT Render_Instance_AnimModel(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) override;
	virtual HRESULT Render_Instance_AnimModel_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>&WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc);

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	vector<Vec4>* Get_RomingPoints() { return &m_points; }
	void Set_RomingPoints(vector<Vec4> points) { m_points.swap(points); }
	
	_float* Get_Speed() { return &m_fSpeed; }
	void Set_Speed(_float speed) { m_fSpeed = speed; }

	_int Get_CurRomingPoint() { return m_iCurPointIndex; }
	void Set_CurRomingPoint(_int cur) 
	{ 
		if (cur < 0)
			m_iCurPointIndex = 0;
		else
			m_iCurPointIndex = cur; 
	}

	Vec4 Get_RomingPointIndex(_int iIndex) 
	{
		if (iIndex >= m_points.size())
			return m_points[0]; // 사이즈보다 크면 처음으로 돌아오니까.

		return m_points[iIndex]; 
	}

	const _bool& Get_TurnOnPoint() const { return m_bTurnOnPoint; }
	void Set_TurnOnPoint(_bool first) { m_bTurnOnPoint = first; }

public:
	const Vec4& Get_Center() const { return m_vCenter; }
	void Set_Lift(_bool bLift) { m_bLift = bLift; }
	const _bool& Lifting() const { return m_bLift; }

protected:
	Vec4 m_vCenter;
	_bool m_bLift = false;
	_float m_fSpeed = 2.0f;

	// 이동할 Point
	vector<Vec4> m_points;
	_int m_iCurPointIndex = 0;
	_bool m_bTurnOnPoint = false;

protected:
	ANIMODEL_INSTANCE_DESC m_AnimInstanceDesc = {};

public:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Free() override;
};

END

