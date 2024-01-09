#include "stdafx.h"
#include "Vfx_SwordMan_Skill_MegaSlash.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Effect.h"

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
	m_iMaxCount = 3;

	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// Effect Sword
	m_pFrameTriger[0]    = 0;
	m_pPositionOffset[0] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[0]    = _float3(3.f, 3.f, 3.f);
	m_pRotationOffset[0] = _float3(0.f, 90.f, 0.f);

	// Event Sword_Dissolve
	m_pFrameTriger[1]    = 15;
	m_pPositionOffset[1] = _float3(51.f, 3.f, 10.f);
	m_pScaleOffset[1]    = _float3(1.f, 0.2f, 0.f);
	m_pRotationOffset[1] = _float3(0.f, 0.f, 0.f);

	// Event Sword_End
	m_pFrameTriger[2]    = 50;
	m_pPositionOffset[2] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[2]    = _float3(0.f, 0.f, 0.f);
	m_pRotationOffset[2] = _float3(0.f, 0.f, 0.f);

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
		// Effect Sword
		if (m_iCount == 0 && m_iOwnerFrame >= m_pFrameTriger[0])
		{
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerModel != nullptr)
			{
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Disappear_Weapon();

				Matrix RightHandMatrix = pOwnerModel->Get_SocketLocalMatrix(0); // 뼈 행렬 x 플레이어 월드 행렬 = 뼈 월드 행렬
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_MegaSlash_Sword"), 
					RightHandMatrix * m_WorldMatrix, m_pPositionOffset[0], m_pScaleOffset[0], m_pRotationOffset[0], nullptr, &m_pSwordEffect);
				Safe_AddRef(m_pSwordEffect);
			}
			m_iCount++;
		}

		// Event Sword_Dissolve
		else if (m_iCount == 1 && m_iOwnerFrame >= m_pFrameTriger[1])
		{
			m_pSwordEffect->Start_Dissolve((_uint)m_pPositionOffset[1].x, // Index
				_float4(m_pScaleOffset[1].x, m_pScaleOffset[1].y, m_pScaleOffset[1].z, 1.f), // Color
				(_uint)m_pPositionOffset[1].y,  // Speed
				(_uint)m_pPositionOffset[1].z); // Total
			m_iCount++;
		}

		// Event Sword_End
		else if (m_iCount == 2 && m_iOwnerFrame >= m_pFrameTriger[2])
		{
			Safe_Release(m_pSwordEffect);
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Appear_Weapon();
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 3)
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
				Matrix matScale = matScale.CreateScale(m_pScaleOffset[0]);
				Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[0].x), XMConvertToRadians(m_pRotationOffset[0].y), XMConvertToRadians(m_pRotationOffset[0].z)));
				Matrix matResult = matScale * matRotation * (RightHandMatrix * OwnerWorldMatrix);
				pEffectTransform->Set_WorldMatrix(matResult);

				// Position
				_vector vCurrentPosition = pEffectTransform->Get_Position();
				_vector vFinalPosition = vCurrentPosition;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[0].x;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP)    * m_pPositionOffset[0].y;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK)  * m_pPositionOffset[0].z;
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