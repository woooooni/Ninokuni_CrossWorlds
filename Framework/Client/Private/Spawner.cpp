#include "stdafx.h"
#include "Spawner.h"

#include "GameInstance.h"

CSpawner::CSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_SPAWNER)
{
}

CSpawner::CSpawner(const CSpawner& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSpawner::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSpawner::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSpawner::LateTick(_float fTimeDelta)
{
	// 랜더 그룹 추가하기.
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
//#ifdef _DEBUG
//	for (auto& iter : m_Colliders)
//	{
//		for (auto& pCollider : iter.second)
//		{
//			pCollider->Render();
//		}
//	}
//#endif
	__super::LateTick(fTimeDelta);
}

HRESULT CSpawner::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSpawner::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpawner::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 1.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = {};
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

void CSpawner::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
}
