#include "stdafx.h"
#include "Pigeon.h"
#include "GameInstance.h"

#include "State_Animal_Fly.h"

CPigeon::CPigeon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CAnimals(pDevice, pContext, strObjectTag, eType)
{
}

CPigeon::CPigeon(const CPigeon& rhs)
	: CAnimals(rhs)
{
}

HRESULT CPigeon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPigeon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CPigeon::Tick(_float fTimeDelta)
{
	m_pStateMachineCom->Tick_State(fTimeDelta);


	__super::Tick(fTimeDelta);
}

void CPigeon::LateTick(_float fTimeDelta)
{
	//__super::LateTick(fTimeDelta);
	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	if (nullptr != m_pControllerCom)
		m_pControllerCom->LateTick_Controller(fTimeDelta);

	if (true == GI->Intersect_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 20.0f))
	{
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_SHADOW, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_NONBLEND, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);

	}

}

HRESULT CPigeon::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}


HRESULT CPigeon::Ready_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if(nullptr != pArg)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, static_cast<OBJECT_INIT_DESC*>(pArg)->vStartPosition);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_AnimShader"), reinterpret_cast<CComponent**>(&m_pAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Animal_Pigeon"),
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

HRESULT CPigeon::Ready_State()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Pigeon.ao|SKM_Pigeon.ao|SKM_Pigeon.ao|Pigeon_run");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_FLY, CState_Animal_Fly::Create(m_pStateMachineCom, strAnimationNames));

	m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_FLY);

	return S_OK;
}

HRESULT CPigeon::Ready_Collider()
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

void CPigeon::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CPigeon::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CPigeon::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

CPigeon* CPigeon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CPigeon* pInstance = new CPigeon(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CPigeon");
		Safe_Release<CPigeon*>(pInstance);
	}

	return pInstance;
}

CGameObject* CPigeon::Clone(void* pArg)
{
	CPigeon* pInstance = new CPigeon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CPigeon");
		Safe_Release<CPigeon*>(pInstance);
	}

	return pInstance;
}

void CPigeon::Free()
{
	__super::Free();

}
