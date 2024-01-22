#include "stdafx.h"
#include "StelliaState_Rage1Loop_Explosion.h"

#include "Stellia.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

CStelliaState_Rage1Loop_Explosion::CStelliaState_Rage1Loop_Explosion(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage1Loop_Explosion::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fRotAngle = 15.f;

	return S_OK;
}

void CStelliaState_Rage1Loop_Explosion::Enter_State(void* pArg)
{
	m_vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
	
	m_vLeftRot = XMVector3Rotate(m_vLook, XMQuaternionRotationRollPitchYaw(0.0f, -m_fRotAngle, 0.0f));
	m_vRightRot = XMVector3Rotate(m_vLook, XMQuaternionRotationRollPitchYaw(0.0f, m_fRotAngle, 0.0f));

	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill04"));

	m_pStellia->Set_StelliaHit(false);
}

void CStelliaState_Rage1Loop_Explosion::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	__super::Rage1_Tick(fTimeDelta);

	Vec4 vDirToPlayer = m_pPlayerTransform->Get_Position() - m_pTransformCom->Get_Position();

	// 스텔리아의 룩과 플레이어의 현재 위치의 라디안 각도.
	_float fAngleToPlayer = XMVectorGetX(XMVector3AngleBetweenNormals(m_vLook, XMVector3Normalize(vDirToPlayer)));

	if (m_pModelCom->Get_CurrAnimationFrame() >= 65 && m_pModelCom->Get_CurrAnimationFrame() <= 70)
	{
		// 플레이어가 안전지대 범위 내에 없다.
		if (XMConvertToDegrees(fAngleToPlayer) >= m_fRotAngle)
		{
			if (CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Hp() > 0)
			{
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Decrease_HP(999);
			}
		}
	}

	// 안전 범위 내에 있을 때에만 플레이어가 스텔리아 공격 가능
	if (XMConvertToDegrees(fAngleToPlayer) < m_fRotAngle)
		m_pStellia->Set_StelliaHit(true);
	else
		m_pStellia->Set_StelliaHit(false);

	//
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		CStellia::STELLIA_STATE eNextState = CStellia::STELLIA_RAGE1LOOP_TURN;
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1LOOP_IDLE, &eNextState);
	}
}

void CStelliaState_Rage1Loop_Explosion::Exit_State()
{
}

CStelliaState_Rage1Loop_Explosion* CStelliaState_Rage1Loop_Explosion::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage1Loop_Explosion* pInstance = new CStelliaState_Rage1Loop_Explosion(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage1Loop_Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage1Loop_Explosion::Free()
{
	__super::Free();
}

