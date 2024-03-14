#include "stdafx.h"
#include "..\Public\RubyCarriage.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "Player.h"

#include "State_CarriageIdle.h"
#include "State_CarriageMove.h"

#include "Monster.h"
#include "MonsterProjectile.h"
#include "Decal.h"
#include "Particle_Manager.h"

CRubyCarriage::CRubyCarriage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CDynamicObject(pDevice, pContext, strObjectTag, eType)
{
}

CRubyCarriage::CRubyCarriage(const CRubyCarriage& rhs)
	: CDynamicObject(rhs)
{

}

HRESULT CRubyCarriage::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRubyCarriage::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CRubyCarriage::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	if (m_tStat.fHp <= 0.0f)
	{
		Set_Dead(true);
		return;
	}
	Compute_CamZ(m_pTransformCom->Get_Position());
	m_pStateMachineCom->Tick_State(fTimeDelta);

	if (m_pRigidBodyCom != nullptr && m_pControllerCom != nullptr)
	{
		m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
		m_pControllerCom->Tick_Controller(fTimeDelta);
	}

	GI->Add_CollisionGroup(COLLISION_GROUP::DEFENCE_TOWER, this);
	__super::Tick(fTimeDelta);
}

void CRubyCarriage::LateTick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	if (nullptr != m_pControllerCom)
		m_pControllerCom->LateTick_Controller(fTimeDelta);

	__super::LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CRubyCarriage::Render()
{
	if (true == m_bDead)
		return S_OK;


	if (FAILED(m_pAnimShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pAnimShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pAnimShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pAnimShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(m_pAnimShaderCom)))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pAnimShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pAnimShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CRubyCarriage::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

void CRubyCarriage::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (m_tStat.fHp > 0.f)
	{
		if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER_PROJECTILE) &&
			tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
		{
			if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
				On_Damaged(tInfo);
		}
	}
}

void CRubyCarriage::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CRubyCarriage::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CRubyCarriage::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_AnimShader"), reinterpret_cast<CComponent**>(&m_pAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_") + m_strObjectTag,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), 
		TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"),
		reinterpret_cast<CComponent**>(&m_pRigidBodyCom), &RigidDesc)))
		return E_FAIL;

	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"),
		reinterpret_cast<CComponent**>(&m_pControllerCom), &ControllerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRubyCarriage::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 150.0f, 190.f, 360.0f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 190.0f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CRubyCarriage::Ready_State()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"Carriage_Idle");
	m_pStateMachineCom->Add_State(STATE_TYPE::STATE_IDLE, CState_CarriageIdle::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"Carriage_Move");
	m_pStateMachineCom->Add_State(STATE_TYPE::STATE_MOVE, CState_CarriageMove::Create(m_pStateMachineCom, strAnimationNames));

	m_pStateMachineCom->Change_State(STATE_TYPE::STATE_IDLE);
	
	return S_OK;
}

HRESULT CRubyCarriage::Ready_States()
{
	m_strKorName = TEXT("·çºñÀÇ ¸¶Â÷");
	m_strSubName = TEXT("¸¶³àÀÇ ½£");
	m_tStat.eElementType = ELEMENTAL_TYPE::WOOD;

	m_tStat.fMaxHp = 30000.0f;
	m_tStat.fHp = 30000.0f;
	m_tStat.iDef = 50.0f;

	return S_OK;
}

void CRubyCarriage::On_Damaged(const COLLISION_INFO& tInfo)
{
	if (m_tStat.fHp <= 0.0f)
		return;

	CMonster* pMonster = nullptr;
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER_PROJECTILE)
	{
		CMonsterProjectile* pProjectile = dynamic_cast<CMonsterProjectile*>(tInfo.pOther);
		if (nullptr == pProjectile)
		{
			MSG_BOX("CMonsterProjectile Cast Failed.");
			return;
		}
		pMonster = pProjectile->Get_Owner();
	}
	else
	{
		pMonster = dynamic_cast<CMonster*>(tInfo.pOther);
	}

	if (nullptr == pMonster)
		return;

	_int iDamage = max(0, pMonster->Get_Stat().iAtk - (m_tStat.iDef * 0.2f));

	m_tStat.fHp = max(0, m_tStat.fHp - iDamage);
}

CRubyCarriage* CRubyCarriage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CRubyCarriage* pInstance = new CRubyCarriage(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CRubyCarriage*>(pInstance);
		MSG_BOX("Failed to Created : CRubyCarriage");
	}

	return pInstance;
}

CGameObject* CRubyCarriage::Clone(void* pArg)
{
	CRubyCarriage* pInstance = new CRubyCarriage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CRubyCarriage*>(pInstance);
		MSG_BOX("Failed to Cloned : CRubyCarriage");
	}

	return pInstance;
}

void CRubyCarriage::Free()
{
	__super::Free();
}
