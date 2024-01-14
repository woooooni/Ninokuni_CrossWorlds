#include "stdafx.h"
#include "Vfx_Glanix_Skill_SwingDown.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"
#include "Effect.h"

CVfx_Glanix_Skill_SwingDown::CVfx_Glanix_Skill_SwingDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_SwingDown::CVfx_Glanix_Skill_SwingDown(const CVfx_Glanix_Skill_SwingDown& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_SwingDown::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_SPINBOMB;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_D_START_WARNING]    = 0;
		m_pPositionOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_D_START_WARNING]    = _float3(7.f, 5.f, 7.f);
		m_pRotationOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_E_SWING_HANDCIRCLE_RIGHT] = 20;
		m_pPositionOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT]    = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_01] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_01] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_01] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_01] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_01] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_01] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_01] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_02] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_02] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_02] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_02] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_02] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_02] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_02] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_02] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_03] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_03] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_03] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_03] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_03] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_03] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_03] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_03] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_04] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_04] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_04] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_04] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_04] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_04] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_04] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_04] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_05] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_05] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_05] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_05] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_05] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_05] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_05] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_05] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_06] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_06] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_06] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_06] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_06] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_06] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_06] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_06] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_07] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_07] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_07] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_07] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_07] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_07] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_07] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_07] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_08] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_08] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_08] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_08] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_08] = 60;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_08] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_08] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_08] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_D_ATTACK_WARNING]    = 70;
		m_pPositionOffset[TYPE_D_ATTACK_WARNING] = _float3(0.f, 0.f, 0.5f);
		m_pScaleOffset[TYPE_D_ATTACK_WARNING]    = _float3(7.f, 5.f, 7.f);
		m_pRotationOffset[TYPE_D_ATTACK_WARNING] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_ATTACK_HANDCIRCLE_LEFT]    = 102;
		m_pPositionOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT]    = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_D_ATTACK_CRACK]    = 108;
		m_pPositionOffset[TYPE_D_ATTACK_CRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_D_ATTACK_CRACK]    = _float3(8.f, 5.f, 8.f);
		m_pRotationOffset[TYPE_D_ATTACK_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_ATTACK_SPRINGUP]    = 110;
		m_pPositionOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_ATTACK_SPRINGUP]    = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_ATTACK_SMOKE]    = 111;
		m_pPositionOffset[TYPE_P_ATTACK_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_ATTACK_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_ATTACK_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_ATTACK_CIRCLES]    = 112;
		m_pPositionOffset[TYPE_P_ATTACK_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_ATTACK_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_ATTACK_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_ATTACK_CIRCLELINE]    = 114;
		m_pPositionOffset[TYPE_E_ATTACK_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_ATTACK_CIRCLELINE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_E_ATTACK_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
    }

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_SwingDown::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_SwingDown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_START_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_START_WARNING])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_SwingDown_Warning"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_START_WARNING], m_pScaleOffset[TYPE_D_START_WARNING], m_pRotationOffset[TYPE_D_START_WARNING]);
			m_iCount++;
		}
		//

		else if (m_iCount == TYPE_E_SWING_HANDCIRCLE_RIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_SWING_HANDCIRCLE_RIGHT])
		{
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerModel != nullptr)
			{
				Matrix RightMatrix = pOwnerModel->Get_SocketLocalMatrix(2); // 뼈 행렬 x 플레이어 월드 행렬 = 뼈 월드 행렬
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FourHandSwing_Circle_Hand"),
					RightMatrix * m_WorldMatrix, m_pPositionOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT], m_pScaleOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT], m_pRotationOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT], nullptr, &m_pHand02Effect);
				Safe_AddRef(m_pHand02Effect);
			}
			m_iCount++;
		}
		
		else if (m_iCount == TYPE_P_SWING_SMOKE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_01])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_01])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_02])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_02])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_03])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_03])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_04])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_04])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_05 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_05])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_05 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_05])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_06 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_06])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_06 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_06])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_07 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_07])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_07 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_07])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_08 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_08])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_08 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_08])
		{

			//if (nullptr != m_pHand02Effect)
			//{
			//	m_pHand02Effect->Set_Dead(true);
			//	Safe_Release(m_pHand02Effect);
			//}

			m_iCount++;
		}
		//

