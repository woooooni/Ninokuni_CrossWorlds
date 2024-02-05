#include "stdafx.h"
#include "GameInstance.h"
#include "Enemy_GuidedMissile.h"

#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Vehicle_Flying.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"

#include "Game_Manager.h"
#include "Player.h"

CEnemy_GuidedMissile::CEnemy_GuidedMissile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVehicleFlying_Projectile(pDevice, pContext, L"Enemy_GuidedMissile", OBJ_TYPE::OBJ_GRANDPRIX_ENEMY_PROJECTILE)
{

}

CEnemy_GuidedMissile::CEnemy_GuidedMissile(const CEnemy_GuidedMissile& rhs)
	: CVehicleFlying_Projectile(rhs)
{
}


HRESULT CEnemy_GuidedMissile::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEnemy_GuidedMissile::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fMoveSpeed = 10.f;
	m_pTransformCom->Set_Scale(Vec3(0.15f, 0.15f, 0.15f));
	Set_Collider_Elemental(dynamic_cast<CCharacter*>(m_pOwner->Get_Rider())->Get_ElementalType());
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

	m_fDeletionTime = 5.f;

	return S_OK;
}

void CEnemy_GuidedMissile::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle(&m_fAccEffect, CUtils::Random_Float(0.1f, 0.1f), fTimeDelta, TEXT("Particle_Smoke"), this);

	Tick_Target(fTimeDelta);

	if (nullptr == m_pTarget)
	{
		Find_Target(fTimeDelta);
		if (nullptr == m_pTarget)
		{
			m_bCameraTarget = false;
			CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
			CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());

			Set_Dead(true);
			return;
		}
	}

	if (m_fAccDeletionTime >= 0.5f)
	{
		if (nullptr != m_pTarget)
		{
			CTransform* pTargetTransform = m_pTarget->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr != pTargetTransform)
			{
				Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
				if (vDir.Length() > 0.001f)
				{
					Vec3 vLook = XMVector3Normalize(m_pTransformCom->Get_Look());

					Vec3 vAxis = XMVector3Cross(vLook, vDir);
					vDir = XMVector3Normalize(pTargetTransform->Get_Position() - m_pTransformCom->Get_Position());

					m_pTransformCom->Rotation_Acc(vAxis, XMConvertToRadians(180.f) * fTimeDelta);
				}
			}
		}
	}
	
	m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);
}

void CEnemy_GuidedMissile::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CEnemy_GuidedMissile::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	__super::Render();

	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}





HRESULT CEnemy_GuidedMissile::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Enemy_GuidedMissile"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 0.2f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CEnemy_GuidedMissile::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_GRANDPRIX_ENEMY) && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		wstring strSoundKey = L"Hit_PC_Damage_Dummy_" + to_wstring(GI->RandomInt(1, 2)) + L".mp3";
		GI->Play_Sound(strSoundKey, SOUND_MONSTERL_HIT, 0.3f, false);
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default();

		m_bCameraTarget = false;
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());


		Set_Dead(true);
	}
}

void CEnemy_GuidedMissile::Tick_Target(_float fTimeDelta)
{
	if (nullptr == m_pTarget)
		return;


	if (m_pTarget->Is_ReserveDead() || m_pTarget->Is_Dead())
		m_pTarget = nullptr;
}

void CEnemy_GuidedMissile::Find_Target(_float fTimeDelta)
{
	list<CGameObject*>& TargetObjects = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER);

	_float fMinDistance = 50.f;
	for (auto& pTarget : TargetObjects)
	{
		if (OBJ_TYPE::OBJ_GRANDPRIX_ENEMY != pTarget->Get_ObjectType())
			continue;

		CTransform* pTargetTransform = pTarget->Get_Component<CTransform>(L"Com_Transform");
		Vec4 vPosition = m_pTransformCom->Get_Position();
		Vec4 vTargetPosition = pTargetTransform->Get_Position();

		Vec3 vDir = vTargetPosition - vPosition;
		if (fMinDistance > vDir.Length())
		{
			fMinDistance = vDir.Length();
			m_pTarget = pTarget;
		}
	}
}


CEnemy_GuidedMissile* CEnemy_GuidedMissile::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEnemy_GuidedMissile* pInstance = new CEnemy_GuidedMissile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CEnemy_GuidedMissile");
		Safe_Release<CEnemy_GuidedMissile*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CEnemy_GuidedMissile::Clone(void* pArg)
{
	CEnemy_GuidedMissile* pInstance = new CEnemy_GuidedMissile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CEnemy_GuidedMissile");
		Safe_Release<CEnemy_GuidedMissile*>(pInstance);
	}

	return pInstance;
}

void CEnemy_GuidedMissile::Free()
{
	__super::Free();
}
