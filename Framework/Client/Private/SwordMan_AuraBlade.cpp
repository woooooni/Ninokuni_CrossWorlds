#include "stdafx.h"
#include "GameInstance.h"
#include "SwordMan_AuraBlade.h"

#include "Effect_Manager.h"

CSwordMan_AuraBlade::CSwordMan_AuraBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext, L"SwordMan_AuraBlade", OBJ_TYPE::OBJ_CHARACTER_PROJECTILE)
{

}

CSwordMan_AuraBlade::CSwordMan_AuraBlade(const CSwordMan_AuraBlade& rhs)
	: CGameObject(rhs)
	, m_fAccDeletionTime(0.f)
	, m_fDeletionTime(rhs.m_fDeletionTime)
{
}


HRESULT CSwordMan_AuraBlade::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSwordMan_AuraBlade::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSwordMan_AuraBlade::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);

	m_fAccDeletionTime += fTimeDelta;
	if (m_fAccDeletionTime >= m_fDeletionTime)
	{
		Set_Dead(true);
		m_fAccDeletionTime = 0.f;
	}
}

void CSwordMan_AuraBlade::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	GI->Add_CollisionGroup(COLLISION_GROUP::CHARACTER, this);

#ifdef _DEBUG
	m_pRendererCom->Set_PlayerPosition(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}

#endif
}

HRESULT CSwordMan_AuraBlade::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;



	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 0.2f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}


CSwordMan_AuraBlade* CSwordMan_AuraBlade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSwordMan_AuraBlade* pInstance = new CSwordMan_AuraBlade(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CSwordMan_AuraBlade");
		Safe_Release<CSwordMan_AuraBlade*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CSwordMan_AuraBlade::Clone(void* pArg)
{
	CSwordMan_AuraBlade* pInstance = new CSwordMan_AuraBlade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CSwordMan_AuraBlade");
		Safe_Release<CSwordMan_AuraBlade*>(pInstance);
	}

	return pInstance;
}

void CSwordMan_AuraBlade::Free()
{
	__super::Free();
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