#pragma region Down + CVfx_Glanix_Skill_FootDown
		else if (m_iCount == TYPE_D_ATTACK_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_ATTACK_WARNING])
		{
		    CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr != pOwnerTransform)
			{
				m_WorldMatrix = pOwnerTransform->Get_WorldFloat4x4();
				GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_SwingDown_Warning"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_ATTACK_WARNING], m_pScaleOffset[TYPE_D_ATTACK_WARNING], m_pRotationOffset[TYPE_D_ATTACK_WARNING]);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_ATTACK_HANDCIRCLE_LEFT && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_HANDCIRCLE_LEFT])
		{
			CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
			if (pOwnerModel != nullptr)
			{
				Matrix LeftMatrix = pOwnerModel->Get_SocketLocalMatrix(1);
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FourHandSwing_Circle_Hand"),
					LeftMatrix * m_WorldMatrix, m_pPositionOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT], m_pScaleOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT], m_pRotationOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT], nullptr, &m_pHand01Effect);
				Safe_AddRef(m_pHand01Effect);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_D_ATTACK_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_ATTACK_CRACK])
		{
		  //  if (nullptr != m_pHand01Effect)
		  //  {
				//m_pHand01Effect->Set_Dead(true);
		  //  	Safe_Release(m_pHand01Effect);
		  //  }

		    GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_SwingDown_Crack"),
		    	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_ATTACK_CRACK], m_pScaleOffset[TYPE_D_ATTACK_CRACK], m_pRotationOffset[TYPE_D_ATTACK_CRACK]);
		    m_iCount++;
		}
		else if (m_iCount == TYPE_E_ATTACK_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_SPRINGUP])
		{
		    GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_SwingDown_SpringUp"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_SPRINGUP], m_pScaleOffset[TYPE_E_ATTACK_SPRINGUP], m_pRotationOffset[TYPE_E_ATTACK_SPRINGUP]);
			m_iCount++;
        }
#pragma region CVfx_Glanix_Skill_FootDown
		else if (m_iCount == TYPE_P_ATTACK_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_ATTACK_SMOKE])
		{
		    GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FootDown_Smoke"),
		    	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_ATTACK_SMOKE], m_pScaleOffset[TYPE_P_ATTACK_SMOKE], m_pRotationOffset[TYPE_P_ATTACK_SMOKE]);
		    m_iCount++;
		}
		else if (m_iCount == TYPE_P_ATTACK_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_ATTACK_CIRCLES])
		{
		    GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FootDown_Circle"),
		    	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_ATTACK_CIRCLES], m_pScaleOffset[TYPE_P_ATTACK_CIRCLES], m_pRotationOffset[TYPE_P_ATTACK_CIRCLES]);
		    m_iCount++;
		}
		else if (m_iCount == TYPE_E_ATTACK_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_CIRCLELINE])
		{
		    GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FootDown_TrailLine"),
		    	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ATTACK_CIRCLELINE], m_pScaleOffset[TYPE_E_ATTACK_CIRCLELINE], m_pRotationOffset[TYPE_E_ATTACK_CIRCLELINE]);
		    m_iCount++;
		}
#pragma endregion
#pragma endregion

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
				Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT].x), XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT].y), XMConvertToRadians(m_pRotationOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT].z)));
				Matrix matResult = matScale * matRotation * (RightHandMatrix * OwnerWorldMatrix);
				pEffectTransform->Set_WorldMatrix(matResult);

				// Position
				_vector vCurrentPosition = pEffectTransform->Get_Position();
				_vector vFinalPosition = vCurrentPosition;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT].x;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP)    * m_pPositionOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT].y;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK)  * m_pPositionOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT].z;
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
				Matrix matRotation = matScale.CreateFromYawPitchRoll(Vec3(XMConvertToRadians(m_pRotationOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT].x), XMConvertToRadians(m_pRotationOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT].y), XMConvertToRadians(m_pRotationOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT].z)));
				Matrix matResult = matScale * matRotation * (RightHandMatrix * OwnerWorldMatrix);
				pEffectTransform->Set_WorldMatrix(matResult);

				// Position
				_vector vCurrentPosition = pEffectTransform->Get_Position();
				_vector vFinalPosition = vCurrentPosition;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_RIGHT) * m_pPositionOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT].x;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_UP)    * m_pPositionOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT].y;
				vFinalPosition += pEffectTransform->Get_State(CTransform::STATE_LOOK)  * m_pPositionOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT].z;
				pEffectTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(XMVectorGetX(vFinalPosition), XMVectorGetY(vFinalPosition), XMVectorGetZ(vFinalPosition), 1.f));
			}
		}
	}
}

void CVfx_Glanix_Skill_SwingDown::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_SwingDown::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_SwingDown::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_SwingDown* CVfx_Glanix_Skill_SwingDown::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_SwingDown* pInstance = new CVfx_Glanix_Skill_SwingDown(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_SwingDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_SwingDown::Clone(void* pArg)
{
	CVfx_Glanix_Skill_SwingDown* pInstance = new CVfx_Glanix_Skill_SwingDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_SwingDown");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_SwingDown::Free()
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

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}