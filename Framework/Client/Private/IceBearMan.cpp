#include "stdafx.h"
#include "IceBearMan.h"

#include "GameInstance.h"

#include "IceBearManBT.h"
#include "UI_MonsterHP_World.h"
#include "UIDamage_Manager.h"

#include "Quest_Manager.h"

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
	
	if (pArg != nullptr)
	{
		OBJECT_INIT_DESC tInfo = *(OBJECT_INIT_DESC*)pArg;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, tInfo.vStartPosition);
		m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	}

	if (FAILED(__super::Ready_RoamingPoint()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);

	if (FAILED(Ready_States()))
		return E_FAIL;

	CGameObject* pHPBar = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pHPBar)
		return E_FAIL;
	
	m_pHPBar = dynamic_cast<CUI_MonsterHP_World*>(pHPBar);
	m_pHPBar->Set_Owner(this, m_tStat.eElementType, 2.5f);

	m_vBloomPower = _float3(0.25f, 0.25f, 0.25f);

	return S_OK;
}

void CIceBearMan::Tick(_float fTimeDelta)
{
	if (nullptr != m_pHPBar)
		m_pHPBar->Tick(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CIceBearMan::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pHPBar)
		m_pHPBar->LateTick(fTimeDelta);

	__super::LateTick(fTimeDelta);
}

HRESULT CIceBearMan::Render()
{
	__super::Render();

	if (nullptr != m_pHPBar)
		m_pHPBar->Render();

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

	/* 피격 */
	if (m_tStat.fHp > 0.f)
	{
		if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
			tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
		{
			if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
			{
				m_pTransformCom->LookAt_ForLandObject(dynamic_cast<CTransform*>(tInfo.pOther->Get_Component<CTransform>(TEXT("Com_Transform")))->Get_Position());

				/* Blow */
				if (tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::BLOW)
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD] = true;

					On_Damaged(tInfo);

					m_pModelCom->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_KnockUp"));

					m_pRigidBodyCom->Add_Velocity(-m_pTransformCom->Get_Look(), m_tStat.fAirVelocity, false);
					m_pRigidBodyCom->Add_Velocity({ 0.f, 1.f, 0.f, 1.f }, m_tStat.fAirVelocity / 1.5f, false);

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOW] = true;

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;
				}

				/* Air || Bound */
				else if (tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::AIR_BORNE ||
					tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::BOUND)
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD] = true;

					On_Damaged(tInfo);

					m_pModelCom->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_KnockUp"));
					m_pRigidBodyCom->Add_Velocity({ 0.f, 1.f, 0.f, 1.f }, m_tStat.fAirVelocity / 2.f, false);

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;
				}

				/* Stun */
				else if (tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::STUN)
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD] = false;

					On_Damaged(tInfo);

					if (!m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOW] && !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_AIR])
					{
						if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() != TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Stun"))
							m_pModelCom->Set_Animation(TEXT("SKM_IceBearMan_Water.ao|IceBearMan_Stun"));

						m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_STUN] = true;
					}

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;
				}

				/* Hit */
				else if (tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::STRONG ||
					tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::WEAK)
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD] = false;

					On_Damaged(tInfo);

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_WEAK] = true;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;
				}
			}
		}
	}
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

	strKorName = TEXT("얼음곰형");
	strSubName = TEXT("코에루크 설원");
	m_tStat.eElementType = ELEMENTAL_TYPE::WATER;

	m_tStat.iLv = 7;
	m_tStat.fMaxHp = 250;
	m_tStat.fHp = 250;
	m_tStat.iAtk = 15;
	m_tStat.iDef = 3;

	m_tStat.fAirVelocity = 7.5f;
	m_tStat.fAirDeadVelocity = 12.f;

	return S_OK;
}

HRESULT CIceBearMan::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 80.f, 100.f, 80.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 100.f, -15.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	/* Atk */
	OBBBox.Extents = { 0.f, 0.f, 0.f };
	OBBDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;

	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 1.6f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 100.f, 0.f);

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
	
	Safe_Release(m_pHPBar);
}
