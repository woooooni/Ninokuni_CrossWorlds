#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CRigidBody;
class CTransform;
END

BEGIN(Client)

class CCurlingGame_Prop abstract : public CGameObject
{
protected:
	enum CG_TYPE { CG_STONE, CG_WALL, CG_ARROW, CG_DEADZONE, CG_TYPEEND };

protected:
	CCurlingGame_Prop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCurlingGame_Prop(const CCurlingGame_Prop& rhs);
	virtual ~CCurlingGame_Prop() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	
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
	const CG_TYPE& Get_CGType() const { return m_eCGType; }

	virtual void Set_Active(const _bool& bActive) { m_bActive = bActive; }
	const _bool Is_Active() const { return m_bActive; }

	class CRigidBody* Get_Rigidbody() const { return m_pRigidBodyCom; }
	class CTransform* Get_Transform() const { return m_pTransformCom; }
	
protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Colliders() PURE;

protected:
	class CModel*		m_pModelCom		= nullptr;
	class CShader*		m_pShaderCom	= nullptr;
	class CRenderer*	m_pRendererCom	= nullptr;
	class CRigidBody*	m_pRigidBodyCom = nullptr;
	class CTransform*	m_pTransformCom = nullptr;

protected:
	ANIMODEL_INSTANCE_DESC m_AnimInstanceDesc = {};

protected:
	CG_TYPE				m_eCGType = CG_TYPE::CG_TYPEEND;

	const _uint			m_iColliderDetectionType = 1; // CCollider::DETECTION_TYPE::BODY;

	_bool				m_bActive = true;

public:
	virtual void Free() override;
};

END

