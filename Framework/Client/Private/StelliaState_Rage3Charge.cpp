#include "stdafx.h"
#include "StelliaState_Rage3Charge.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "Effect.h"
#include "Particle_Manager.h"
#include "Particle.h"

#include "UI_Stellia_GaugeBar.h"

CStelliaState_Rage3Charge::CStelliaState_Rage3Charge(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3Charge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fSlepTime = 0.05f;
	m_fDecreaseTime = 0.4f;

	m_fLimitTime = 6.f;
	m_fShakeTime = 0.5f;
	m_iClickDest = 30;

	return S_OK;
}

void CStelliaState_Rage3Charge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Loop"));
	m_fAccSlepTime = 0.f;
	m_fAccClickTime = 0.f;
	m_fAccShakeTime = 0.5f;
	m_fAccDecreaseTime = 0.f;

	m_bIsStartEvent = false;
	m_bIsTimeSlep = false;
	m_bIsSlow = false;
	m_bIsFinishGuard = false;

	m_iClickPower = 0;

	iRealClickNum = 0;

	m_bNoGuardCollision = false;

	m_fAcc = 0.f;
	m_bPlaySound = false;
	_float fOffSet = ((Vec4)m_pTransformCom->Get_Position() - m_pStellia->Get_OriginPos()).Length();

	if (fOffSet >= 30.f)
	{
		m_bIsStartPosOut = true;
	}
	else
		m_bIsStartPosOut = false;

	// Effect Create
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Rage03Charge"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_Rage3Charge::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	// 거리 구하기
	_float fDist = ((Vec4)m_pTransformCom->Get_Position() - (Vec4)m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position()).Length();

	if (!m_bIsFinishGuard && fDist < 10.f && !m_bNoGuardCollision) // 충돌 발생 (거리로 측정)
	{
		if (!m_pStellia->Get_StelliaGaugeBar()->Get_Active())
			m_pStellia->Get_StelliaGaugeBar()->Set_Active(true);

		/* 플레이어가 가드 상태가 아니라면 */
		if (CCharacter::BATTLE_GUARD != CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_StateMachine()->Get_CurrState())
		{
			Player_KnockDown();

			m_bNoGuardCollision = true;
			return;
		}

		if (!m_bPlaySound)
		{
			m_fAcc += fTimeDelta;

			if (1.5f <= m_fAcc)
			{
				m_bPlaySound = true;
				CSound_Manager::GetInstance()->Play_Sound(TEXT("Stellia_Sv_Skill_6_St.ogg"), CHANNELID::SOUND_CUTSCENE, 0.7f, true);
			}
		}
		m_pTransformCom->LookAt_ForLandObject(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position());

		// 처음 한 번 호출.
		First_GuardEvent(fTimeDelta);

		if (false == m_bGuardEffect)
		{
			Create_GuardEffect();
			m_bGuardEffect = true;
		}

		m_pStellia->Get_TargetDesc().pTragetTransform->LookAt_ForLandObject(m_pTransformCom->Get_Position());

		m_fAccClickTime += fTimeDelta;
		m_fAccShakeTime += fTimeDelta;

		// 파훼 실패(시간 초과)
		if (m_fAccClickTime >= m_fLimitTime) // 막고있는 상태 해제
		{
			// ui 지우기
			m_pStellia->Get_StelliaGaugeBar()->Set_Active(false);

			Delete_GuardEffect();
			Create_ResultEffect(false);

			m_pPlayer->Get_CharacterStateCom()->Change_State(CCharacter::DAMAGED_KNOCKDOWN);

			Player_KnockDown();
		
			m_pStellia->Set_IsPlayerGuardEvent(false);
			m_bIsFinishGuard = true;

			CSound_Manager::GetInstance()->Play_Sound(TEXT("Impact_Ground_Crystal_1.ogg"), CHANNELID::SOUND_UI2, 1.f, true);


			/* 액션 캠 블렌딩 없이 종료 */
			if (CAMERA_TYPE::ACTION == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
				dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION))->Finish_Action_Stellia_Guard(false);
		}

		// 파훼 성공
		if (m_iClickPower >= m_iClickDest) // 막고있는 상태 해제 // 클릭 횟수
		{
			// ui 지우기
			m_pStellia->Get_StelliaGaugeBar()->Set_Active(false);

			Delete_GuardEffect();
			Create_ResultEffect(true);

			/* 액션 캠 블렌딩으로 종료 */
			if (CAMERA_TYPE::ACTION == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
				dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION))->Finish_Action_Stellia_Guard(true);

			CSound_Manager::GetInstance()->Play_Sound(TEXT("Impact_Ground_Crystal_1.ogg"), CHANNELID::SOUND_UI2, 1.f, true);


			if (m_iBreakCount < 1)
			{
				m_iBreakCount += 1;
				m_pStellia->Set_IsPlayerGuardEvent(false);
				m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3DAMAGE);
				return;
			}
			else
			{
				// 레이지 종료
				_float fStunTime = 15.f;
				m_pStateMachineCom->Change_State(CStellia::STELLIA_COUNTERSTART, &fStunTime);
				return;
			}
		}// 파훼 실패(중간에 가드 해제)
		else if (m_pPlayer->Get_CharacterStateCom()->Get_CurrState() != CCharacter::BATTLE_GUARD) // 막고있는 상태 해제
		{
			// ui 지우기
			m_pStellia->Get_StelliaGaugeBar()->Set_Active(false);

			Delete_GuardEffect();
			Create_ResultEffect(false);

			Player_KnockDown();

			m_pStellia->Set_IsPlayerGuardEvent(false);
			m_bIsFinishGuard = true;

			CSound_Manager::GetInstance()->Play_Sound(TEXT("Impact_Ground_Crystal_1.ogg"), CHANNELID::SOUND_UI2, 1.f, true);

			/* 액션 캠 블렌딩 없이 종료 */
			if (CAMERA_TYPE::ACTION == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
				dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION))->Finish_Action_Stellia_Guard(false);
		}

		// 버튼 클릭
		if (KEY_TAP(KEY::LBTN))
		{
			m_iClickPower += 1;

			iRealClickNum += 1;

			if(0 != iRealClickNum && 0 == iRealClickNum % 5)
				CSound_Manager::GetInstance()->Play_Sound(TEXT("Stellia_Guard_Click.mp3"), CHANNELID::SOUND_UI2, 1.f, true);
		}

		m_fAccDecreaseTime += fTimeDelta;
		if (m_fAccDecreaseTime >= m_fDecreaseTime)
		{
			if (m_iClickPower > 0)
			{
				m_iClickPower -= 1;
				m_fAccDecreaseTime = m_fDecreaseTime - m_fAccDecreaseTime;
			}
		}

		// ui
		m_pStellia->Get_StelliaGaugeBar()->Set_CurGauge(m_iClickPower);
	}
	else // 충돌 발생하지 않음 (거리로 측정)
	{
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRage3AroundSpeed, fTimeDelta);

		// 만약 타임 슬립 걸려 있다면
		if (m_bIsTimeSlep)
		{
			if (m_bIsSlow)
			{
				m_fAccSlepTime += fTimeDelta;
		
				if (m_fAccSlepTime >= m_fSlepTime)
				{
					m_bIsTimeSlep = false;
					m_bIsSlow = false;
					GI->Set_TimeScale(TIMER_TYPE::GAME_PLAY, 1.f);
				}
			}
		}
	}

	if (!m_bIsStartEvent && !m_bNoGuardCollision)
	{
		m_pTransformCom->LookAt_ForLandObject(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position());
	}


	// 다음 상태 넘어가기 위한 검사 함수.
	Check_RangeOut();
}

