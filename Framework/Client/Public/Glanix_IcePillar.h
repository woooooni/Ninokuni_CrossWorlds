#pragma once
#include "Boss.h"

BEGIN(Client)

class CGlanix;
class CGlanix_IcePillar_Controller;

class CGlanix_IcePillar final : public CGameObject
{
private:
	CGlanix_IcePillar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CGlanix_IcePillar(const CGlanix_IcePillar& rhs);
	virtual ~CGlanix_IcePillar() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public:
	virtual HRESULT Render_Instance(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);
	virtual HRESULT Render_Instance_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CRigidBody* m_pRigidBodyCom = nullptr;
	CPhysX_Controller* m_pControllerCom = { nullptr };

	_bool		m_bShake = false;

private:
	_int		m_iKey;
	Vec4		m_vRotateOriginPos;
	LERP_FLOAT_DESC m_tSpeedDesc;
	CGlanix*	m_pGlanix = nullptr;

public:
	static CGlanix_IcePillar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	friend class CGlanix_IcePillar_Controller;
};

END

