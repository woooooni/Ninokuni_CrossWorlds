#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CRigidbody;
class CTransform;
class CStateMachine;
class CPhysX_Controller;
END

BEGIN(Client)

class CGrandprix_Enemy abstract : public CGameObject
{
public:
	enum ENEMY_STATE { FLYING_RUNSTART, FLYING_STAND, FLYING_RUN, FLYING_TRACE, FLYING_DIE, ENEMY_STATE_END };

protected:
	CGrandprix_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CGrandprix_Enemy(const CGrandprix_Enemy& rhs);
	virtual ~CGrandprix_Enemy() = default;

public:
	virtual void Set_Active(_bool bActive) { m_bActive = bActive; }
	_bool Is_Active() { return m_bActive; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

	virtual HRESULT Render_Instance_AnimModel(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) override;

	virtual HRESULT Render_Instance_AnimModel_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) override;


public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;

protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Colliders() PURE;		
	virtual HRESULT Ready_States() PURE;

protected:
	ANIMODEL_INSTANCE_DESC m_AnimInstanceDesc = {};

protected:
	class CRigidBody* m_pRigidBodyCom = { nullptr };
	class CPhysX_Controller* m_pControllerCom = { nullptr };
	class CStateMachine* m_pStateCom = { nullptr };
	class CTransform* m_pTransformCom = { nullptr };
	class CRenderer* m_pRendererCom = { nullptr };
	class CTexture* m_pTextureCom = { nullptr };
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };

protected:
	_bool m_bActive = { false };

public:
	virtual void Free() override;
};
END