void CStelliaState_Rage3Charge::Exit_State()
{
	Delete_GuardEffect();
}

void CStelliaState_Rage3Charge::First_GuardEvent(_float fTimeDelta)
{
	if (!m_bIsStartEvent)
	{
		m_pStellia->Get_TargetDesc().pTragetTransform->LookAt_ForLandObject(m_pTransformCom->Get_Position());
		Vec3 vPlayerLook = m_pStellia->Get_TargetDesc().pTarget->Get_Component_Transform()->Get_Look();
		m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(-vPlayerLook, 20.f, true);

		GI->Set_TimeScale(TIMER_TYPE::GAME_PLAY, 0.1f);

		m_bIsTimeSlep = true;
		m_bIsSlow = true;
		m_bIsStartEvent = true;

		// 액션캠 카메라 블렌딩 시작
		dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION))
			->Start_Action_Stellia_Guard(m_pStellia->Get_Component_Transform());

		CSound_Manager::GetInstance()->Play_Sound(TEXT("Impact_Impulse_1.mp3"), CHANNELID::SOUND_UI2, 1.f, true);

	}

	if (m_bIsTimeSlep)
	{
		if (m_bIsSlow)
		{
			m_fAccSlepTime += fTimeDelta;

			if (m_fAccSlepTime >= m_fSlepTime)
			{
				m_bIsTimeSlep = false;
				m_bIsSlow = false;
				GI->Set_TimeScale(TIMER_TYPE::GAME_PLAY, 1.f);

				m_pStellia->Get_TargetDesc().pTragetTransform->Set_Position(m_pTransformCom->Get_Position() + m_pTransformCom->Get_Look() * 3.f);

				// ui 띄우기
				m_pStellia->Get_StelliaGaugeBar()->Set_MaxGauge(30);
			}
		}
	}
}

