#include "stdafx.h"
#include "Vfx_Engineer_Skill_Destruction_Cannon.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Character_Projectile.h"
#include "Character_Manager.h"
#include "Particle.h"
#include "Decal.h"
#include "Effect.h"
#include "Utils.h"
#include "Game_Manager.h"
#include "Player.h"

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

	// 등장
	{
		m_pFrameTriger[TYPE_ET0_E_METAL] = 5;
		m_pPositionOffset[TYPE_ET0_E_METAL] = _float3(0.f, 0.f, 1.f);
		m_pScaleOffset[TYPE_ET0_E_METAL]    = _float3(2.f, 2.f, 2.f);
		m_pRotationOffset[TYPE_ET0_E_METAL] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET0_P_SMOKE] = 5;
		m_pPositionOffset[TYPE_ET0_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET0_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET0_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

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
		m_pFrameTriger[TYPE_ET3_E_METAL] = 95;
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
		if (-1 == m_iType)
		{
			CCharacter* pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
			if (nullptr == pPlayer)
				MSG_BOX("Casting_Failde");
			else
				m_iType = pPlayer->Get_ElementalType();

			switch (m_iType)
			{
			case ELEMENTAL_TYPE::FIRE:
				m_fMainColor = _float3(1.000f, 0.670f, 0.446f);
				break;
			case ELEMENTAL_TYPE::WATER:
				m_fMainColor = _float3(0.544f, 0.964f, 0.879f);
				break;
			case ELEMENTAL_TYPE::WOOD:
				m_fMainColor = _float3(0.741f, 0.938f, 0.469f);
				break;
			}
		}

		// 대포 등장
		if (m_iCount == TYPE_ET0_E_METAL && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET0_E_METAL])
		{
			Create_Metal();
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET0_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET0_P_SMOKE])
		{
			m_iCount++;
		}

		// 대포 발사 준비
		else if (m_iCount == TYPE_ET1_P_LIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_LIGHT])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_Destruction_Cannon_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_LIGHT], m_pScaleOffset[TYPE_ET1_P_LIGHT], m_pRotationOffset[TYPE_ET1_P_LIGHT], nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fMainColor);
			}
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
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_Destruction_Cannon_CircleLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_CIRCLELINES], m_pScaleOffset[TYPE_ET2_E_CIRCLELINES], m_pRotationOffset[TYPE_ET2_E_CIRCLELINES], nullptr, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.1f), CUtils::Random_Float(0.f, 0.1f));
			}
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
			Create_Metal();
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

void CVfx_Engineer_Skill_Destruction_Cannon::Create_Metal()
{
	_uint iCreateCount = CUtils::Random_Int(15, 25);

	CEffect* pEffect = nullptr;
	_float3 fColor    = _float3(0.f, 0.f, 0.f);
	_float3 fRotation = _float3(0.f, 0.f, 0.f);

	for (size_t i = 0; i < iCreateCount; ++i)
	{
		// 종류
		_uint iRandomEffectCount = CUtils::Random_Int(0, 3);
		switch (iRandomEffectCount)
		{
		case 0:
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Bolt"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET0_E_METAL], m_pScaleOffset[TYPE_ET0_E_METAL], m_pRotationOffset[TYPE_ET0_E_METAL], nullptr, &pEffect);
			break;

		case 1:
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Nut"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET0_E_METAL], m_pScaleOffset[TYPE_ET0_E_METAL], m_pRotationOffset[TYPE_ET0_E_METAL], nullptr, &pEffect);
			break;

		case 2:
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Spring"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET0_E_METAL], m_pScaleOffset[TYPE_ET0_E_METAL], m_pRotationOffset[TYPE_ET0_E_METAL], nullptr, &pEffect);
			break;

		case 3:
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Gear"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET0_E_METAL], m_pScaleOffset[TYPE_ET0_E_METAL], m_pRotationOffset[TYPE_ET0_E_METAL], nullptr, &pEffect);
			break;
		default:
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Nut"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET0_E_METAL], m_pScaleOffset[TYPE_ET0_E_METAL], m_pRotationOffset[TYPE_ET0_E_METAL], nullptr, &pEffect);
			break;
		}
		pEffect->Start_RigidbodyJump(Vec3(CUtils::Random_Float(-5.f, 5.f), CUtils::Random_Float(1.f, 15.f), CUtils::Random_Float(-5.f, 5.f)), CUtils::Random_Float(2.5f, 5.f), true);

		// 색상
		_uint iRandomColorCount = CUtils::Random_Int(0, 14);
		if (5 > iRandomColorCount) // 5
		{
			fColor = _float3(0.772f, 0.772f, 0.772f); // 밝은회색
		}
		else if (9 > iRandomColorCount) // 4
		{
			fColor = _float3(0.492f, 0.492f, 0.492f); // 진한회색
		}
		else if (12 > iRandomColorCount) // 3
		{
			fColor = _float3(0.872f, 0.872f, 0.872f); // 하얀회색
		}
		else if (14 > iRandomColorCount) // 2
		{
			fColor = _float3(0.984f, 0.794f, 0.383f); // 밝은골드
		}
		else if (15 > iRandomColorCount) // 1
		{
			fColor = _float3(0.902f, 0.685f, 0.215f); // 진한골드
		}
		pEffect->Set_Color(fColor);

		CTransform* pTransform = pEffect->Get_Component<CTransform>(L"Com_Transform");
		if (nullptr != pTransform)
		{
			// 회전 
			_uint iRandomAxisCount = CUtils::Random_Int(0, 2);
			switch (iRandomAxisCount)
			{
			case 0:
				fRotation = _float3(CUtils::Random_Float(-360.f, 360.f), 0.f, 0.f);
				break;

			case 1:
				fRotation = _float3(0.f, CUtils::Random_Float(-360.f, 360.f), 0.f);
				break;

			case 2:
				fRotation = _float3(0.f, 0.f, CUtils::Random_Float(-360.f, 360.f));
				break;
			}
			pTransform->FixRotation(fRotation.x, fRotation.y, fRotation.z);

			// 스케일
			_float fScale = CUtils::Random_Float(1.5f, 3.f);
			pTransform->Set_Scale(_float3(fScale, fScale, fScale));
		}

		// 블룸
		pEffect->Set_BloomPower(_float3(CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f), CUtils::Random_Float(0.f, 1.f)));
	
		// 유지시간
		pEffect->Set_LifeTime(3.f);
	}
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