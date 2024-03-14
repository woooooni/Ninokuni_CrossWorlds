#include "stdafx.h"
#include "Glanix_IcePillar_Controller.h"

#include "GameInstance.h"

#include "Glanix.h"
#include "Glanix_IcePillar.h"

#include "StateMachine.h"
#include "Model.h"
#include "Animation.h"

#include "Decal.h"
#include "Effect_Manager.h"

CGlanix_IcePillar_Controller::CGlanix_IcePillar_Controller()
{
}

CGlanix_IcePillar_Controller::~CGlanix_IcePillar_Controller()
{
}

void CGlanix_IcePillar_Controller::Tick(const _float fTimeDelta)
{
	if (nullptr == m_pGlanix || m_pPillars.empty())
		return;

	if (m_pWarningDecal == nullptr)
	{
		m_matWarning._11 = m_fDecalSize;
		m_matWarning._22 = 1.f;
		m_matWarning._33 = m_fDecalSize;

		m_matWarning._41 = m_vOriginPos.x;
		m_matWarning._42 = 2.f;
		m_matWarning._43 = m_vOriginPos.z;
		m_matWarning._44 = 1.f;

		GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_Rage01_Warning"),
			XMLoadFloat4x4(&m_matWarning), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), nullptr, &m_pWarningDecal, false);
		Safe_AddRef(m_pWarningDecal);
	}

	// 데칼 줄어들기
	if (m_bIsSmallerDecal)
	{
		if (m_fDecalSize > m_fSmallerDest)
		{
			m_fDecalSize -= fTimeDelta * 10.f;

			m_matWarning._11 = m_fDecalSize;
			m_matWarning._33 = m_fDecalSize;

			if (m_pWarningDecal != nullptr)
			{
				m_pWarningDecal->Get_TransformCom()->Set_Scale(Vec3(m_fDecalSize, 1.f, m_fDecalSize));
			}
		}
		else
		{
			if(!m_bCollision)
				m_bIsSmallerDecal = false;
		}
	}

	/* 콜라이더 On Off */
	if (TEXT("SKM_Glanix.ao|Glanix_RageSkillCharge") == m_pGlanixModelCom->Get_CurrAnimation()->Get_AnimationName())
	{
		if (!m_bOnCollider)
		{
			if (!m_pGlanixModelCom->Is_Tween() && 50 <= m_pGlanixModelCom->Get_CurrAnimationFrame())
			{
				for (auto& pPillar : m_pPillars)
					pPillar->Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, true);
			
				m_bOnCollider = true;
				m_bOffCollider = false;
			}
		}
	}
	else
	{
		if (!m_bOffCollider)
		{
			for (auto& pPillar : m_pPillars)
				pPillar->Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, false);

			m_bOnCollider = false;
			m_bOffCollider = true;
		}
	}

	/* 충돌 이후, m_fStopDuration 동안 멈춰있다가 다시 시작 */
	if (m_bCollision)
	{
		// 데칼 줄어들기
		if (!m_bIsSmallerDecal)
		{
			m_bIsSmallerDecal = true;
			m_fSmallerDest = m_fDecalSize - m_fSmallerValue;
		}

		m_fAcc += fTimeDelta;
		if (m_fStopDuration <= m_fAcc)
		{
			m_bCollision = false;

			m_fAcc = 0.f;

			Accelerate_PillarsSpeed();
		}
	}
	else /* 킬 카운트가 2 넘으면 시간 패턴 추가 */
	{
		if (2 <= m_iKillCount)
		{
			if (m_bAcceleration && !m_pPillars[0]->m_tSpeedDesc.bActive)
			{
				m_fAcc += fTimeDelta;

				if (m_fNormalSpeedDuration <= m_fAcc) /* 빠르게 변환! */
				{
					m_fAcc = 0.f;

					m_bAcceleration = false;

					const _float fTargetSpeed = (2 <= m_iKillCount) ? m_fDefaultRotSpeed * -4.f : m_fDefaultRotSpeed * 4.f;

					for (auto& pPillar : m_pPillars)
					{
						pPillar->m_tSpeedDesc.Start(
							pPillar->m_tSpeedDesc.fCurValue,
							fTargetSpeed,
							0.75f,
							LERP_MODE::EASE_OUT);
					}
				}
				else /* 일시적 역방향 스윙 */
				{
					for (auto& pPillar : m_pPillars)
					{
						pPillar->m_tSpeedDesc.fCurValue += (2 <= m_iKillCount) ? +fTimeDelta * 0.5f : -fTimeDelta * 0.5f;
					}
				}
			}
			else if(!m_bAcceleration && !m_pPillars[0]->m_tSpeedDesc.bActive) /* 느리게 변환! */
			{
				m_bAcceleration = true;

				const _float fTargetSpeed = (2 <= m_iKillCount) ? m_fDefaultRotSpeed * -1.2f : m_fDefaultRotSpeed * 1.2f;

				for (auto& pPillar : m_pPillars)
				{
					pPillar->m_tSpeedDesc.Start(
						pPillar->m_tSpeedDesc.fCurValue,
						fTargetSpeed,
						0.5f,
						LERP_MODE::EASE_OUT);
				}
			}
		}
	}
}

