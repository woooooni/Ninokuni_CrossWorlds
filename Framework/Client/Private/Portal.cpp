#include "stdafx.h"
#include "GameInstance.h"
#include "Portal.h"
#include "Level_Loading.h"
#include "Effect_Manager.h"
#include "Vfx.h"

CPortal::CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Portal", OBJ_TYPE::OBJ_PORTAL)
{
}

CPortal::CPortal(const CPortal& rhs)
	: CGameObject(rhs)
{
}


HRESULT CPortal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPortal::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	PORTAL_DESC* pPortalDesc = (PORTAL_DESC*)pArg;

	m_eCurrentLevel = pPortalDesc->eCurrentLevel;
	m_eNextLevel = pPortalDesc->eNextLevel;
	m_vNextPos = pPortalDesc->vNextPos;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Collider()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(pPortalDesc->vStartPosition));

	return S_OK;
}

void CPortal::Tick(_float fTimeDelta)
{
	if (nullptr == pEffectObject)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_PortalPoint"), m_pTransformCom->Get_WorldMatrix(), nullptr, &pEffectObject);
		Safe_AddRef(pEffectObject);
	}

	GI->Add_CollisionGroup(COLLISION_GROUP::PORTAL, this);
	__super::Tick(fTimeDelta);
}

void CPortal::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif // DEBUG
}

HRESULT CPortal::Render()
{
	return S_OK;
}

void CPortal::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if (OBJ_TYPE::OBJ_CHARACTER == tInfo.pOther->Get_ObjectType() && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		if (FAILED(GI->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, m_eNextLevel, L""))))
			MSG_BOX("Portal Failde Activate");
	}
}

void CPortal::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CPortal::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CPortal::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPortal::Ready_Collider()
{
	CCollider_AABB::AABB_COLLIDER_DESC AABBDesc;
	BoundingBox AABBBox;
	AABBBox.Extents = { 2.f, 1.f, 1.f};
	AABBDesc.tBox = AABBBox;
	AABBDesc.pNode = nullptr;
	AABBDesc.pOwnerTransform = m_pTransformCom;
	AABBDesc.ModelPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	AABBDesc.vOffsetPosition = { 0.f, AABBBox.Extents.y, 0.f };

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::AABB, CCollider::DETECTION_TYPE::BODY, &AABBDesc)))
		return E_FAIL;

	return S_OK;
}


CPortal* CPortal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPortal* pInstance = new CPortal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CPortal");
		Safe_Release<CPortal*>(pInstance);
	}

	return pInstance;
}

CGameObject* CPortal::Clone(void* pArg)
{
	CPortal* pInstance = new CPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CPortal");
		Safe_Release<CPortal*>(pInstance);
	}

	return pInstance;
}

void CPortal::Free()
{
	__super::Free();
	
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	
	if (nullptr != pEffectObject)
	{
		pEffectObject->Set_Dead(true);
		Safe_Release(pEffectObject);
	}
}
