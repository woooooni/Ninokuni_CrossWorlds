#include "stdafx.h"
#include "Vfx_SwordMan_Skill_MegaSlash.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Effect.h"
#include "Particle.h"
#include "Utils.h"

CVfx_SwordMan_Skill_MegaSlash::CVfx_SwordMan_Skill_MegaSlash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_MegaSlash::CVfx_SwordMan_Skill_MegaSlash(const CVfx_SwordMan_Skill_MegaSlash& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_MegaSlash::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_BURST;
	m_iMaxCount = 14;

	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// Decal
	m_pFrameTriger[0]    = 0;
	m_pPositionOffset[0] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[0]    = _float3(18.f, 5.f, 15.f);
	m_pRotationOffset[0] = _float3(0.f, -90.f, 0.f);

	// Fire
	m_pFrameTriger[1]    = 3;
	m_pPositionOffset[1] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[1]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[1] = _float3(0.f, 0.f, 0.f);

	// Effect Sword
	m_pFrameTriger[2]    = 10;
	m_pPositionOffset[2] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[2]    = _float3(3.f, 3.f, 3.f);
	m_pRotationOffset[2] = _float3(0.f, 90.f, 0.f);

	// Event Sword_Dissolve
	m_pFrameTriger[3]    = 25;
	m_pPositionOffset[3] = _float3(52.f, 4.f, 10.f);
	m_pScaleOffset[3]    = _float3(1.f, 0.2f, 0.f);
	m_pRotationOffset[3] = _float3(0.f, 0.f, 0.f);

	// Effect Trail
	m_pFrameTriger[4]    = 45;
	m_pPositionOffset[4] = _float3(0.f, 0.1f, 0.f);
	m_pScaleOffset[4]    = _float3(10.f, 10.f, 10.f);
	m_pRotationOffset[4] = _float3(0.f, 20.f, 0.f);

	// ----------------------------------------------

	// Fire 1
	m_pFrameTriger[5]    = 46;
	m_pPositionOffset[5] = _float3(4.f, 0.7f, 0.f);
	m_pScaleOffset[5]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[5] = _float3(0.f, 0.f, 0.f);

	// Fire 2
	m_pFrameTriger[6]    = 46;
	m_pPositionOffset[6] = _float3(2.f, 0.7f, 2.f);
	m_pScaleOffset[6]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[6] = _float3(0.f, 0.f, 0.f);

	// Fire 3
	m_pFrameTriger[7]    = 47;
	m_pPositionOffset[7] = _float3(1.f, 0.7f, 4.f);
	m_pScaleOffset[7]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[7] = _float3(0.f, 0.f, 0.f);

	// Fire 4
	m_pFrameTriger[8]    = 47;
	m_pPositionOffset[8] = _float3(-1.f, 0.7f, 4.f);
	m_pScaleOffset[8]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[8] = _float3(0.f, 0.f, 0.f);

	// Fire 5
	m_pFrameTriger[9]    = 48;
	m_pPositionOffset[9] = _float3(-2.f, 0.7f, 2.f);
	m_pScaleOffset[9]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[9] = _float3(0.f, 0.f, 0.f);

	// Fire 6
	m_pFrameTriger[10]    = 48;
	m_pPositionOffset[10] = _float3(-4.f, 0.7f, 0.f);
	m_pScaleOffset[10]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[10] = _float3(0.f, 0.f, 0.f);

	// Sparkle 1
	m_pFrameTriger[11]    = 49;
	m_pPositionOffset[11] = _float3(0.f, 0.f, 2.f);
	m_pScaleOffset[11]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[11] = _float3(0.f, 0.f, 0.f);

	// Sparkle 2
	m_pFrameTriger[12]    = 49;
	m_pPositionOffset[12] = _float3(0.f, 0.f, 2.f);
	m_pScaleOffset[12]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[12] = _float3(0.f, 0.f, 0.f);

	// ----------------------------------------------

	// Event Sword_End
	m_pFrameTriger[13]    = 50;
	m_pPositionOffset[13] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[13]    = _float3(0.f, 0.f, 0.f);
	m_pRotationOffset[13] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_MegaSlash::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_MegaSlash::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (-1 == m_iType)
		{
			CCharacter* pPlayer = static_cast<CCharacter*>(m_pOwnerObject);
			if (nullptr == pPlayer)
				MSG_BOX("Casting_Failde");
			else
				m_iType = pPlayer->Get_ElementalType();

			switch (m_iType)
			{
			case ELEMENTAL_TYPE::FIRE:
				m_fMainColor     = _float3(0.881f, 0.263f, 0.023f);
				m_fSwordColor    = _float3(1.f, 0.359f, 0.212f);
				m_fDissolveColor = _float3(1.f, 0.241f, 0.067f);
				break;
			case ELEMENTAL_TYPE::WATER:
				m_fMainColor     = _float3(0.4f, 0.8f, 0.9f);
				m_fSwordColor    = _float3(0.f, 0.489f, 0.694f);
				m_fDissolveColor = _float3(0.043f, 0.555f, 0.839f);
				break;
			case ELEMENTAL_TYPE::WOOD:
				m_fMainColor     = _float3(0.3f, 1.f, 0.5f);
				m_fSwordColor    = _float3(0.196f, 0.611f, 0.321f);
				m_fDissolveColor = _float3(0.279f, 0.699f, 0.405f);
				break;
			}
		}

		// Decal
		if (m_iCount == 0 && m_iOwnerFrame >= m_pFrameTriger[0])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_MegaSlash_SemiCircle"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[0], m_pScaleOffset[0], m_pRotationOffset[0]);
			m_iCount++;
		}

		// Fire
		else if (m_iCount == 1 && m_iOwnerFrame >= m_pFrameTriger[1])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_MegaSlash_Trail_Fire_Big"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[1], m_pScaleOffset[1], m_pRotationOffset[1]);
			m_iCount++;
		}

		// Effect Sword
		else if (m_iCount == 2 && m_iOwnerFrame >= m_pFrameTriger[2])
		{
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerModel != nullptr)
			{
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Disappear_Weapon();

				Matrix RightHandMatrix = pOwnerModel->Get_SocketLocalMatrix(0); // 뼈 행렬 x 플레이어 월드 행렬 = 뼈 월드 행렬
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_MegaSlash_Sword"), 
					RightHandMatrix * m_WorldMatrix, m_pPositionOffset[2], m_pScaleOffset[2], m_pRotationOffset[2], nullptr, &m_pSwordEffect);
				if (nullptr != m_pSwordEffect)
				{
					Safe_AddRef(m_pSwordEffect);
					m_pSwordEffect->Set_Color(m_fSwordColor);
				}
			}
			m_iCount++;
		}

		// Event Sword_Dissolve
		else if (m_iCount == 3 && m_iOwnerFrame >= m_pFrameTriger[3])
		{
			m_pSwordEffect->Start_Dissolve((_uint)m_pPositionOffset[3].x, // Index
				_float4(m_fDissolveColor.x, m_fDissolveColor.y, m_fDissolveColor.z, 1.f), // Color
				(_uint)m_pPositionOffset[3].y,  // Speed
				(_uint)m_pPositionOffset[3].z); // Total
			m_iCount++;
		}

		// Effect Trail
		else if (m_iCount == 4 && m_iOwnerFrame >= m_pFrameTriger[4])
		{
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_MegaSlash_Trail"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[4], m_pScaleOffset[4], m_pRotationOffset[4], nullptr, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			m_iCount++;
		}

		// -------------------------------------------------------------------
		// 4 ~ 11

		else if (m_iCount == 5 && m_iOwnerFrame >= m_pFrameTriger[5])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_MegaSlash_Trail_Fire_01"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[5], m_pScaleOffset[5], m_pRotationOffset[5]);
			m_iCount++;
		}
		else if (m_iCount == 6 && m_iOwnerFrame >= m_pFrameTriger[6])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_MegaSlash_Trail_Fire_02"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[6], m_pScaleOffset[6], m_pRotationOffset[6]);
			m_iCount++;
		}
		else if (m_iCount == 7 && m_iOwnerFrame >= m_pFrameTriger[7])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_MegaSlash_Trail_Fire_01"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[7], m_pScaleOffset[7], m_pRotationOffset[7]);
			m_iCount++;
		}
		else if (m_iCount == 8 && m_iOwnerFrame >= m_pFrameTriger[8])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_MegaSlash_Trail_Fire_02"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[8], m_pScaleOffset[8], m_pRotationOffset[8]);
			m_iCount++;
		}
		else if (m_iCount == 9 && m_iOwnerFrame >= m_pFrameTriger[9])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_MegaSlash_Trail_Fire_01"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[9], m_pScaleOffset[9], m_pRotationOffset[9]);
			m_iCount++;
		}
		else if (m_iCount == 10 && m_iOwnerFrame >= m_pFrameTriger[10])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_MegaSlash_Trail_Fire_02"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[10], m_pScaleOffset[10], m_pRotationOffset[10]);
			m_iCount++;
		}
		// -------------------------------------------------------------------

		else if (m_iCount == 11 && m_iOwnerFrame >= m_pFrameTriger[11])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_MegaSlash_Trail_Sparkle_Circle_01"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[11], m_pScaleOffset[11], m_pRotationOffset[11], nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fMainColor);
			}
			m_iCount++;
		}
		else if (m_iCount == 12 && m_iOwnerFrame >= m_pFrameTriger[12])
		{
		    CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_MegaSlash_Trail_Sparkle_Circle_02"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[12], m_pScaleOffset[12], m_pRotationOffset[12], nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fMainColor);
			}
			m_iCount++;
		}

		// -------------------------------------------------------------------

		// Event Sword_End
		else if (m_iCount == 13 && m_iOwnerFrame >= m_pFrameTriger[13])
		{
			Safe_Release(m_pSwordEffect);
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Appear_Weapon();
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 14)
			m_bFinish = true;
	}

	// 무기 위치 업데이트
	if (m_pSwordEffect != nullptr)
	{
		CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
		CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
		if (pOwnerTransform != nullptr && pOwnerModel != nullptr)
		{
			CTransform* pEffectTransform = m_pSwordEffect->Get_Component<CTransform>(L"Com_Transform");
			if (pEffectTransform != nullptr)
			{
				Matrix RightHandMatrix = pOwnerModel->Get_SocketLocalMatrix(0);
				Matrix OwnerWorldMatrix = pOwnerTransform->Get_WorldMatrix();

				// Scale / Rotation
				Matrix matScale = matScale.CreateScale(m_pScaleOffset[2]);
				Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[2].x), XMConvertToRadians(m_pRotationOffset[2].y), XMConvertToRadians(m_pRotationOffset[2].z)));
				Matrix matResult = matScale * matRotation * (RightHandMatrix * OwnerWorldMatrix);
				pEffectTransform->Set_WorldMatrix(matResult);

				// Position
				_vector vCurrentPosition = pEffectTransform->Get_Position();
				_vector vFinalPosition = vCurrentPosition;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[2].x;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP)    * m_pPositionOffset[2].y;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK)  * m_pPositionOffset[2].z;
				pEffectTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));
			}
		}
	}
}

void CVfx_SwordMan_Skill_MegaSlash::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_MegaSlash::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_MegaSlash::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_MegaSlash* CVfx_SwordMan_Skill_MegaSlash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_MegaSlash* pInstance = new CVfx_SwordMan_Skill_MegaSlash(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_MegaSlash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_MegaSlash::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_MegaSlash* pInstance = new CVfx_SwordMan_Skill_MegaSlash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_MegaSlash");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_MegaSlash::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}

	if(m_pSwordEffect != nullptr)
		Safe_Release(m_pSwordEffect);
}