void CStelliaState_Rage3Charge::Player_KnockDown()
{
	/* 플레이어와 스텔리아의 위치를 반별하여 플레이어를 날릴 방향 결정 */
	//_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	//Vec4 vDest = m_pStellia->Get_OriginPos();
	//vDest -= m_pTransformCom->Get_Position();
	//_vector vDestNormal = XMVector3Normalize(vDest);
	//
	//_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	//_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* 보스의 look을 기준으로 플레이어가 왼쪽, 오른쪽에 위치하는지를 판별. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* 보스가 바라보는 방향을 기준으로 오른쪽에 위치. */
	if (fCrossProductY > 0.f)
	{
		Vec3 vDir = m_pTransformCom->Get_Right() + m_pTransformCom->Get_Up() * 0.5f;
		vDir.Normalize();

		dynamic_cast<CCharacter*>(m_pStellia->Get_TargetDesc().pTarget)->Get_CharacterStateCom()->Change_State(CCharacter::DAMAGED_KNOCKDOWN);
		m_pStellia->Get_TargetDesc().pTarget->Get_Component_Rigidbody()->Add_Velocity(vDir, 15.f, true);
	}
	/* 보스가 바라보는 방향을 기준으로 왼쪽에 위치. */
	else 
	{
		Vec3 vDir = m_pTransformCom->Get_Left() + m_pTransformCom->Get_Up() * 0.5f;
		vDir.Normalize();

		dynamic_cast<CCharacter*>(m_pStellia->Get_TargetDesc().pTarget)->Get_CharacterStateCom()->Change_State(CCharacter::DAMAGED_KNOCKDOWN);
		m_pStellia->Get_TargetDesc().pTarget->Get_Component_Rigidbody()->Add_Velocity(vDir, 15.f, true);
	}

	/* 플레이어 데미지 주기 */
	const _float fDeltaHp = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Stat().iMaxHp * 0.1f;
	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Decrease_HP(fDeltaHp);
}

void CStelliaState_Rage3Charge::Check_RangeOut()
{
	Vec4 vCenterToStellia = m_pStellia->Get_OriginPos() - (Vec4)m_pTransformCom->Get_Position();

	if (m_bIsStartPosOut)
	{
		if (vCenterToStellia.Length() < m_fAroundDist - 1.f)
		{
			m_bIsStartPosOut = false;
		}
	}
	// 스텔리아가 AroundDist에 도달한다면 Break.
	if (!m_bIsStartPosOut && vCenterToStellia.Length() >= m_fAroundDist)
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3CHARGE_BREAK);
	}
}

void CStelliaState_Rage3Charge::Create_GuardEffect()
{
	// 레디얼 블러 활성화
	CGame_Manager::GetInstance()->Lerp_RadialBlur(true, true, 0.f, -0.07f, 0.6f, 16.f);

	_matrix WorldMatrix = m_pStellia->Get_TargetDesc().pTragetTransform->Get_WorldMatrix();

	Vec4 vStelliaPosition = m_pTransformCom->Get_Position();
	vStelliaPosition.y += 5.f;
	Vec4 vPlayerPosition = m_pStellia->Get_TargetDesc().pTragetTransform->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = vStelliaPosition - vPlayerPosition;
	vLook.Normalize();
	Vec3 vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	vRight.Normalize();
	Vec3 vUp = XMVector3Cross(vLook, vRight);
	vUp.Normalize();

	_float3	vScale = m_pStellia->Get_TargetDesc().pTragetTransform->Get_Scale();
	WorldMatrix.r[CTransform::STATE_RIGHT] = vRight * vScale.x;
	WorldMatrix.r[CTransform::STATE_UP] = vUp * vScale.y;
	WorldMatrix.r[CTransform::STATE_LOOK] = vLook * vScale.z;

	CGameObject* pPlayer = m_pStellia->Get_TargetDesc().pTarget;

	if (pPlayer == nullptr)
		return;

	if (nullptr == m_pPlayerGuard)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Skill_Rage03Charge_FrontLine_Player"),
			WorldMatrix, _float3(0.f, 0.1f, -0.1f), _float3(10.f, 10.f, 15.f), _float3(0.f, 0.f, 0.f), pPlayer, &m_pPlayerGuard, false);
		Safe_AddRef(m_pPlayerGuard);
	}

	if (nullptr == m_pSpark_Player)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Rage03Charge_FrontLine_Player_Circle"),
			WorldMatrix, _float3(0.f, 1.1f, 2.2f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), pPlayer, &m_pSpark_Player, false);
		Safe_AddRef(m_pSpark_Player);
	}

	if (nullptr == m_pSpark_Monster)
	{
		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), -vLook);
		vRight.Normalize();
		vUp = XMVector3Cross(-vLook, vRight);
		vUp.Normalize();
		WorldMatrix.r[CTransform::STATE_RIGHT] = vRight * vScale.x;
		WorldMatrix.r[CTransform::STATE_UP] = vUp * vScale.y;
		WorldMatrix.r[CTransform::STATE_LOOK] = -vLook * vScale.z;

		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Rage03Charge_FrontLine_Stellia_Circle"),
			WorldMatrix, _float3(0.f, 1.1f, -1.8f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), pPlayer, &m_pSpark_Monster, false);
		Safe_AddRef(m_pSpark_Monster);
	}
}

