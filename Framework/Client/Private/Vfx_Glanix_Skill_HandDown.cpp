#include "stdafx.h"
#include "Vfx_Glanix_Skill_HandDown.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"
#include "Effect.h"
#include "Decal.h"

#include "Game_Manager.h"
#include "Player.h"

CVfx_Glanix_Skill_HandDown::CVfx_Glanix_Skill_HandDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_HandDown::CVfx_Glanix_Skill_HandDown(const CVfx_Glanix_Skill_HandDown& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_HandDown::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_ICEWAVE;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_D_START_WARNING]    = 0;
		m_pPositionOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.5f);
		m_pScaleOffset[TYPE_D_START_WARNING]    = _float3(5.f, 2.f, 14.3f);
		m_pRotationOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_P_PREACT_BIGSMOKE]    = 16;
		m_pPositionOffset[TYPE_P_PREACT_BIGSMOKE] = _float3(0.f, 4.f, 0.5f);
		m_pScaleOffset[TYPE_P_PREACT_BIGSMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_PREACT_BIGSMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_PREACT_CIRCLELINE]    = 17;
		m_pPositionOffset[TYPE_E_PREACT_CIRCLELINE] = _float3(0.f, 25.f, 0.f);
		m_pScaleOffset[TYPE_E_PREACT_CIRCLELINE]    = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_E_PREACT_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_PREACT_CIRCLES]    = 18;
		m_pPositionOffset[TYPE_P_PREACT_CIRCLES] = _float3(0.f, 3.5f, 1.f);
		m_pScaleOffset[TYPE_P_PREACT_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_PREACT_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_PREACT_HANDCIRCLELEFT]    = 25;
		m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLELEFT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_PREACT_HANDCIRCLELEFT]    = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLELEFT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_PREACT_HANDCIRCLERIGHT]    = 25;
		m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLERIGHT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_PREACT_HANDCIRCLERIGHT]    = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLERIGHT] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_D_ATTACK_CRACK]    = 48;
		m_pPositionOffset[TYPE_D_ATTACK_CRACK] = _float3(0.f, 0.f, 0.5f);
		m_pScaleOffset[TYPE_D_ATTACK_CRACK]    = _float3(7.f, 5.f, 7.f);
		m_pRotationOffset[TYPE_D_ATTACK_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_ATTACK_SPRINGUP]    = 50;
		m_pPositionOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(0.f, 0.f, -5.f);
		m_pScaleOffset[TYPE_E_ATTACK_SPRINGUP]    = _float3(1.2f, 1.5f, 1.3f);
		m_pRotationOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(0.f, 180.f, 0.f);

		m_pFrameTriger[TTPE_P_ATTACK_SMOKE]    = 51;
		m_pPositionOffset[TTPE_P_ATTACK_SMOKE] = _float3(0.f, 0.5f, 7.f);
		m_pScaleOffset[TTPE_P_ATTACK_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TTPE_P_ATTACK_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_ATTACK_CIRCLES]    = 53;
		m_pPositionOffset[TYPE_P_ATTACK_CIRCLES] = _float3(0.f, 0.f, 7.f);
		m_pScaleOffset[TYPE_P_ATTACK_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_ATTACK_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	return S_OK;
}

