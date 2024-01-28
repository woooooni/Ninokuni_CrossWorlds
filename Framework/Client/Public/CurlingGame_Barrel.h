#pragma once

#include "Client_Defines.h"
#include "CurlingGame_Prop.h"

BEGIN(Client)

class CCurlingGame_Barrel final : public CCurlingGame_Prop
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
	CCurlingGame_Barrel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCurlingGame_Barrel(const CCurlingGame_Barrel& rhs);
	virtual ~CCurlingGame_Barrel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	virtual HRESULT Render() override;

	virtual HRESULT Render_ShadowDepth() override;

	virtual HRESULT Render_Instance(class CShader* pInstancingShader,
		class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);

	virtual HRESULT Render_Instance_Shadow(class CShader* pInstancingShader,
		class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);

	virtual HRESULT Render_Instance_AnimModel(class CShader* pInstancingShader,
		class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) override;

	virtual HRESULT Render_Instance_AnimModel_Shadow(class CShader* pInstancingShader,
		class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) override;

public:
	void Launch(const Vec3& vDir, const _float& fPower);
	void PutDown();

	const _bool& Is_Launched() const { return m_bLaunched; }
	void Set_OwnerType(const OBJ_TYPE& eType) { m_eOwnerType = eType; }
	const OBJ_TYPE& Get_OwnerType() const { return m_eOwnerType; }

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Colliders() override;

private:
	HRESULT Calculate_ElasticCollision(CGameObject* pOther);
	HRESULT Calculate_ActionAndReaction(class CCurlingGame_Wall* pWall);

private:
	ELASTIC_COLL_DESC	m_tElasticColDesc	= {};
	LERP_FLOAT_DESC		m_tHeightLerpDesc	= {};

	_uint				m_iNumCol			= 0;

	_bool				m_bLaunched			= false;

	OBJ_TYPE			m_eOwnerType		= OBJ_TYPE::OBJ_END;

public:
	static CCurlingGame_Barrel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;

public:
	virtual void Free() override;
};

END

