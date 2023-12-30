#include "stdafx.h"
#include "IceBearMan.h"

#include "GameInstance.h"

#include "IceBearManBT.h"

CIceBearMan::CIceBearMan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CIceBearMan::CIceBearMan(const CIceBearMan& rhs)
	: CMonster(rhs)
{
}

HRESULT CIceBearMan::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CIceBearMan::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_vector*)pArg);
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, { -5.f, 0.f, -5.f, 1.f });

	if (FAILED(__super::Ready_RoamingPoint()))
		return E_FAIL;
	//m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	//
	///* 로밍 경로(임시) */
	//m_vecRoamingArea.push_back(XMVectorSet(-3.f, 1.f, 1.f, 1.f));
	//m_vecRoamingArea.push_back(XMVectorSet(-1.f, 1.f, 6.f, 1.f));
	//m_vecRoamingArea.push_back(m_vOriginPos);

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	return S_OK;
}

void CIceBearMan::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::HOME))
	{
		_uint iCurAnimIndex = m_pModelCom->Get_CurrAnimationIndex();
		m_pModelCom->Set_Animation(iCurAnimIndex + 1);
	}
	else if (KEY_TAP(KEY::DEL))
	{
		_int iCurAnimIndex = m_pModelCom->Get_CurrAnimationIndex() - 1;
		if (iCurAnimIndex < 0)
			iCurAnimIndex = 0;
		m_pModelCom->Set_Animation(iCurAnimIndex);
	}
	// >> 

	__super::Tick(fTimeDelta);
}

void CIceBearMan::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CIceBearMan::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CIceBearMan::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CIceBearMan::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CIceBearMan::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CIceBearMan::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CIceBearMan::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For. Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_IceBearMan"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
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

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For. Com_RigidBody*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CIceBearMan::Ready_States()
{
	m_pBTCom = CIceBearManBT::Create(m_pDevice, m_pContext, this);

	m_strKorName = TEXT("얼음곰형");
	m_tStat.iLv = 7;
	m_tStat.fMaxHp = 250;
	m_tStat.fHp = 250;

	return S_OK;
}

HRESULT CIceBearMan::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 50.f, 50.f, 50.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	/* Atk */
	OBBDesc.vOffsetPosition = Vec3(0.f, 50.f, -100.f);
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;

	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 2.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CIceBearMan* CIceBearMan::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CIceBearMan* pInstance = new CIceBearMan(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CIceBearMan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIceBearMan::Clone(void* pArg)
{
	CIceBearMan* pInstance = new CIceBearMan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CIceBearMan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBearMan::Free()
{
	__super::Free();

}