HRESULT CVfx_Glanix_Skill_HandDown::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_HandDown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_START_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_START_WARNING])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_FourHandSwing_Warning"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_START_WARNING], m_pScaleOffset[TYPE_D_START_WARNING], m_pRotationOffset[TYPE_D_START_WARNING], nullptr, &m_pWarningDecal, false);
			if (nullptr != m_pWarningDecal)
			{
				Safe_AddRef(m_pWarningDecal);
				m_pWarningDecal->Set_UVFlow(true, true, 0, _float2(0.f, -1.f), 1.f);
			}
			m_iCount++;
		}


		else if (m_iCount == TYPE_P_PREACT_BIGSMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_PREACT_BIGSMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_HandDown_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_PREACT_BIGSMOKE], m_pScaleOffset[TYPE_P_PREACT_BIGSMOKE], m_pRotationOffset[TYPE_P_PREACT_BIGSMOKE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_PREACT_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_PREACT_CIRCLELINE])
		{
			if (false == m_bRadialBlur)
			{
				// 레디얼 블러 활성화
				CGame_Manager::GetInstance()->Set_RadialBlur(true, 16.f, 0.02f);
				m_bRadialBlur = true;
			}

			GET_INSTANCE(CEffect_Manager)->Tick_Generate_Effect(&m_fTimeAcc, 0.2f, fTimeDelta, TEXT("Effect_Glanix_Roar_TrailLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_PREACT_CIRCLELINE], m_pScaleOffset[TYPE_E_PREACT_CIRCLELINE], m_pRotationOffset[TYPE_E_PREACT_CIRCLELINE]);
			
			if (m_iOwnerFrame >= m_pFrameTriger[TYPE_E_PREACT_CIRCLELINE] + 15)
				m_iCount++;
		}
		else if (m_iCount == TYPE_P_PREACT_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_PREACT_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_HandDown_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_PREACT_CIRCLES], m_pScaleOffset[TYPE_P_PREACT_CIRCLES], m_pRotationOffset[TYPE_P_PREACT_CIRCLES]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_PREACT_HANDCIRCLELEFT && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_PREACT_HANDCIRCLELEFT])
		{
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerModel != nullptr)
			{
				Matrix LeftMatrix = pOwnerModel->Get_SocketLocalMatrix(1); // 뼈 행렬 x 플레이어 월드 행렬 = 뼈 월드 행렬
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FourHandSwing_Circle_Hand"),
					LeftMatrix * m_WorldMatrix, m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLELEFT], m_pScaleOffset[TYPE_E_PREACT_HANDCIRCLELEFT], m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLELEFT], nullptr, &m_pHand01Effect);
				Safe_AddRef(m_pHand01Effect);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_PREACT_HANDCIRCLERIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_PREACT_HANDCIRCLERIGHT])
		{
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerModel != nullptr)
			{
				Matrix RightMatrix = pOwnerModel->Get_SocketLocalMatrix(2); // 뼈 행렬 x 플레이어 월드 행렬 = 뼈 월드 행렬
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FourHandSwing_Circle_Hand"),
					RightMatrix * m_WorldMatrix, m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLERIGHT], m_pScaleOffset[TYPE_E_PREACT_HANDCIRCLERIGHT], m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLERIGHT], nullptr, &m_pHand02Effect);
				Safe_AddRef(m_pHand02Effect);
			}
			m_iCount++;
		}


		else if (m_iCount == TYPE_D_ATTACK_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_ATTACK_CRACK])
		{
			m_pWarningDecal->Set_Dead(true);
			Safe_Release(m_pWarningDecal);

			CGame_Manager::GetInstance()->Set_RadialBlur(false);

			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_HandDown_Crack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_ATTACK_CRACK], m_pScaleOffset[TYPE_D_ATTACK_CRACK], m_pRotationOffset[TYPE_D_ATTACK_CRACK]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_ATTACK_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_SPRINGUP])
		{
			if (nullptr != m_pHand01Effect)
			{
				m_pHand01Effect->Set_Dead(true);
				Safe_Release(m_pHand01Effect);
			}

			if (nullptr != m_pHand02Effect)
			{
				m_pHand02Effect->Set_Dead(true);
				Safe_Release(m_pHand02Effect);
			}

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_HandDown_SpringUp"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_SPRINGUP], m_pScaleOffset[TYPE_E_ATTACK_SPRINGUP], m_pRotationOffset[TYPE_E_ATTACK_SPRINGUP], nullptr, &m_SpringUpEffect, false);
			Safe_AddRef(m_SpringUpEffect);
			m_iCount++;
		}
		else if (m_iCount == TTPE_P_ATTACK_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TTPE_P_ATTACK_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_HandDown_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TTPE_P_ATTACK_SMOKE], m_pScaleOffset[TTPE_P_ATTACK_SMOKE], m_pRotationOffset[TTPE_P_ATTACK_SMOKE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_ATTACK_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_ATTACK_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_HandDown_Attack_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_ATTACK_CIRCLES], m_pScaleOffset[TYPE_P_ATTACK_CIRCLES], m_pRotationOffset[TYPE_P_ATTACK_CIRCLES]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}

	// 구 위치 업데이트
	if (nullptr != m_pHand01Effect)
	{
		CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
		CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
		if (pOwnerTransform != nullptr && pOwnerModel != nullptr)
		{
			CTransform* pEffectTransform = m_pHand01Effect->Get_Component<CTransform>(L"Com_Transform");
			if (pEffectTransform != nullptr)
			{
				Matrix RightHandMatrix = pOwnerModel->Get_SocketLocalMatrix(1);
				Matrix OwnerWorldMatrix = pOwnerTransform->Get_WorldMatrix();

				// Scale / Rotation
				Matrix matScale = matScale.CreateScale(pEffectTransform->Get_Scale());//m_pScaleOffset[TYPE_E_CIRCLE_LHAND_06]);
				Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLELEFT].x), XMConvertToRadians(m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLELEFT].y), XMConvertToRadians(m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLELEFT].z)));
				Matrix matResult = matScale * matRotation * (RightHandMatrix * OwnerWorldMatrix);
				pEffectTransform->Set_WorldMatrix(matResult);

				// Position
				_vector vCurrentPosition = pEffectTransform->Get_Position();
				_vector vFinalPosition = vCurrentPosition;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLELEFT].x;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP) * m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLELEFT].y;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK) * m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLELEFT].z;
				pEffectTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));
			}
		}
	}

	if (nullptr != m_pHand02Effect)
	{
		CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
		CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
		if (pOwnerTransform != nullptr && pOwnerModel != nullptr)
		{
			CTransform* pEffectTransform = m_pHand02Effect->Get_Component<CTransform>(L"Com_Transform");
			if (pEffectTransform != nullptr)
			{
				Matrix RightHandMatrix = pOwnerModel->Get_SocketLocalMatrix(2);
				Matrix OwnerWorldMatrix = pOwnerTransform->Get_WorldMatrix();

				// Scale / Rotation
				Matrix matScale = matScale.CreateScale(pEffectTransform->Get_Scale());
				Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLERIGHT].x), XMConvertToRadians(m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLERIGHT].y), XMConvertToRadians(m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLERIGHT].z)));
				Matrix matResult = matScale * matRotation * (RightHandMatrix * OwnerWorldMatrix);
				pEffectTransform->Set_WorldMatrix(matResult);

				// Position
				_vector vCurrentPosition = pEffectTransform->Get_Position();
				_vector vFinalPosition = vCurrentPosition;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLERIGHT].x;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP) * m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLERIGHT].y;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK) * m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLERIGHT].z;
				pEffectTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));
			}
		}
	}
}

