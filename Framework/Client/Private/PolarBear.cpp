#include "stdafx.h"
#include "PolarBear.h"
#include "GameInstance.h"

#include "State_Animal_Idle.h"
#include "State_Animal_Run.h"
#include "State_Animal_Walk.h"
#include "State_Animal_Lift.h"

CPolarBear::CPolarBear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CAnimals(pDevice, pContext, strObjectTag, eType)
{
}

CPolarBear::CPolarBear(const CPolarBear& rhs)
	: CAnimals(rhs)
{
}

HRESULT CPolarBear::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPolarBear::Initialize(void* pArg)
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

void CPolarBear::Tick(_float fTimeDelta)
{
	m_pStateMachineCom->Tick_State(fTimeDelta);


	__super::Tick(fTimeDelta);
}

void CPolarBear::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CPolarBear::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}


HRESULT CPolarBear::Ready_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (nullptr != pArg)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, static_cast<OBJECT_INIT_DESC*>(pArg)->vStartPosition);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_AnimShader"), reinterpret_cast<CComponent**>(&m_pAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Animal_PolarBear"),
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



	m_pModelCom->Set_Animation(0);

	return S_OK;
}

HRESULT CPolarBear::Ready_State()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_PolarBear.ao|PolarBear_Idle01");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_IDLE, CState_Animal_Idle::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_PolarBear.ao|PolarBear_Run");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_RUN, CState_Animal_Run::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_PolarBear.ao|PolarBear_Walk");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_WALK, CState_Animal_Walk::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_PolarBear.ao|PolarBear_LiftedStart");
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_PolarBear.ao|PolarBear_LiftedLoop");
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_PolarBear.ao|PolarBear_LiftedLoop02");
	strAnimationNames.push_back(L"SKM_Dochi.ao|SKM_Dochi.ao|SKM_PolarBear.ao|PolarBear_LiftedFinish");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_LIFT, CState_Animal_Lift::Create(m_pStateMachineCom, strAnimationNames));


	m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_IDLE);

	return S_OK;
}

HRESULT CPolarBear::Ready_Collider()
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

void CPolarBear::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CPolarBear::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CPolarBear::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

CPolarBear* CPolarBear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CPolarBear* pInstance = new CPolarBear(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CPolarBear");
		Safe_Release<CPolarBear*>(pInstance);
	}

	return pInstance;
}

CGameObject* CPolarBear::Clone(void* pArg)
{
	CPolarBear* pInstance = new CPolarBear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CPolarBear");
		Safe_Release<CPolarBear*>(pInstance);
	}

	return pInstance;
}

void CPolarBear::Free()
{
	__super::Free();

}
