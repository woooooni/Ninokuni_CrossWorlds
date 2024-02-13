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

CStelliaState_Rage3Charge::CStelliaState_Rage3Charge(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3Charge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fLimitTime = 6.f;
	m_fShakeTime = 0.5f;
	m_iClickDest = 30.f;
	m_fMinChargeLength = 5.f;

	return S_OK;
}

void CStelliaState_Rage3Charge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Loop"));
	m_fAccClickTime = 0.f;
	m_fAccShakeTime = 0.5f;

	m_iClickPower = 0;

	m_vStartPos = m_pTransformCom->Get_Position();

	// Effect Create
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Rage03Charge"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_Rage3Charge::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pStellia->Get_IsPlayerGuardEvent()) // 플레이어가 현재 막고있는 상태
	{
		// Effect Create ----------------------------
		if (nullptr == m_pPlayerGuard)
		{
			_matrix WorldMatrix = m_pStellia->Get_TargetDesc().pTragetTransform->Get_WorldMatrix();

			// 몬스터 - 플레이어
			Vec4 vStelliaPosition = m_pTransformCom->Get_Position();
			Vec4 vPlayerPosition  = m_pStellia->Get_TargetDesc().pTragetTransform->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = vStelliaPosition - vPlayerPosition;
			vLook.Normalize();
			Vec3 vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
			vRight.Normalize();
			Vec3 vUp = XMVector3Cross(vLook, vRight);
			vUp.Normalize();

			_float3	vScale = m_pStellia->Get_TargetDesc().pTragetTransform->Get_Scale();
			WorldMatrix.r[CTransform::STATE_RIGHT] = vRight * vScale.x;
			WorldMatrix.r[CTransform::STATE_UP]    = vUp    * vScale.y;
			WorldMatrix.r[CTransform::STATE_LOOK]  = vLook  * vScale.z;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Skill_Rage03Charge_FrontLine_Player"),
				WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(10.f, 10.f, 10.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pPlayerGuard, false);
			Safe_AddRef(m_pPlayerGuard);
		}
		// ---------------------------- Effect Create

		m_pStellia->Get_TargetDesc().pTragetTransform->LookAt_ForLandObject(m_pTransformCom->Get_Position());
		m_pStellia->Get_TargetDesc().pTragetTransform->Move(-m_pStellia->Get_TargetDesc().pTragetTransform->Get_Look(), 3.f, fTimeDelta);

		m_fAccClickTime += fTimeDelta;
		m_fAccShakeTime += fTimeDelta;

		//if (m_fAccShakeTime >= m_fShakeTime)
		//{
			CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
		//	m_fAccShakeTime = m_fShakeTime - m_fAccShakeTime;
		//}

		// 파훼 실패(시간 초과)
		if (m_fAccClickTime >= m_fLimitTime) // 막고있는 상태 해제
		{
			if (nullptr != m_pPlayerGuard)
			{
				m_pPlayerGuard->Set_Dead(true);
				Safe_Release(m_pPlayerGuard);
			}

			m_pPlayer->Get_CharacterStateCom()->Change_State(CCharacter::DAMAGED_KNOCKDOWN);
			
			/* 플레이어와 스텔리아의 위치를 반별하여 플레이어를 날릴 방향 결정 */
			_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
			Vec4 vDest = m_pStellia->Get_OriginPos();
			vDest -= m_pTransformCom->Get_Position();
			_vector vDestNormal = XMVector3Normalize(vDest);

			_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
			_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

			/* 보스의 look을 기준으로 센터가 왼쪽, 오른쪽에 위치하는지를 판별. */
			_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
			_float fCrossProductY = XMVectorGetY(vCrossProduct);

			/* 보스가 바라보는 방향을 기준으로 오른쪽에 위치. */
			if (fCrossProductY > 0.f)
			{
				Vec3 m_vRightRot = XMVector3Rotate(vLookNormal, XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(45.f), 0.0f));

				m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(m_vRightRot, 10.f, false);
				m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity({ 0.f, 1.f, 0.f }, 10.f, false);
			}
			/* 보스가 바라보는 방향을 기준으로 왼쪽에 위치. */
			else if (fCrossProductY < 0.f)
			{
				Vec3 m_vLeftRot = XMVector3Rotate(vLookNormal, XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(-45.f), 0.0f));

				m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(m_vLeftRot, 10.f, false);
				m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity({ 0.f, 1.f, 0.f }, 10.f, false);
			}

			m_pStellia->Set_IsPlayerGuardEvent(false);
		}

		// 파훼 성공
		if (m_iClickPower >= m_iClickDest) // 막고있는 상태 해제
		{
			if (nullptr != m_pPlayerGuard)
			{
				m_pPlayerGuard->Set_Dead(true);
				Safe_Release(m_pPlayerGuard);
			}

			if (m_iBreakCount < 2)
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
		}

		// 파훼 실패(중간에 가드 해제)
		else if (m_pPlayer->Get_CharacterStateCom()->Get_CurrState() != CCharacter::BATTLE_GUARD) // 막고있는 상태 해제
		{
			if (nullptr != m_pPlayerGuard)
			{
				m_pPlayerGuard->Set_Dead(true);
				Safe_Release(m_pPlayerGuard);
			}

			dynamic_cast<CCharacter*>(m_pStellia->Get_TargetDesc().pTarget)->Get_CharacterStateCom()->Change_State(CCharacter::DAMAGED_KNOCKDOWN);
			m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
				-m_pStellia->Get_TargetDesc().pTarget->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_Look(), 10.f, false);

			m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
				{ 0.f, 1.f, 0.f }, 10.f, false);

			m_pStellia->Set_IsPlayerGuardEvent(false);
		}

		// 버튼 클릭
		if (KEY_TAP(KEY::LBTN))
		{
			CSound_Manager::GetInstance()->Play_Sound(TEXT("Impact_Metal_Bell_1_1.ogg"), CHANNELID::SOUND_UI2, 1.f, true);
			m_iClickPower += 1;
		}
	}
	else
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRage3AroundSpeed, fTimeDelta);

	// 최소 질주 거리 계산(시작하자마자 브레이크 밟는거 방지)
	Vec4 vCurPos = (Vec4)m_pTransformCom->Get_Position() - m_vStartPos;
	m_fCurChargeLength = fabs(vCurPos.Length());

	// 스텔리아가 AroundDist에 도달한다면 Turn.
	Vec4 vCenterToStellia = m_pStellia->Get_OriginPos() - (Vec4)m_pTransformCom->Get_Position();
	if (fabs(vCenterToStellia.Length()) > m_fAroundDist && m_fCurChargeLength > m_fMinChargeLength)
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3CHARGE_BREAK);
	}
}

void CStelliaState_Rage3Charge::Exit_State()
{
	if (nullptr != m_pPlayerGuard)
	{
		m_pPlayerGuard->Set_Dead(true);
		Safe_Release(m_pPlayerGuard);
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
}
