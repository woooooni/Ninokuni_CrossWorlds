#include "stdafx.h"
#include "Quest_DestSpot.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"
#include "Effect_Manager.h"
#include "Vfx.h"

// 임시로 몬스터에 담는다. 충돌 처리 그룹 추가 될 때까지.
CQuest_DestSpot::CQuest_DestSpot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, LAYER_TYPE::LAYER_MONSTER)
{
}

CQuest_DestSpot::CQuest_DestSpot(const CQuest_DestSpot& rhs)
	: CGameObject(rhs)
{

}

HRESULT CQuest_DestSpot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CQuest_DestSpot::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(Vec4*)pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	// 위치가 셋팅되고 이펙트를 생성해야 함. //
	if (FAILED(GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_QuestPoint"), m_pTransformCom->Get_WorldMatrix(), nullptr, &pEffectObject)))
		return E_FAIL;
	Safe_AddRef(pEffectObject);

	return S_OK;
}

void CQuest_DestSpot::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	// 임시로 몬스터에 담는다. 충돌 처리 그룹 추가 될 때까지.
	GI->Add_CollisionGroup(COLLISION_GROUP::MONSTER, this);

	m_pControllerCom->Tick_Controller(fTimeDelta);

}

void CQuest_DestSpot::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pControllerCom->LateTick_Controller(fTimeDelta);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
	m_pRendererCom->Add_Debug(m_pControllerCom);

	m_pRendererCom->Set_PlayerPosition(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
#endif // DEBUG
}

HRESULT CQuest_DestSpot::Render()
{
	// __super::Render();

	return S_OK;
}

HRESULT CQuest_DestSpot::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

void CQuest_DestSpot::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		m_bIsCol = true;
	}
}

void CQuest_DestSpot::Collision_Continue(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		m_bIsCol = true;
	}
}

void CQuest_DestSpot::Collision_Exit(const COLLISION_INFO& tInfo)
{
	int i = 0;
}

HRESULT CQuest_DestSpot::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CQuest_DestSpot::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	::ZeroMemory(&SphereDesc, sizeof(SphereDesc));

	BoundingSphere Sphere;
	ZeroMemory(&Sphere, sizeof(BoundingSphere));
	Sphere.Center = Vec3(0.f, 0.f, 0.f);
	Sphere.Radius = 2.f;
	SphereDesc.tSphere = Sphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = {};
	SphereDesc.vOffsetPosition = Vec3::Zero;
	// XMStoreFloat4x4(&SphereDesc.ModelPivotMatrix, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f)));
	
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider_Sphere::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;



	return S_OK;
}

#pragma endregion

CQuest_DestSpot* CQuest_DestSpot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CQuest_DestSpot* pInstance = new CQuest_DestSpot(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CQuest_DestSpot");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CQuest_DestSpot::Clone(void* pArg)
{
	CQuest_DestSpot* pInstance = new CQuest_DestSpot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CQuest_DestSpot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuest_DestSpot::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pControllerCom);

	if(nullptr != pEffectObject)
		Safe_Release(pEffectObject);
}