HRESULT CGlanix_IcePillar_Controller::Create_Pillars(const _int& iNum, const _float& fRadius, const Vec4& vOriginPos, CGlanix* pGlanix)
{
#ifdef _DEBUG
	if (FAILED(Ready_DebugDraw()))
		return E_FAIL;
#endif

	/* 얼음 기둥 생성 */
	m_fDeathDistance = m_fRadius = fRadius;
	m_vOriginPos = vOriginPos;
	m_pGlanix	 = pGlanix;
	if (nullptr == m_pGlanix)
		return E_FAIL;
	m_pGlanixModelCom = pGlanix->Get_Component<CModel>(L"Com_Model");
	if (nullptr == m_pGlanixModelCom)
		return E_FAIL;

	for (size_t i = 0; i < iNum; i++)
	{
		const float fAngle = 360.f / (_float)iNum * i;

		Vec4 vPos = Vec4::One;
		{
			vPos.x = vOriginPos.x + fRadius * cos(XMConvertToRadians(fAngle));
			vPos.y = vOriginPos.y;
			vPos.z = vOriginPos.z + fRadius * sin(XMConvertToRadians(fAngle));
		}

		CGameObject* pGameObject = nullptr;
		GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_IcePillar"), &vPos, &pGameObject);

		if (nullptr != pGameObject)
		{
			CGlanix_IcePillar* pPillar = dynamic_cast<CGlanix_IcePillar*>(pGameObject);
			if (nullptr != pPillar)
			{
				/* Set Data */
				{
					pPillar->m_iKey = i;
					pPillar->m_vRotateOriginPos = m_pGlanix->Get_OriginPos();
					pPillar->m_tSpeedDesc.fCurValue = m_fDefaultRotSpeed;
					pPillar->m_pGlanix = m_pGlanix;
				}
				m_pPillars.push_back(pPillar);
			}
		}
	}

	return S_OK;
}

HRESULT CGlanix_IcePillar_Controller::Delete_Pillar(const _int iIndex)
{
	++m_iKillCount;
	m_fAcc = 0.f;
	m_bAcceleration = true;

	m_fDeathDistance -= 4.f;
	if (m_fDeathDistance < 0.f)
		m_fDeathDistance = 0.f;

	for (vector<CGlanix_IcePillar*>::iterator iter = m_pPillars.begin(); iter != m_pPillars.end(); ++iter)
	{
		if ((*iter)->m_iKey == iIndex)
		{
			(*iter)->Set_Dead(true);
			m_pPillars.erase(iter);

			m_bCollision = true;
			Decelerate_PillarsSpeed();
			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CGlanix_IcePillar_Controller::Clear_Pillars()
{
	for (auto& pPillar : m_pPillars)
	{
		pPillar->Set_Dead(true);
	}

	m_pPillars.clear();
	m_pPillars.shrink_to_fit();

	return S_OK;
}

void CGlanix_IcePillar_Controller::Accelerate_PillarsSpeed()
{
	for (auto& pPillar : m_pPillars)
	{
		/* 회전 방향 전환 */
		const _float fTargetSpeed = (2 <= m_iKillCount) ? m_fDefaultRotSpeed * -1.2f : m_fDefaultRotSpeed * 1.2f;

		pPillar->m_tSpeedDesc.Start(
			0.f, 
			fTargetSpeed,
			0.75f, 
			LERP_MODE::EASE_OUT);
	}
}

void CGlanix_IcePillar_Controller::Decelerate_PillarsSpeed()
{
	for (auto& pPillar : m_pPillars)
	{
		pPillar->m_tSpeedDesc.Start(
			pPillar->m_tSpeedDesc.fCurValue, 
			0.f, 
			1.3f, 
			LERP_MODE::EASE_OUT);
	}
}

#ifdef _DEBUG
HRESULT CGlanix_IcePillar_Controller::Render()
{
	Render_DebugDraw();

	return S_OK;
}

HRESULT CGlanix_IcePillar_Controller::Ready_DebugDraw()
{
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(GI->Get_Context());
	m_pEffect = new BasicEffect(GI->Get_Device());

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCodes = nullptr;
	size_t			iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(GI->Get_Device()->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;

	_float	fRadius = 0.5f;
	Vec3	vOrigin = { 0.f, fRadius * 0.5f, 0.f };
	m_pSphere = new BoundingSphere(vOrigin, fRadius);

	if (nullptr == m_pSphere)
		return E_FAIL;

	return S_OK;
}

HRESULT CGlanix_IcePillar_Controller::Render_DebugDraw()
{
	Vec4 vOrigin = m_pGlanix->Get_OriginPos();

	if (m_pEffect != nullptr && m_pBatch != nullptr && m_pSphere != nullptr && m_pInputLayout != nullptr)
	{
		m_pEffect->SetWorld(XMMatrixIdentity());
		m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
		m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

		m_pEffect->Apply(GI->Get_Context());

		GI->Get_Context()->IASetInputLayout(m_pInputLayout);

		m_pBatch->Begin();
		{
			m_pSphere->Center = Vec4(vOrigin + Vec3(1.f, 0.f, 0.f) * m_fDeathDistance).xyz();
			DX::Draw(m_pBatch, *m_pSphere, Colors::Yellow);

			m_pSphere->Center = Vec4(vOrigin + Vec3(-1.f, 0.f, 0.f) * m_fDeathDistance).xyz();
			DX::Draw(m_pBatch, *m_pSphere, Colors::Yellow);

			m_pSphere->Center = Vec4(vOrigin + Vec3(0.f, 0.f, 1.f) * m_fDeathDistance).xyz();
			DX::Draw(m_pBatch, *m_pSphere, Colors::Yellow);

			m_pSphere->Center = Vec4(vOrigin + Vec3(0.f, 0.f, -1.f) * m_fDeathDistance).xyz();
			DX::Draw(m_pBatch, *m_pSphere, Colors::Yellow);
		}
		m_pBatch->End();
	}

	return S_OK;
}
#endif


void CGlanix_IcePillar_Controller::PillarControllerFree()
{
#ifdef _DEBUG
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pSphere);
	Safe_Release(m_pInputLayout);
#endif
	if (m_pWarningDecal != nullptr)
	{
		m_pWarningDecal->Set_Dead(true);
		Safe_Release(m_pWarningDecal);
	}
}
