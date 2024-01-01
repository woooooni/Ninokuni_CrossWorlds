#include "stdafx.h"
#include "Dochi.h"
#include "GameInstance.h"

#include "State_Animal_Idle.h"
#include "State_Animal_Run.h"
#include "State_Animal_Walk.h"
#include "State_Animal_Lift.h"

CDochi::CDochi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CAnimals(pDevice, pContext, strObjectTag, eType)
{
}

CDochi::CDochi(const CDochi& rhs)
	: CAnimals(rhs)
{
}

HRESULT CDochi::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDochi::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Collider()))
		return E_FAIL;

	m_vCenter = m_pTransformCom->Get_Position();

	return S_OK;
}

void CDochi::Tick(_float fTimeDelta)
{
	m_pStateMachineCom->Tick_State(fTimeDelta);


	__super::Tick(fTimeDelta);
}

void CDochi::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CDochi::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDochi::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDochi::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc)
{
	if (FAILED(__super::Render_Instance_AnimModel(pInstancingShader, pInstancingBuffer, WorldMatrices, TweenDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDochi::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{

	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDochi::Ready_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	OBJECT_INIT_DESC Init_Data = {};

	if (pArg != nullptr)
		Init_Data.vStartPosition = static_cast<OBJECT_INIT_DESC*>(pArg)->vStartPosition;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Init_Data.vStartPosition);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_AnimShader"), reinterpret_cast<CComponent**>(&m_pAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Dochi"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"),
		reinterpret_cast<CComponent**>(&m_pRigidBodyCom), &RigidDesc)))
		return E_FAIL;

	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"),
		reinterpret_cast<CComponent**>(&m_pControllerCom), &ControllerDesc)))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);

	return S_OK;
}

HRESULT CDochi::Ready_State()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_Dochi.ao|Dochi_Idle01");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_IDLE, CState_Animal_Idle::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_Dochi.ao|Dochi_Run");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_RUN, CState_Animal_Run::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_Dochi.ao|Dochi_Walk");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_WALK, CState_Animal_Walk::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_Dochi.ao|Dochi_LiftedStart");
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_Dochi.ao|Dochi_LiftedLoop");
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_Dochi.ao|Dochi_LiftedLoop2");
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_Dochi.ao|Dochi_LiftedFinish");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_LIFT, CState_Animal_Lift::Create(m_pStateMachineCom, strAnimationNames));


	m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_IDLE);

	return S_OK;
}

HRESULT CDochi::Ready_Collider()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	::ZeroMemory(&SphereDesc, sizeof(SphereDesc));

	BoundingSphere Sphere;
	ZeroMemory(&Sphere, sizeof(BoundingSphere));

	Sphere.Center = Vec3(0.f, 0.f, 0.f);
	Sphere.Radius = 1.f;

	SphereDesc.tSphere = Sphere;
	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3::Zero;

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider_Sphere::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CDochi::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CDochi::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CDochi::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

CDochi* CDochi::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CDochi* pInstance = new CDochi(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CDochi");
		Safe_Release<CDochi*>(pInstance);
	}

	return pInstance;
}

CGameObject* CDochi::Clone(void* pArg)
{
	CDochi* pInstance = new CDochi(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CDochi");
		Safe_Release<CDochi*>(pInstance);
	}

	return pInstance;
}

void CDochi::Free()
{
	__super::Free();

}
