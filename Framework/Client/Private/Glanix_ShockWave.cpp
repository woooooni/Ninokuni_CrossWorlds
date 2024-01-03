#include "stdafx.h"
#include "Glanix_ShockWave.h"

#include "GameInstance.h"

CGlanix_ShockWave::CGlanix_ShockWave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_SPAWNER)
{
}

CGlanix_ShockWave::CGlanix_ShockWave(const CGlanix_ShockWave& rhs)
	: CGameObject(rhs)
{
}

HRESULT CGlanix_ShockWave::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlanix_ShockWave::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4((_float4*)pArg));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CGlanix_ShockWave::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pTransformCom->Move(m_pTransformCom->Get_Look(), 10.f, fTimeDelta);
	Vec4 vPosition = m_pTransformCom->Get_Position();
	_float fZ = vPosition.z;

	if (fZ <= 300.f)
		Set_Dead(this);
}

void CGlanix_ShockWave::LateTick(_float fTimeDelta)
{
	// 랜더 그룹 추가하기.
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	__super::LateTick(fTimeDelta);
}

HRESULT CGlanix_ShockWave::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CGlanix_ShockWave::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CGlanix_ShockWave::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 1000.f, 20.f, 100.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = {};
	OBBDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	/* ATK */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;

	// 그려지긴 하는데 터짐. 일단 나중에
	m_pColliderCom->Set_AttackType(CCollider::ATTACK_TYPE::BLOW);

	return S_OK;
}

CGlanix_ShockWave* CGlanix_ShockWave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGlanix_ShockWave* pInstance = new CGlanix_ShockWave(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGlanix_ShockWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGlanix_ShockWave::Clone(void* pArg)
{
	CGlanix_ShockWave* pInstance = new CGlanix_ShockWave(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGlanix_ShockWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix_ShockWave::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
}
