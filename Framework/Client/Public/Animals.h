#pragma once

#include "DynamicObject.h"

BEGIN(Client)

class CAnimals abstract : public CDynamicObject
{
public:
	enum STATE { STATE_IDLE, STATE_LIFT, STATE_RUN, STATE_STAND, STATE_WALK, STATE_FLY, TATE_END };
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

//public:
//	void Find_MinObject();

public:
	const Vec4& Get_Center() const { return m_vCenter; }
	void Set_Lift(_bool bLift) { m_bLift = bLift; }
	const _bool& Lifting() const { return m_bLift; }

protected:
	Vec4 m_vCenter;
	_bool m_bLift = false;

protected:
	ANIMODEL_INSTANCE_DESC m_AnimInstanceDesc = {};

public:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Free() override;
};

END

