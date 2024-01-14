#include "stdafx.h"
#include "Glanix_IcePillar_Controller.h"

#include "GameInstance.h"

#include "Glanix.h"
#include "Glanix_IcePillar.h"

#include "StateMachine.h"
#include "Model.h"
#include "Animation.h"

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

	m_fDeathDistance -= 3.f;
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
