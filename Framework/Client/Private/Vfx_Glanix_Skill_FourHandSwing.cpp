#include "stdafx.h"
#include "Vfx_Glanix_Skill_FourHandSwing.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"
#include "Effect.h"

CVfx_Glanix_Skill_FourHandSwing::CVfx_Glanix_Skill_FourHandSwing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_FourHandSwing::CVfx_Glanix_Skill_FourHandSwing(const CVfx_Glanix_Skill_FourHandSwing& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_FourHandSwing::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_QUADBLOW;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	/* 0. Warning Decal */
	m_pFrameTriger[TYPE_D_DECAL_00]    = 15;
	m_pPositionOffset[TYPE_D_DECAL_00] = _float3(0.f, 0.f, 0.5f);
	m_pScaleOffset[TYPE_D_DECAL_00]    = _float3(5.f, 5.f, 9.f);
	m_pRotationOffset[TYPE_D_DECAL_00] = _float3(0.f, 0.f, 0.f);

	/* 1. Roar */
	{
		{
			m_pFrameTriger[TYPE_P_SPARKLE_01] = 42;
			m_pPositionOffset[TYPE_P_SPARKLE_01] = _float3(0.f, 3.f, 1.f);
			m_pScaleOffset[TYPE_P_SPARKLE_01] = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_SPARKLE_01] = _float3(0.f, 0.f, 0.f);

			m_pFrameTriger[TYPE_P_SMOKE_01] = 42;
			m_pPositionOffset[TYPE_P_SMOKE_01] = _float3(0.f, 3.f, 1.f);
			m_pScaleOffset[TYPE_P_SMOKE_01] = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_SMOKE_01] = _float3(0.f, 0.f, 0.f);
		}

		{
			m_pFrameTriger[TYPE_P_SPARKLE_02] = 51;
			m_pPositionOffset[TYPE_P_SPARKLE_02] = _float3(0.f, 3.f, 1.f);
			m_pScaleOffset[TYPE_P_SPARKLE_02] = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_SPARKLE_02] = _float3(0.f, 0.f, 0.f);

			m_pFrameTriger[TYPE_P_SMOKE_02] = 51;
			m_pPositionOffset[TYPE_P_SMOKE_02] = _float3(0.f, 3.f, 1.f);
			m_pScaleOffset[TYPE_P_SMOKE_02] = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_SMOKE_02] = _float3(0.f, 0.f, 0.f);
		}

		{
			m_pFrameTriger[TYPE_P_SPARKLE_03] = 56;
			m_pPositionOffset[TYPE_P_SPARKLE_03] = _float3(0.f, 3.f, 1.f);
			m_pScaleOffset[TYPE_P_SPARKLE_03] = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_SPARKLE_03] = _float3(0.f, 0.f, 0.f);

			m_pFrameTriger[TYPE_P_SMOKE_03] = 56;
			m_pPositionOffset[TYPE_P_SMOKE_03] = _float3(0.f, 3.f, 1.f);
			m_pScaleOffset[TYPE_P_SMOKE_03] = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_SMOKE_03] = _float3(0.f, 0.f, 0.f);
		}

		{
			m_pFrameTriger[TYPE_P_LIGHTSPARKLE_04] = 61;
			m_pPositionOffset[TYPE_P_LIGHTSPARKLE_04] = _float3(0.f, 3.f, 1.f);
			m_pScaleOffset[TYPE_P_LIGHTSPARKLE_04] = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_LIGHTSPARKLE_04] = _float3(0.f, 0.f, 0.f);
		}

		{
			m_pFrameTriger[TYPE_P_LIGHTSPARKLE_05] = 65;
			m_pPositionOffset[TYPE_P_LIGHTSPARKLE_05] = _float3(0.f, 3.f, 1.f);
			m_pScaleOffset[TYPE_P_LIGHTSPARKLE_05] = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_LIGHTSPARKLE_05] = _float3(0.f, 0.f, 0.f);
		}
	}

	/* 2. HandLight */
	{
		m_pFrameTriger[TYPE_E_CIRCLE_LHAND_06] = 75;
		m_pPositionOffset[TYPE_E_CIRCLE_LHAND_06] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_CIRCLE_LHAND_06] = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_E_CIRCLE_LHAND_06] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_CIRCLE_RHAND_06] = 75;
		m_pPositionOffset[TYPE_E_CIRCLE_RHAND_06] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_CIRCLE_RHAND_06] = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_E_CIRCLE_RHAND_06] = _float3(0.f, 0.f, 0.f);
	}

	/* 3. Attack */
	{
		{
			m_pFrameTriger[TYPE_E_TRAIL_07]    = 87;
			m_pPositionOffset[TYPE_E_TRAIL_07] = _float3(0.f, 0.7f, 0.5f);
			m_pScaleOffset[TYPE_E_TRAIL_07]    = _float3(5.f, 5.f, 5.f);
			m_pRotationOffset[TYPE_E_TRAIL_07] = _float3(0.f, -20.f, 10.f);

			m_pFrameTriger[TYPE_P_CIRCLE_07]    = 92;
			m_pPositionOffset[TYPE_P_CIRCLE_07] = _float3(0.f, 0.f, 0.f);
			m_pScaleOffset[TYPE_P_CIRCLE_07]    = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_CIRCLE_07] = _float3(0.f, 0.f, 0.f);
		}

		{
			m_pFrameTriger[TYPE_E_TRAIL_08]    = 102;
			m_pPositionOffset[TYPE_E_TRAIL_08] = _float3(0.f, 0.5f, 0.5f);
			m_pScaleOffset[TYPE_E_TRAIL_08]    = _float3(5.f, 5.f, 5.f);
			m_pRotationOffset[TYPE_E_TRAIL_08] = _float3(0.f, 20.f, -40.f);

			m_pFrameTriger[TYPE_P_CIRCLE_08]    = 112;
			m_pPositionOffset[TYPE_P_CIRCLE_08] = _float3(0.f, 0.f, 0.f);
			m_pScaleOffset[TYPE_P_CIRCLE_08]    = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_CIRCLE_08] = _float3(0.f, 0.f, 0.f);
		}

		{
			m_pFrameTriger[TYPE_E_TRAIL_09]    = 116;
			m_pPositionOffset[TYPE_E_TRAIL_09] = _float3(0.f, 0.7f, 0.5f);
			m_pScaleOffset[TYPE_E_TRAIL_09]    = _float3(5.f, 5.f, 5.f);
			m_pRotationOffset[TYPE_E_TRAIL_09] = _float3(0.f, -20.f, -20.f);

			m_pFrameTriger[TYPE_P_CIRCLE_09]    = 122;
			m_pPositionOffset[TYPE_P_CIRCLE_09] = _float3(0.f, 0.f, 0.f);
			m_pScaleOffset[TYPE_P_CIRCLE_09]    = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_CIRCLE_09] = _float3(0.f, 0.f, 0.f);
		}

		{
			m_pFrameTriger[TYPE_E_TRAIL_10]    = 129;
			m_pPositionOffset[TYPE_E_TRAIL_10] = _float3(0.f, 0.7f, 0.5f);
			m_pScaleOffset[TYPE_E_TRAIL_10]    = _float3(5.f, 5.f, 5.f);
			m_pRotationOffset[TYPE_E_TRAIL_10] = _float3(0.f, 20.f, 10.f);

			m_pFrameTriger[TYPE_P_CIRCLE_10]    = 137;
			m_pPositionOffset[TYPE_P_CIRCLE_10] = _float3(0.f, 0.f, 0.f);
			m_pScaleOffset[TYPE_P_CIRCLE_10]    = _float3(1.f, 1.f, 1.f);
			m_pRotationOffset[TYPE_P_CIRCLE_10] = _float3(0.f, 0.f, 0.f);
		}
	}

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_FourHandSwing::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_FourHandSwing::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_DECAL_00 && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_DECAL_00])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_FourHandSwing_Warning"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_DECAL_00], m_pScaleOffset[TYPE_D_DECAL_00], m_pRotationOffset[TYPE_D_DECAL_00]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SPARKLE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SPARKLE_01])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SPARKLE_01], m_pScaleOffset[TYPE_P_SPARKLE_01], m_pRotationOffset[TYPE_P_SPARKLE_01]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_01])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_01], m_pScaleOffset[TYPE_P_SMOKE_01], m_pRotationOffset[TYPE_P_SMOKE_01]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SPARKLE_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SPARKLE_02])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SPARKLE_02], m_pScaleOffset[TYPE_P_SPARKLE_02], m_pRotationOffset[TYPE_P_SPARKLE_02]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_02])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_02], m_pScaleOffset[TYPE_P_SMOKE_02], m_pRotationOffset[TYPE_P_SMOKE_02]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SPARKLE_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SPARKLE_03])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SPARKLE_03], m_pScaleOffset[TYPE_P_SPARKLE_03], m_pRotationOffset[TYPE_P_SPARKLE_03]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_03])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_03], m_pScaleOffset[TYPE_P_SMOKE_03], m_pRotationOffset[TYPE_P_SMOKE_03]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_LIGHTSPARKLE_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_LIGHTSPARKLE_04])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_SparkleLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_LIGHTSPARKLE_04], m_pScaleOffset[TYPE_P_LIGHTSPARKLE_04], m_pRotationOffset[TYPE_P_LIGHTSPARKLE_04]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_LIGHTSPARKLE_05 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_LIGHTSPARKLE_05])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_SparkleLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_LIGHTSPARKLE_05], m_pScaleOffset[TYPE_P_LIGHTSPARKLE_05], m_pRotationOffset[TYPE_P_LIGHTSPARKLE_05]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_CIRCLE_LHAND_06 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_CIRCLE_LHAND_06])
		{
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerModel != nullptr)
			{
				Matrix LeftMatrix = pOwnerModel->Get_SocketLocalMatrix(1); // 뼈 행렬 x 플레이어 월드 행렬 = 뼈 월드 행렬
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FourHandSwing_Circle_Hand"),
					LeftMatrix * m_WorldMatrix, m_pPositionOffset[TYPE_E_CIRCLE_LHAND_06], m_pScaleOffset[TYPE_E_CIRCLE_LHAND_06], m_pRotationOffset[TYPE_E_CIRCLE_LHAND_06], nullptr, &m_pHand01Effect);
				Safe_AddRef(m_pHand01Effect);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_CIRCLE_RHAND_06 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_CIRCLE_RHAND_06])
		{
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerModel != nullptr)
			{
				Matrix RightMatrix = pOwnerModel->Get_SocketLocalMatrix(2); // 뼈 행렬 x 플레이어 월드 행렬 = 뼈 월드 행렬
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FourHandSwing_Circle_Hand"),
					RightMatrix * m_WorldMatrix, m_pPositionOffset[TYPE_E_CIRCLE_RHAND_06], m_pScaleOffset[TYPE_E_CIRCLE_RHAND_06], m_pRotationOffset[TYPE_E_CIRCLE_RHAND_06], nullptr, &m_pHand02Effect);
				Safe_AddRef(m_pHand02Effect);
			}
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_TRAIL_07 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_07])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FourHandSwing_Trail_Right"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_07], m_pScaleOffset[TYPE_E_TRAIL_07], m_pRotationOffset[TYPE_E_TRAIL_07]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLE_07 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLE_07])
		{
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerModel != nullptr)
			{
				Matrix LeftMatrix = pOwnerModel->Get_SocketLocalMatrix(1);
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Circle"),
					LeftMatrix * m_WorldMatrix, m_pPositionOffset[TYPE_P_CIRCLE_07], m_pScaleOffset[TYPE_P_CIRCLE_07], m_pRotationOffset[TYPE_P_CIRCLE_07]);
			}
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_TRAIL_08 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_08])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FourHandSwing_Trail_Left"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_08], m_pScaleOffset[TYPE_E_TRAIL_08], m_pRotationOffset[TYPE_E_TRAIL_08]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLE_08 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLE_08])
		{
		    CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
		    if (pOwnerModel != nullptr)
		    {
		    	Matrix RightMatrix = pOwnerModel->Get_SocketLocalMatrix(2); // 뼈 행렬 x 플레이어 월드 행렬 = 뼈 월드 행렬
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Circle"),
					RightMatrix* m_WorldMatrix, m_pPositionOffset[TYPE_P_CIRCLE_08], m_pScaleOffset[TYPE_P_CIRCLE_08], m_pRotationOffset[TYPE_P_CIRCLE_08]);
		    }
		    m_iCount++;
		}

		else if (m_iCount == TYPE_E_TRAIL_09 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_09])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FourHandSwing_Trail_Right"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_09], m_pScaleOffset[TYPE_E_TRAIL_09], m_pRotationOffset[TYPE_E_TRAIL_09]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLE_09 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLE_09])
		{
		    CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
		    if (pOwnerModel != nullptr)
		    {
				Matrix LeftMatrix = pOwnerModel->Get_SocketLocalMatrix(1);
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Circle"),
					LeftMatrix* m_WorldMatrix, m_pPositionOffset[TYPE_P_CIRCLE_09], m_pScaleOffset[TYPE_P_CIRCLE_09], m_pRotationOffset[TYPE_P_CIRCLE_09]);
		    }
		    m_iCount++;
		}

		else if (m_iCount == TYPE_E_TRAIL_10 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_10])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FourHandSwing_Trail_Left"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_10], m_pScaleOffset[TYPE_E_TRAIL_10], m_pRotationOffset[TYPE_E_TRAIL_10]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLE_10 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLE_10])
		{
		    CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
		    if (pOwnerModel != nullptr)
		    {
				Matrix RightMatrix = pOwnerModel->Get_SocketLocalMatrix(2); // 뼈 행렬 x 플레이어 월드 행렬 = 뼈 월드 행렬
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Circle"),
					RightMatrix * m_WorldMatrix, m_pPositionOffset[TYPE_P_CIRCLE_10], m_pScaleOffset[TYPE_P_CIRCLE_10], m_pRotationOffset[TYPE_P_CIRCLE_10]);
		    }
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
				Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[TYPE_E_CIRCLE_LHAND_06].x), XMConvertToRadians(m_pRotationOffset[TYPE_E_CIRCLE_LHAND_06].y), XMConvertToRadians(m_pRotationOffset[TYPE_E_CIRCLE_LHAND_06].z)));
				Matrix matResult = matScale * matRotation * (RightHandMatrix * OwnerWorldMatrix);
				pEffectTransform->Set_WorldMatrix(matResult);

				// Position
				_vector vCurrentPosition = pEffectTransform->Get_Position();
				_vector vFinalPosition = vCurrentPosition;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[TYPE_E_CIRCLE_LHAND_06].x;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP) * m_pPositionOffset[TYPE_E_CIRCLE_LHAND_06].y;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK) * m_pPositionOffset[TYPE_E_CIRCLE_LHAND_06].z;
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
				Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[TYPE_E_CIRCLE_RHAND_06].x), XMConvertToRadians(m_pRotationOffset[TYPE_E_CIRCLE_RHAND_06].y), XMConvertToRadians(m_pRotationOffset[TYPE_E_CIRCLE_RHAND_06].z)));
				Matrix matResult = matScale * matRotation * (RightHandMatrix * OwnerWorldMatrix);
				pEffectTransform->Set_WorldMatrix(matResult);

				// Position
				_vector vCurrentPosition = pEffectTransform->Get_Position();
				_vector vFinalPosition = vCurrentPosition;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[TYPE_E_CIRCLE_RHAND_06].x;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP) * m_pPositionOffset[TYPE_E_CIRCLE_RHAND_06].y;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK) * m_pPositionOffset[TYPE_E_CIRCLE_RHAND_06].z;
				pEffectTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));
			}
		}
	}
}

void CVfx_Glanix_Skill_FourHandSwing::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_FourHandSwing::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_FourHandSwing::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_FourHandSwing* CVfx_Glanix_Skill_FourHandSwing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_FourHandSwing* pInstance = new CVfx_Glanix_Skill_FourHandSwing(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_FourHandSwing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_FourHandSwing::Clone(void* pArg)
{
	CVfx_Glanix_Skill_FourHandSwing* pInstance = new CVfx_Glanix_Skill_FourHandSwing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_FourHandSwing");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_FourHandSwing::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}

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
}