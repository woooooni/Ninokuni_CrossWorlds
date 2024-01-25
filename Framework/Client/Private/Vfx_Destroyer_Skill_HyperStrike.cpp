#include "stdafx.h"
#include "Vfx_Destroyer_Skill_HyperStrike.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Character_Projectile.h"
#include "Character_Manager.h"
#include "Destroyer_HyperStrike_Hammer.h"
#include "Decal.h"
#include "Effect.h"

CVfx_Destroyer_Skill_HyperStrike::CVfx_Destroyer_Skill_HyperStrike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_HyperStrike::CVfx_Destroyer_Skill_HyperStrike(const CVfx_Destroyer_Skill_HyperStrike& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_BURST;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE] = 33;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 2.25f, -0.15f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE]    = _float3(1.5f, 1.5f, 1.5f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 33;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 3.25f, -0.15f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_D_CRACK] = 38;
		m_pPositionOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.55f);
		m_pScaleOffset[TYPE_ET2_D_CRACK]    = _float3(2.f, 3.f, 2.f);
		m_pRotationOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_FIRES] = 38;
		m_pPositionOffset[TYPE_ET2_P_FIRES] = _float3(0.f, 0.f, 1.3f);
		m_pScaleOffset[TYPE_ET2_P_FIRES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_FIRES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 38;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.5f, 1.3f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_D_CIRCLE] = 45;
		m_pPositionOffset[TYPE_ET3_D_CIRCLE] = _float3(0.f, 0.f, 0.56f);
		m_pScaleOffset[TYPE_ET3_D_CIRCLE]    = _float3(17.f, 3.f, 17.f);
		m_pRotationOffset[TYPE_ET3_D_CIRCLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_O_HAMMER] = 50;
		m_pPositionOffset[TYPE_ET3_O_HAMMER] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_O_HAMMER]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_O_HAMMER] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pPositionOffset[TYPE_ET3_D_CRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_D_CRACK]    = _float3(15.f, 1.f, 15.f);
		m_pRotationOffset[TYPE_ET3_D_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pPositionOffset[TYPE_ET3_E_STONECRACK] = _float3(-1.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_STONECRACK]    = _float3(0.5f, 0.4f, 0.5f);
		m_pRotationOffset[TYPE_ET3_E_STONECRACK] = _float3(0.f, 0.f, 0.f);

		m_pPositionOffset[TYPE_ET3_P_FIRES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_FIRES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_FIRES] = _float3(0.f, 0.f, 0.f);

		m_pPositionOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pPositionOffset[TYPE_ET3_P_STONE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_STONE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_STONE] = _float3(0.f, 0.f, 0.f);

		m_pPositionOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.4f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_CIRCLELINE]    = _float3(2.f, 2.f, 2.f);
		m_pRotationOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_HyperStrike::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_HyperStrike_CirecleLine_HandSmall"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_CIRCLELINE], m_pScaleOffset[TYPE_ET1_E_CIRCLELINE], m_pRotationOffset[TYPE_ET1_E_CIRCLELINE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_HyperStrike_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_CIRCLES], m_pScaleOffset[TYPE_ET1_P_CIRCLES], m_pRotationOffset[TYPE_ET1_P_CIRCLES]);
			m_iCount++;
		}
		
		else if (m_iCount == TYPE_ET2_D_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_CRACK])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Destroyer_Skill_HyperStrike_Crack_Small"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_D_CRACK], m_pScaleOffset[TYPE_ET2_D_CRACK], m_pRotationOffset[TYPE_ET2_D_CRACK]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_FIRES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_HyperStrike_Fire_Small"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_FIRES], m_pScaleOffset[TYPE_ET2_P_FIRES], m_pRotationOffset[TYPE_ET2_P_FIRES]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_HyperStrike_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], m_pRotationOffset[TYPE_ET2_P_CIRCLES]);
			m_iCount++;
		}

		// -----------------------------------------------------------------------------------------------------
		else if (m_iCount == TYPE_ET3_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_D_CIRCLE], m_pScaleOffset[TYPE_ET3_D_CIRCLE], m_pRotationOffset[TYPE_ET3_D_CIRCLE], nullptr, &m_pEt3_Decal, false);
			Safe_AddRef(m_pEt3_Decal);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_O_HAMMER && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_O_HAMMER])
		{
			Shoot_Hammer();
			m_iCount++;
		}

		else if (m_iCount > TYPE_ET3_O_HAMMER)
		{
			if (nullptr != m_pHammer && true == m_pHammer->Get_IsGround())
			{
				if (nullptr != m_pEt3_Decal)
				{
					m_pEt3_Decal->Set_Dead(true);
					Safe_Release(m_pEt3_Decal);
				}

				CTransform* pTransform = m_pHammer->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr != pTransform)
					m_WorldMatrix = pTransform->Get_WorldFloat4x4();

				// IsGround
				GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Destroyer_Skill_HyperStrike_Crack"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_D_CRACK], m_pScaleOffset[TYPE_ET3_D_CRACK], m_pRotationOffset[TYPE_ET3_D_CRACK]);

				CEffect* pEffect = nullptr;
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_HyperStrike_SpringUp"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_E_STONECRACK], m_pScaleOffset[TYPE_ET3_E_STONECRACK], m_pRotationOffset[TYPE_ET3_E_STONECRACK], m_pHammer, &pEffect);
				if (nullptr != pEffect)
				{
					pEffect->Reserve_Dissolve(73,            // Index
						_float4(0.756f, 0.532f, 0.38f, 1.f), // Color
						3.f,   // Speed
						10.f); // Total
				}

				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_HyperStrike_Fire"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_FIRES], m_pScaleOffset[TYPE_ET3_P_FIRES], m_pRotationOffset[TYPE_ET3_P_FIRES]);

				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_BattleCry_Circles"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_CIRCLES], m_pScaleOffset[TYPE_ET3_P_CIRCLES], m_pRotationOffset[TYPE_ET3_P_CIRCLES]);

				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_HyperStrike_Stone"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_STONE], m_pScaleOffset[TYPE_ET3_P_STONE], m_pRotationOffset[TYPE_ET3_P_STONE]);

				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_HyperStrike_CirecleLine"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_E_CIRCLELINE], m_pScaleOffset[TYPE_ET3_E_CIRCLELINE], m_pRotationOffset[TYPE_ET3_E_CIRCLELINE]);

				Safe_Release(m_pHammer);

				m_iCount++;
			}
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Destroyer_Skill_HyperStrike::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Ready_Components()
{
	return S_OK;
}

