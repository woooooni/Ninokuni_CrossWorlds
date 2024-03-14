#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CPipeLine;
class CTransform;
class CStateMachine;
class CRigidBody;
class CPhysX_Controller;
END


BEGIN(Client)

class CDynamicObject abstract : public CGameObject
{
public:
	enum DYNAMIC_TYPE { DYNAMIC_ANIMAL, DYNAMIC_PROP, DYNAMIC_END };

protected:
	explicit CDynamicObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int iObjectType);
	explicit CDynamicObject(const CDynamicObject& rhs);
	virtual ~CDynamicObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;


	virtual HRESULT Render_Instance_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

	virtual HRESULT Ready_Components() PURE;

public:
	const DYNAMIC_TYPE Get_DynamicType() const { return m_eType; }

protected:
	void Find_MinDistanceObject();

protected:
	CShader* m_pAnimShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CStateMachine* m_pStateMachineCom = nullptr;
	CRigidBody* m_pRigidBodyCom = nullptr;
	CPhysX_Controller* m_pControllerCom = nullptr;

	wstring m_strDynamicName = L"";


	DYNAMIC_TYPE m_eType = DYNAMIC_TYPE::DYNAMIC_END;
public:
	virtual void Free() override;
};

END

