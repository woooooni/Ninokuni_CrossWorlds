#include "stdafx.h"
#include "Vfx_Engineer_Skill_Destruction_Cannon.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Character_Projectile.h"
#include "Character_Manager.h"
#include "Particle.h"

CVfx_Engineer_Skill_Destruction_Cannon::CVfx_Engineer_Skill_Destruction_Cannon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_Destruction_Cannon::CVfx_Engineer_Skill_Destruction_Cannon(const CVfx_Engineer_Skill_Destruction_Cannon& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_Destruction_Cannon::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 대포 발사 준비
	{
		m_pFrameTriger[TYPE_ET1_P_LIGHT] = 20;
		m_pPositionOffset[TYPE_ET1_P_LIGHT] = _float3(0.f, 1.5f, 3.2f);
		m_pScaleOffset[TYPE_ET1_P_LIGHT]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_LIGHT] = _float3(0.f, 0.f, 0.f);
	}

	// 대포 발사
	{
		m_pFrameTriger[TYPE_ET2_O_BULLET] = 44;
		m_pPositionOffset[TYPE_ET2_O_BULLET] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_O_BULLET] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_O_BULLET] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_CIRCLELINES] = 44;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINES] = _float3(0.f, 45.f, -15.f); //x,z,-y
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINES] = _float3(0.1f, 0.1f, 0.1f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINES] = _float3(90.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_FIRE] = 44;
		m_pPositionOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 1.5f, 3.2f);
		m_pScaleOffset[TYPE_ET2_P_FIRE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_TWINKLE] = 44;
		m_pPositionOffset[TYPE_ET2_P_TWINKLE] = _float3(0.f, 1.5f, 3.2f);
		m_pScaleOffset[TYPE_ET2_P_TWINKLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_TWINKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_SMOKE] = 44;
		m_pPositionOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 1.5f, 3.2f);
		m_pScaleOffset[TYPE_ET2_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

	// 대포 회수
	{
		m_pFrameTriger[TYPE_ET3_E_METAL] = 50;
		m_pPositionOffset[TYPE_ET3_E_METAL] = _float3(1.8f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_METAL]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_E_METAL] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_Destruction_Cannon::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_Destruction_Cannon::Tick(_float fTimeDelta)
{
	if (m_bFinish || m_pOwnerObject == nullptr || m_pOwnerObject->Is_ReserveDead() || m_pOwnerObject->Is_Dead())
	{
		Set_Dead(true);
		return;
	}

	CModel* pModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
	if (pModel == nullptr)
	{
		Set_Dead(true);
		return;
	}

	if (m_bOwnerTween) // false == TweenFinish
		m_bOwnerTween = pModel->Is_Tween();
	else
		m_iOwnerFrame = pModel->Get_CurrAnimationFrame();


	if (!m_bOwnerTween)
	{
		// 대포 발사 준비
		if (m_iCount == TYPE_ET1_P_LIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_LIGHT])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_Destruction_Cannon_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_LIGHT], m_pScaleOffset[TYPE_ET1_P_LIGHT], m_pRotationOffset[TYPE_ET1_P_LIGHT]);
			m_iCount++;
		}

		// 대포 발사
		else if (m_iCount == TYPE_ET2_O_BULLET && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_O_BULLET])
		{
			Fire_Cannon();
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_CIRCLELINES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINES])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_Destruction_Cannon_CircleLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_CIRCLELINES], m_pScaleOffset[TYPE_ET2_E_CIRCLELINES], m_pRotationOffset[TYPE_ET2_E_CIRCLELINES]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_FIRE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_ExplosionShot_Fire"),
            	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_FIRE], m_pScaleOffset[TYPE_ET2_P_FIRE], m_pRotationOffset[TYPE_ET2_P_FIRE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_TWINKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_TWINKLE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_BurstCall_Circles"),
            	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_TWINKLE], m_pScaleOffset[TYPE_ET2_P_TWINKLE], m_pRotationOffset[TYPE_ET2_P_TWINKLE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SMOKE])
		{
			m_iCount++;
		}

		// 대포 회수
		else if (m_iCount == TYPE_ET3_E_METAL && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_METAL])
		{

			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Engineer_Skill_Destruction_Cannon::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_Destruction_Cannon::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_Destruction_Cannon::Ready_Components()
{
	return S_OK;
}

void CVfx_Engineer_Skill_Destruction_Cannon::Fire_Cannon()
{
	CGameObject* pGameObject = nullptr;

	CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc = {};
	ProjectileDesc.pOwner = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::ENGINEER);

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, L"Prototype_GameObject_Engineer_Burst_CannonBomb", &ProjectileDesc, &pGameObject)))
	{
		MSG_BOX("Add GameObject Failed : CEngineer_Burst_DestructionCannon::Fire_Cannon");
		return;
	}

	CTransform* pTransform = pGameObject->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
	{
		MSG_BOX("Find Transform Failed : CEngineer_Burst_DestructionCannon::Fire_Cannon");
		return;
	}

	_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
	WorldMatrix.r[CTransform::STATE_POSITION] += (XMVector3Normalize(WorldMatrix.r[CTransform::STATE_LOOK]) * 3.f);
	WorldMatrix.r[CTransform::STATE_POSITION] += XMVectorSet(0.f, 1.5f, 0.f, 0.f);
	pTransform->Set_WorldMatrix(WorldMatrix);
}

CVfx_Engineer_Skill_Destruction_Cannon* CVfx_Engineer_Skill_Destruction_Cannon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_Destruction_Cannon* pInstance = new CVfx_Engineer_Skill_Destruction_Cannon(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_Destruction_Cannon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_Destruction_Cannon::Clone(void* pArg)
{
	CVfx_Engineer_Skill_Destruction_Cannon* pInstance = new CVfx_Engineer_Skill_Destruction_Cannon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_Destruction_Cannon");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_Destruction_Cannon::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}