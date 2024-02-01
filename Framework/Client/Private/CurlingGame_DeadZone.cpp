#include "stdafx.h"
#include "..\Public\CurlingGame_DeadZone.h"

#include "GameInstance.h"

CCurlingGame_DeadZone::CCurlingGame_DeadZone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CCurlingGame_Prop(pDevice, pContext, strObjectTag)
{
}

CCurlingGame_DeadZone::CCurlingGame_DeadZone(const CCurlingGame_DeadZone& rhs)
	: CCurlingGame_Prop(rhs)
{
}

HRESULT CCurlingGame_DeadZone::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_DeadZone::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	/* Type */
	m_eCGType = CG_TYPE::CG_DEADZONE;	

	return S_OK;
}

void CCurlingGame_DeadZone::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCurlingGame_DeadZone::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CCurlingGame_DeadZone::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCurlingGame_DeadZone::Ready_Colliders()
{
	/* Obb Collider */
	BoundingOrientedBox OBBBox;
	{
		ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));
		XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
		OBBBox.Extents = { 1700.f, 500.f, 200.f };
	}

	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	{
		ZeroMemory(&OBBDesc, sizeof OBBDesc);
		OBBDesc.tBox = OBBBox;
		OBBDesc.pNode = nullptr;
		OBBDesc.pOwnerTransform = m_pTransformCom;
		OBBDesc.vOffsetPosition = Vec3(0.f, OBBBox.Extents.y * 0.5f, 0.f);
		XMStoreFloat4x4(&OBBDesc.ModelPivotMatrix, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f)));
	}

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, m_iColliderDetectionType, &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

CCurlingGame_DeadZone* CCurlingGame_DeadZone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCurlingGame_DeadZone* pInstance = new CCurlingGame_DeadZone(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CCurlingGame_DeadZone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCurlingGame_DeadZone::Clone(void* pArg)
{
	CCurlingGame_DeadZone* pInstance = new CCurlingGame_DeadZone(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CCurlingGame_DeadZone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCurlingGame_DeadZone::Free()
{
	__super::Free();
}
