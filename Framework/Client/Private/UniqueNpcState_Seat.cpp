#include "stdafx.h"
#include "UniqueNpcState_Seat.h"

#include "GameNpc.h"

#include "Animation.h"
#include "RubyCarriage.h"
#include "Ruby.h"

CUniqueNpcState_Seat::CUniqueNpcState_Seat(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CUniqueNpcState_Seat::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CUniqueNpcState_Seat::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CUniqueNpcState_Seat::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	vector<Vec4>& vPoints = *m_pNpc->Get_RoamingArea();
	_uint iRouteSize = m_pNpc->Get_RoamingArea()->size();
	Vec3 vOneDistance = vPoints[3] - Vec3(m_pTransformCom->Get_Position());
	_float fOneSectionDistance = vOneDistance.Length();
	Vec3 vTwoDistance = vPoints[6] - Vec3(m_pTransformCom->Get_Position());
	_float fTwoSectionDistance = vTwoDistance.Length();
	Vec3 vThreeDistance = vPoints[9] - Vec3(m_pTransformCom->Get_Position());
	_float fThreeDistance = vThreeDistance.Length();
	Vec3 vFourDistance = vPoints[12] - Vec3(m_pTransformCom->Get_Position());
	_float fFourDistance = vFourDistance.Length();
	Vec3 vFiveDistance = vPoints[15] - Vec3(m_pTransformCom->Get_Position());
	_float fFiveDistance = vFiveDistance.Length();

	// 섹션별로 구간을 나눠서.

	if (iRouteSize >= m_pNpc->Get_CurRoamingIndex())
	{
		// Size 7개
		const _float fArriveTime = 15.0f;
		m_fTime += fTimeDelta;

		if (fOneSectionDistance >= 4.0f && false == m_bSection[SECTION_ONE])
		{
			_float fRatio = std::clamp(m_fTime / fArriveTime, 0.0f, 1.0f);
			Vec3 vAt = CubicBezier(vPoints[0], vPoints[1], vPoints[2], vPoints[3], fRatio);
			m_pTransformCom->LookAt(vAt);
			m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pNpc->Get_Stat()->fSpeed, fTimeDelta);
		}
		else if (fTwoSectionDistance >= 4.0f && false == m_bSection[SECTION_TWO])
		{
			if (false == m_bSection[SECTION_ONE])
			{
				m_fTime = 0.0f;
				m_fTime += fTimeDelta;
				m_bSection[SECTION_ONE] = true;
			}

			_float fRatio = std::clamp(m_fTime / fArriveTime, 0.0f, 1.0f);
			Vec3 vAt = CubicBezier(vPoints[3], vPoints[4], vPoints[5], vPoints[6], fRatio);
			m_pTransformCom->LookAt(vAt);
			m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pNpc->Get_Stat()->fSpeed, fTimeDelta);
		}
		else if (fThreeDistance >= 4.0f && false == m_bSection[SECTION_THREE])
		{
			if (false == m_bSection[SECTION_TWO])
			{
				m_fTime = 0.0f;
				m_fTime += fTimeDelta;
				m_bSection[SECTION_TWO] = true;
			}

			_float fRatio = std::clamp(m_fTime / fArriveTime, 0.0f, 1.0f);
			Vec3 vAt = CubicBezier(vPoints[6], vPoints[7], vPoints[8], vPoints[9], fRatio);
			m_pTransformCom->LookAt(vAt);
			m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pNpc->Get_Stat()->fSpeed, fTimeDelta);
		}
		else if (fFourDistance >= 4.0f && false == m_bSection[SECTION_FOUR])
		{
			if (false == m_bSection[SECTION_THREE])
			{
				m_fTime = 0.0f;
				m_fTime += fTimeDelta;
				m_bSection[SECTION_THREE] = true;
			}

			_float fRatio = std::clamp(m_fTime / fArriveTime, 0.0f, 1.0f);
			Vec3 vAt = CubicBezier(vPoints[9], vPoints[10], vPoints[11], vPoints[12], fRatio);
			m_pTransformCom->LookAt(vAt);
			m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pNpc->Get_Stat()->fSpeed, fTimeDelta);
		}
		else if(fFiveDistance >= 4.0f && false == m_bSection[SECTION_FIVE])
		{
			if (false == m_bSection[SECTION_FOUR])
			{
				m_fTime = 0.0f;
				m_fTime += fTimeDelta;
				m_bSection[SECTION_FOUR] = true;
			}

			_float fRatio = std::clamp(m_fTime / fArriveTime, 0.0f, 1.0f);
			Vec3 vAt = CubicBezier(vPoints[12], vPoints[13], vPoints[14], vPoints[15], fRatio);
			m_pTransformCom->LookAt(vAt);
			m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pNpc->Get_Stat()->fSpeed, fTimeDelta);
		}
		else
		{
			if (false == m_bSection[SECTION_FIVE])
			{
				m_bSection[SECTION_FIVE] = true;
				CRuby* pRuby = static_cast<CRuby*>(m_pOwner);
				static_cast<CRubyCarriage*>(pRuby->Get_RidingObject())->Set_TakeTheCarriage(false);
				CStateMachine* pCarriageState = pRuby->Get_RidingObject()->Get_Component<CStateMachine>(TEXT("Com_StateMachine"));
				pCarriageState->Change_State(CRubyCarriage::STATE_IDLE);
			}
		}
	}
}

void CUniqueNpcState_Seat::Exit_State()
{
}

Vec3 CUniqueNpcState_Seat::CubicBezier(Vec4 p0, Vec4 p1, Vec4 p2, Vec4 p3, _float fTime)
{
	Vec3 v0 = Vec3(p0);
	Vec3 v1 = Vec3(p1);
	Vec3 v2 = Vec3(p2);
	Vec3 v3 = Vec3(p3);

	Vec3 q0 = Vec3::Lerp(v0, v1, fTime);
	Vec3 q1 = Vec3::Lerp(v1, v2, fTime);
	Vec3 q2 = Vec3::Lerp(v2, v3, fTime);

	Vec3 s1 = Vec3::Lerp(q0, q1, fTime);
	Vec3 s2 = Vec3::Lerp(q1, q2, fTime);

	
	return Vec3::Lerp(s1, s2, fTime);
}

CUniqueNpcState_Seat* CUniqueNpcState_Seat::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CUniqueNpcState_Seat* pInstance = new CUniqueNpcState_Seat(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CUniqueNpcState_Seat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUniqueNpcState_Seat::Free()
{
	__super::Free();
}

