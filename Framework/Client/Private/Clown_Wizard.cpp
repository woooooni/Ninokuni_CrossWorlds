#include "stdafx.h"
#include "Clown_Wizard.h"

#include "GameInstance.h"

#include "Clown_WizardBT.h"

CClown_Wizard::CClown_Wizard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CClown_Wizard::CClown_Wizard(const CClown_Wizard& rhs)
	: CMonster(rhs)
{
}

HRESULT CClown_Wizard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CClown_Wizard::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	return S_OK;
}

void CClown_Wizard::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::NUM_8))
	{
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK];
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND];
	}
	if (KEY_TAP(KEY::F4) || KEY_TAP(KEY::F5))
	{
		m_tStat.fHp -= 10.f;
	}

	// << : Test 
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

void CClown_Wizard::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CClown_Wizard::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CClown_Wizard::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CClown_Wizard::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CClown_Wizard::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CClown_Wizard::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CClown_Wizard::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-10.f, 1.f, 6.f, 1.f));
	m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	/* 로밍 경로(임시) */
	m_vecRoamingArea.push_back(XMVectorSet(-15.f, 1.f, 6.f, 1.f));
	m_vecRoamingArea.push_back(XMVectorSet(-7.f, 1.f, -3.f, 1.f));
	m_vecRoamingArea.push_back(m_vOriginPos);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For. Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Clown_Wizard"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	//CRigidBody::RIGID_BODY_DESC RigidDesc;
	//RigidDesc.pNavigation = m_pNavigationCom;
	//RigidDesc.pTransform = m_pTransformCom;
	//
	//
	//RigidDesc.PhysXDesc.vOffsetPos = { 0.f, 0.f, 0.f };
	//RigidDesc.PhysXDesc.vExtents = { 5.f, 5.f, 10.f };
	//
	//RigidDesc.PhysXDesc.eColliderType = PHYSX_COLLIDER_TYPE::BOX;
	//RigidDesc.PhysXDesc.eRigidType = PHYSX_RIGID_TYPE::DYNAMIC;
	//
	//RigidDesc.PhysXDesc.bLockAngle_X = true;
	//RigidDesc.PhysXDesc.bLockAngle_Y = false;
	//RigidDesc.PhysXDesc.bLockAngle_Z = true;
	//
	//RigidDesc.PhysXDesc.bKinematic = false;
	//RigidDesc.PhysXDesc.fAngularDamping = 30.f;
	//RigidDesc.PhysXDesc.fDensity = 1.f;
	//
	//
	//RigidDesc.PhysXDesc.fStaticFriction = 0.f;
	//RigidDesc.PhysXDesc.fDynamicFriction = 1.f;
	//RigidDesc.PhysXDesc.fRestitution = 0.f;
	//
	//RigidDesc.PhysXDesc.fMaxVelocity = 10.f;
	//RigidDesc.PhysXDesc.pGameObject = this;
	//RigidDesc.PhysXDesc.bKinematic = true;
	//
	///* For. Com_RigidBody*/
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CClown_Wizard::Ready_States()
{
	m_pBTCom = CClown_WizardBT::Create(m_pDevice, m_pContext, this);

	m_tStat.fMaxHp = 100;
	m_tStat.fHp = 100;

	return S_OK;
}

HRESULT CClown_Wizard::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);

	/* 원거리 공격 범위 */
	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 6.f;
	ColliderDesc.tSphere = tSphere;

	ColliderDesc.pOwner = this;
	ColliderDesc.pNode = nullptr;
	ColliderDesc.pOwnerTransform = m_pTransformCom;
	ColliderDesc.ModePivotMatrix = m_pModelCom->Get_PivotMatrix();
	ColliderDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);
	ColliderDesc.bLockAngle_X = false;
	ColliderDesc.bLockAngle_Y = false;
	ColliderDesc.bLockAngle_Z = false;

	ColliderDesc.fAngularDamping = 0.f;
	ColliderDesc.fDensity = 1.f;

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

CClown_Wizard* CClown_Wizard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CClown_Wizard* pInstance = new CClown_Wizard(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CClown_Wizard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CClown_Wizard::Clone(void* pArg)
{
	CClown_Wizard* pInstance = new CClown_Wizard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CClown_Wizard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClown_Wizard::Free()
{
	__super::Free();

}