void CVfx_Destroyer_Skill_HyperStrike::Shoot_Hammer()
{
	if(nullptr == m_pOwnerObject)
		return;

	CCharacter* pPlayer = static_cast<CCharacter*>(m_pOwnerObject);
	if(nullptr == pPlayer)
		return;

	CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc = {};
	ProjectileDesc.pOwner = pPlayer;

	CGameObject* pHammer = nullptr;
	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, L"Prototype_GameObject_Destroyer_Burst_HyperStrikeHammer", &ProjectileDesc, &pHammer)))
		MSG_BOX("Add GameObject Failed. : CState_Destroyer_BurstSkill_HyperStrike::Shoot_Hammer()");

	if(nullptr == pHammer)
		return;

	CTransform* pTransform = pHammer->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
		return;

	_matrix InitMatirx = XMLoadFloat4x4(&m_WorldMatrix);
	InitMatirx.r[CTransform::STATE_POSITION] += XMVector3Normalize(InitMatirx.r[CTransform::STATE_LOOK]) * 10.f;
	InitMatirx.r[CTransform::STATE_POSITION] += XMVectorSet(0.f, 15.f, 0.f, 0.f);

	pTransform->Set_WorldMatrix(InitMatirx);

	CPhysX_Controller* pController = pHammer->Get_Component<CPhysX_Controller>(L"Com_Controller");
	if (nullptr == pController)
		return;

	pController->Set_EnterLevel_Position(pTransform->Get_Position());

	m_pHammer = static_cast<CDestroyer_HyperStrike_Hammer*>(pHammer);
	Safe_AddRef(m_pHammer);
}

CVfx_Destroyer_Skill_HyperStrike* CVfx_Destroyer_Skill_HyperStrike::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_HyperStrike* pInstance = new CVfx_Destroyer_Skill_HyperStrike(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_HyperStrike");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_HyperStrike::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_HyperStrike* pInstance = new CVfx_Destroyer_Skill_HyperStrike(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_HyperStrike");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_HyperStrike::Free()
{
	__super::Free();

	if (nullptr != m_pEt3_Decal)
	{
		m_pEt3_Decal->Set_Dead(true);
		Safe_Release(m_pEt3_Decal);
	}

	if(nullptr != m_pHammer)
		Safe_Release(m_pHammer);

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}