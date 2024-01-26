#pragma once

#include "Client_Defines.h"
#include "Shuffleboard_Prop.h"

BEGIN(Client)

class CShuffleboard_Barrel final : public CShuffleboard_Prop
{
public:
	typedef struct tagElasticCollisionDesc
	{
		_bool bSet = false;
		Vec3 vVelocity = Vec3::Zero;

		void Set(const Vec3 v)
		{
			vVelocity = v;
			bSet = true;
		}

		void Clear()
		{
			vVelocity = Vec3::Zero;
			bSet = false;
		}

	}ELASTIC_COLL_DESC;

private:
	CShuffleboard_Barrel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CShuffleboard_Barrel(const CShuffleboard_Barrel& rhs);
	virtual ~CShuffleboard_Barrel() = default;

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

public:
	void PutDown();

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Colliders() override;

private:
	HRESULT Calculate_ElasticCollision(CGameObject* pOther);

private:
	ELASTIC_COLL_DESC	m_tElasticColDesc = {};

public:
	static CShuffleboard_Barrel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;

	LERP_FLOAT_DESC m_tHeightLerpDesc = {};

public:
	virtual void Free() override;
};

END

