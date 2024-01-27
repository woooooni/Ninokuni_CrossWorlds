#include "stdafx.h"
#include "..\Public\CurlingGame_Wall.h"

#include "GameInstance.h"

CCurlingGame_Wall::CCurlingGame_Wall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CCurlingGame_Prop(pDevice, pContext, strObjectTag)
{
}

CCurlingGame_Wall::CCurlingGame_Wall(const CCurlingGame_Wall& rhs)
	: CCurlingGame_Prop(rhs)
{
}

HRESULT CCurlingGame_Wall::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_Wall::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	/* Type */
	m_eCGType = CG_TYPE::CG_WALL;

	/* Trnasform */
	if (nullptr != m_pTransformCom)
	{
		//m_pTransformCom->Set_Scale(Vec3(1.f));
	}

	return S_OK;
}

void CCurlingGame_Wall::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCurlingGame_Wall::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCurlingGame_Wall::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Shader */
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;
	}

	/* For. Com_Model */
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Prop_Wall"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCurlingGame_Wall::Ready_Colliders()
{
	/* Obb Collider */
	BoundingOrientedBox OBBBox;
	{
		ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));
		XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
		OBBBox.Extents = { 300.f, 100.f, 30.f };
	}

	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	{
		ZeroMemory(&OBBDesc, sizeof OBBDesc);
		OBBDesc.tBox = OBBBox;
		OBBDesc.pNode = nullptr;
		OBBDesc.pOwnerTransform = m_pTransformCom;
		OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
		OBBDesc.vOffsetPosition = Vec3(0.f, OBBBox.Extents.y, 0.f);
	}

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, m_iColliderDetectionType, &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

CCurlingGame_Wall* CCurlingGame_Wall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCurlingGame_Wall* pInstance = new CCurlingGame_Wall(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CCurlingGame_Wall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCurlingGame_Wall::Clone(void* pArg)
{
	CCurlingGame_Wall* pInstance = new CCurlingGame_Wall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CCurlingGame_Wall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCurlingGame_Wall::Free()
{
	__super::Free();
}
