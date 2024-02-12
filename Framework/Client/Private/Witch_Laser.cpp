#include "stdafx.h"
#include "Witch_Laser.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Stellia.h"

#include "Camera_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"

#include "Vfx.h"
#include "Effect.h"

CWitch_Laser::CWitch_Laser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CWitch_Laser::CWitch_Laser(const CWitch_Laser& rhs)
	: CMonsterProjectile(rhs)
{

}

HRESULT CWitch_Laser::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CWitch_Laser::Initialize(void* pArg)
{
	//if (pArg == nullptr)
	//	return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false); 

	m_fActiveTime = .5f;

	return S_OK;
}

void CWitch_Laser::Tick(_float fTimeDelta)
{
	// 레이저 경고 생성
	if (!m_bIsLaserWarningCreate && m_pLaserWarning == nullptr)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Witch_Skill_Laser_Warning"), m_pTransformCom->Get_WorldMatrix(), nullptr, &m_pLaserWarning);

		m_bIsLaserWarningCreate = true;
	}
	else // 다시 살리기
	{
		if (!m_bIsLaserCreate && m_pLaserWarning->Is_Dead() && m_pLaser == nullptr)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);

			// 공격 레이저 생성
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Rage3_LaserLine"), m_pTransformCom->Get_WorldMatrix(), 
				_float3(0.f, 0.045, -0.055f), _float3(40.f, 50.f, 875.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pLaser);
			if (nullptr != m_pLaser)
			{
				m_pLaser->Reserve_Dissolve(73,  // Index
					_float4(1.f, 1.f, 1.f, 1.f),// Color
					5.f,   // Speed
					10.f); // Total
			}

			m_bIsLaserCreate = true;
		}
	}

	// 레이저 이펙트
	if (m_bIsLaserCreate)
	{
		if (m_pLaser != nullptr)
		{
			m_fAccTime += fTimeDelta;
			if (m_fAccTime > m_fActiveTime)
			{
				Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
				Set_Dead(true);
			}

			//if (m_pLaser->Get_IsDissolve() || m_pLaser->Get_DieEffect() || m_pLaser->Is_ReserveDead() || m_pLaser->Is_Dead())
			//{
			//	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
			//	Set_Dead(true);
			//}
		}
	}

	__super::Tick(fTimeDelta);
}

void CWitch_Laser::LateTick(_float fTimeDelta)
{
	LateUpdate_Collider(fTimeDelta);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

void CWitch_Laser::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
	}
}

void CWitch_Laser::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CWitch_Laser::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CWitch_Laser::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CWitch_Laser::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 1.35f, 1.5f, 32.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = {};
	OBBDesc.vOffsetPosition = Vec3(0.f, 1.5f, 0.f);

	/* Attack */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion

CWitch_Laser* CWitch_Laser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CWitch_Laser* pInstance = new CWitch_Laser(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CWitch_Laser");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CWitch_Laser::Clone(void* pArg)
{
	CWitch_Laser* pInstance = new CWitch_Laser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWitch_Laser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWitch_Laser::Free()
{
	__super::Free();
}

