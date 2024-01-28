#include "stdafx.h"
#include "PumpkinCandle.h"

#include "GameInstance.h"

#include "PumpkinCandleBT.h"
#include "UI_MonsterHP_World.h"
#include "UIDamage_Manager.h"
#include "UI_Minimap_Icon.h"
#include "UIMinimap_Manager.h"

#include "Quest_Manager.h"

#include "InvasionState_Attack.h"
#include "InvasionState_Idle01.h"
#include "InvasionState_Idle02.h"
#include "InvasionState_Dead.h"

CPumpkinCandle::CPumpkinCandle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CPumpkinCandle::CPumpkinCandle(const CPumpkinCandle& rhs)
	: CMonster(rhs)
{
}

HRESULT CPumpkinCandle::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CPumpkinCandle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		OBJECT_INIT_DESC tInfo = *(OBJECT_INIT_DESC*)pArg;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, tInfo.vStartPosition);
		m_pControllerCom->Set_EnterLevel_Position(m_pTransformCom->Get_Position());
		m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	}

	if (FAILED(__super::Ready_RoamingPoint()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_pModelCom->Set_Animation(12);

	if (FAILED(Ready_States()))
		return E_FAIL;

	CGameObject* pHPBar = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pHPBar)
		return E_FAIL;

	m_pHPBar = dynamic_cast<CUI_MonsterHP_World*>(pHPBar);
	m_pHPBar->Set_Owner(this, m_tStat.eElementType, 1.5f);

	m_vBloomPower = _float3(0.8f, 0.8f, 0.8f);

	//m_pMinimapIcon
	CGameObject* pIcon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minimap_Icon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pIcon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Minimap_Icon*>(pIcon))
		return E_FAIL;
	m_pMinimapIcon = dynamic_cast<CUI_Minimap_Icon*>(pIcon);
	m_pMinimapIcon->Set_Owner(this);

	return S_OK;
}

void CPumpkinCandle::Tick(_float fTimeDelta)
{
	if (!m_bIsInvasion)
	{
		if (nullptr != m_pHPBar)
			m_pHPBar->Tick(fTimeDelta);

		if (nullptr != m_pMinimapIcon)
		{
			if (true == CUIMinimap_Manager::GetInstance()->Is_InMinimap(m_pTransformCom))
				m_pMinimapIcon->Set_Active(true);
			else
				m_pMinimapIcon->Set_Active(false);
		}

		if (nullptr != m_pMinimapIcon)
			m_pMinimapIcon->Tick(fTimeDelta);
	}


	__super::Tick(fTimeDelta);
}

void CPumpkinCandle::LateTick(_float fTimeDelta)
{
	if (!m_bIsInvasion)
	{
		if (nullptr != m_pHPBar)
			m_pHPBar->LateTick(fTimeDelta);

		if (nullptr != m_pMinimapIcon)
			m_pMinimapIcon->LateTick(fTimeDelta);
	}

	__super::LateTick(fTimeDelta);
}

HRESULT CPumpkinCandle::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CPumpkinCandle::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CPumpkinCandle::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);

	// 애니메이션 이름 펌킨으로 바꾸기

	/* 피격 */
	if (m_tStat.fHp > 0.f)
	{
		if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER 
			|| tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER_PROJECTILE
			|| tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_DEFENCE_TOWER) &&
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

					m_pModelCom->Set_Animation(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_KnockUp_Start"));

					m_pRigidBodyCom->Add_Velocity(-m_pTransformCom->Get_Look(), m_tStat.fAirVelocity, false);
					m_pRigidBodyCom->Add_Velocity({ 0.f, 1.f, 0.f, 1.f }, m_tStat.fAirVelocity / 1.5f, false);

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOW] = true;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_AIR] = false;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;
				}

				/* Air || Bound */
				else if (tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::AIR_BORNE ||
					tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::BOUND)
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD] = true;

					On_Damaged(tInfo);

					m_pModelCom->Set_Animation(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_KnockUp_Start"));
					m_pRigidBodyCom->Add_Velocity({ 0.f, 1.f, 0.f, 1.f }, m_tStat.fAirVelocity / 2.f, false);

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOW] = false;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_AIR] = true;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;
				}

				/* Stun */
				else if (tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::STUN)
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD] = false;

					On_Damaged(tInfo);

					if (!m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOW] && !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_AIR])
					{

						if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() != TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_Stun"))
							m_pModelCom->Set_Animation(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_Stun"));

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

void CPumpkinCandle::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CPumpkinCandle::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CPumpkinCandle::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CPumpkinCandle::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CPumpkinCandle::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CPumpkinCandle::Ready_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_PumpkinCandle"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 0.5f;
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

HRESULT CPumpkinCandle::Ready_States()
{
	m_pBTCom = CPumpkinCandleBT::Create(m_pDevice, m_pContext, this);

	strKorName = TEXT("호박촛불");
	strSubName = TEXT("마녀의 숲");
	m_tStat.eElementType = ELEMENTAL_TYPE::FIRE;

	m_tStat.iLv = 9;
	m_tStat.fMaxHp = 12500;
	m_tStat.fHp = 12500;
	m_tStat.iAtk = 50;
	m_tStat.iDef = 50;

	m_tStat.fAirVelocity = 7.5f;
	m_tStat.fAirDeadVelocity = 15.f;

	if (m_bIsInvasion)
	{
		list<wstring> strAnimationName;

		strAnimationName.clear();
		strAnimationName.push_back(L"SKM_PumpkinCandle.ao|PumpkinCandle_Attack01");
		m_pStateCom->Add_State(INVASION_STATE_ATTACK, CInvasionState_Attack::Create(m_pStateCom, strAnimationName));

		strAnimationName.clear();
		strAnimationName.push_back(L"SKM_PumpkinCandle.ao|PumpkinCandle_BattleStand");
		m_pStateCom->Add_State(INVASION_STATE_IDLE01, CInvasionState_Idle01::Create(m_pStateCom, strAnimationName));

		strAnimationName.clear();
		strAnimationName.push_back(L"SKM_PumpkinCandle.ao|PumpkinCandle_BattleStand");
		m_pStateCom->Add_State(INVASION_STATE_IDLE02, CInvasionState_Idle02::Create(m_pStateCom, strAnimationName));

		strAnimationName.clear();
		strAnimationName.push_back(L"SKM_PumpkinCandle.ao|PumpkinCandle_KnockUp_Loop");
		m_pStateCom->Add_State(INVASION_STATE_DEAD, CInvasionState_Dead::Create(m_pStateCom, strAnimationName));

		m_pStateCom->Change_State(INVASION_STATE_IDLE01);
	}

	return S_OK;
}

HRESULT CPumpkinCandle::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 40.f, 30.f, 40.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 110.f, 0.f);

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
	tSphere.Radius = 1.2f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 100.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CPumpkinCandle* CPumpkinCandle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CPumpkinCandle* pInstance = new CPumpkinCandle(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CPumpkinCandle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPumpkinCandle::Clone(void* pArg)
{
	CPumpkinCandle* pInstance = new CPumpkinCandle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CPumpkinCandle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPumpkinCandle::Free()
{
	__super::Free();

	Safe_Release(m_pMinimapIcon);
	Safe_Release(m_pHPBar);
}