void CStelliaState_Rage3Charge::Delete_GuardEffect()
{
	// 레디얼 블러 비활성화
	if (true == m_bGuardEffect)
	{
		CGame_Manager::GetInstance()->Lerp_RadialBlur(false, false, -0.1f, 0.f, 0.3f, 16.f);
		m_bGuardEffect = false;
	}

	if (nullptr != m_pPlayerGuard)
	{
		m_pPlayerGuard->Set_Dead(true);
		Safe_Release(m_pPlayerGuard);
	}
	if (nullptr != m_pSpark_Player)
	{
		m_pSpark_Player->Set_Dead(true);
		Safe_Release(m_pSpark_Player);
	}
	if (nullptr != m_pSpark_Monster)
	{
		m_pSpark_Monster->Set_Dead(true);
		Safe_Release(m_pSpark_Monster);
	}
}

void CStelliaState_Rage3Charge::Create_ResultEffect(_bool bSuccess)
{
	// 레디얼 블러 비활성화
	if (true == m_bGuardEffect)
	{
		CGame_Manager::GetInstance()->Lerp_RadialBlur(false, false, -0.1f, 0.f, 0.3f, 16.f);
		m_bGuardEffect = false;
	}

	_matrix WorldMatrix = m_pStellia->Get_TargetDesc().pTragetTransform->Get_WorldMatrix();

	Vec4 vStelliaPosition = m_pTransformCom->Get_Position();
	vStelliaPosition.y += 5.f;
	Vec4 vPlayerPosition = m_pStellia->Get_TargetDesc().pTragetTransform->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = vStelliaPosition - vPlayerPosition;
	vLook.Normalize();
	Vec3 vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	vRight.Normalize();
	Vec3 vUp = XMVector3Cross(vLook, vRight);
	vUp.Normalize();

	_float3	vScale = m_pStellia->Get_TargetDesc().pTragetTransform->Get_Scale();
	WorldMatrix.r[CTransform::STATE_RIGHT] = vRight * vScale.x;
	WorldMatrix.r[CTransform::STATE_UP] = vUp * vScale.y;
	WorldMatrix.r[CTransform::STATE_LOOK] = vLook * vScale.z;

	// 성공
	if (bSuccess)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Rage03Charge_Success"),
			WorldMatrix, _float3(0.f, 1.1f, 0.2f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
	}
	// 실패
	else
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Rage03Charge_Fail"),
			WorldMatrix, _float3(0.f, 1.1f, 0.2f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
	}
}

CStelliaState_Rage3Charge* CStelliaState_Rage3Charge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3Charge* pInstance = new CStelliaState_Rage3Charge(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3Charge::Free()
{
	__super::Free();

	if (nullptr != m_pPlayerGuard)
	{
		m_pPlayerGuard->Set_Dead(true);
		Safe_Release(m_pPlayerGuard);
	}
	if (nullptr != m_pSpark_Player)
	{
		m_pSpark_Player->Set_Dead(true);
		Safe_Release(m_pSpark_Player);
	}
	if (nullptr != m_pSpark_Monster)
	{
		m_pSpark_Monster->Set_Dead(true);
		Safe_Release(m_pSpark_Monster);
	}
}
