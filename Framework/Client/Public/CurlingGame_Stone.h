#pragma once

#include "Client_Defines.h"
#include "CurlingGame_Prop.h"

BEGIN(Client)

class CCurlingGame_Stone final : public CCurlingGame_Prop
{
public:
	enum STONE_TYPE { BARREL, POT, STONE_TYPEEND };

public:
	typedef struct tagStoneInitDesc
	{
		STONE_TYPE eStoneType = STONE_TYPE::STONE_TYPEEND;

	}STONE_INIT_DESC;

private:
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
	CCurlingGame_Stone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCurlingGame_Stone(const CCurlingGame_Stone& rhs);
	virtual ~CCurlingGame_Stone() = default;

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
	const _bool& Is_Putted() const { return m_bPutted; }
	const _bool& Is_Launched() const { return m_bLaunched; }
	const _bool& Is_Outted() const { return m_bOutted; }
	const _bool& Is_Moving() const { return m_bMoving; }

	const STONE_TYPE& Get_StoneType() const { return m_eStoneType; }

public:
	void Launch(const Vec4& vDir, const _float& fPower);
	void PutDown();

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
	LERP_VEC4_DESC		m_tPosLerpDesc		= {};
	
	_uint				m_iNumCol			= 0;

	_bool				m_bPutted			= false;
	_bool				m_bLaunched			= false;
	_bool				m_bOutted			= false;
	_bool				m_bMoving			= false;

	STONE_TYPE			m_eStoneType		= STONE_TYPE::STONE_TYPEEND;

	const _float		m_fRotateSpeed		= 50.f;

public:
	static CCurlingGame_Stone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;

public:
	virtual void Free() override;
};

END