void CVfx_Glanix_Skill_HandDown::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_HandDown::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_HandDown::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_HandDown* CVfx_Glanix_Skill_HandDown::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_HandDown* pInstance = new CVfx_Glanix_Skill_HandDown(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_HandDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_HandDown::Clone(void* pArg)
{
	CVfx_Glanix_Skill_HandDown* pInstance = new CVfx_Glanix_Skill_HandDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_HandDown");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_HandDown::Free()
{
	__super::Free();

	if (nullptr != m_pHand01Effect)
	{
		m_pHand01Effect->Set_Dead(true);
		Safe_Release(m_pHand01Effect);
	}

	if (nullptr != m_pHand02Effect)
	{
		m_pHand02Effect->Set_Dead(true);
		Safe_Release(m_pHand02Effect);
	}

	if (nullptr != m_pWarningDecal)
	{
		m_pWarningDecal->Set_Dead(true);
		Safe_Release(m_pWarningDecal);
	}

	if (nullptr != m_SpringUpEffect)
	{
		m_SpringUpEffect->Start_Dissolve(73, // Index
			_float4(1.f, 1.f, 1.f, 1.f),     // Color
			4.f,   // Speed
			10.f); // Total

		Safe_Release(m_SpringUpEffect);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
	else
	{
		CGame_Manager::GetInstance()->Set_RadialBlur(false);
	}
}