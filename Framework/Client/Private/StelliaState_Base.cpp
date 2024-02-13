#include "stdafx.h"
#include "StelliaState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Stellia.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Particle_Manager.h"
#include "Decal.h"

#include "Camera_Group.h"

_uint CStelliaState_Base::m_iAtkIndex = 0;

CStelliaState_Base::CStelliaState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CStelliaState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_pStellia = dynamic_cast<CStellia*>(m_pOwner);

	m_pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	m_fRunSpeed = 6.f;
	m_fSprintSpeed = 12.f;
	m_fRage3AroundSpeed = 40.f;

	m_fAroundDist = 30.f;


	/* 공격 패턴만 따로 모아놓기. (후에 순차적 혹은 랜덤으로 전환하기 위해) */
	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK1);
	m_vecAtkState.push_back(CStellia::STELLIA_SPINTAIL);
	m_vecAtkState.push_back(CStellia::STELLIA_JUMPSTAMP);
	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK2);
	m_vecAtkState.push_back(CStellia::STELLIA_CHARGE);
	m_vecAtkState.push_back(CStellia::STELLIA_LASER);

	return S_OK;
}

void CStelliaState_Base::Enter_State(void* pArg)
{
}

void CStelliaState_Base::Tick_State(_float fTimeDelta)
{
	/* Dead */
	if (m_pStellia->Get_Stat().fHp <= 0.f)
		m_pStateMachineCom->Change_State(CStellia::STELLIA_DEAD);
}

void CStelliaState_Base::Exit_State()
{
}

void CStelliaState_Base::Init_Pattern()
{
	m_vecAtkState.clear();

	m_vecAtkState.push_back(CStellia::STELLIA_JUMPSTAMP);
	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK1);
	m_vecAtkState.push_back(CStellia::STELLIA_TRIPLELASER);
	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK2);
	m_vecAtkState.push_back(CStellia::STELLIA_CHARGE);
	m_vecAtkState.push_back(CStellia::STELLIA_BIGBANG);
	m_vecAtkState.push_back(CStellia::STELLIA_SPINTAIL);

}

_bool CStelliaState_Base::State_Wait(_float fDestTime, _float fTimeDelta)
{
	/* 기안티와 다르게 루프 애니메이션이 아니기에 종료 시점으로 처리. */
	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() != TEXT("SKM_Stellia.ao|Stellia_Stand02"))
	{
		m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Stand02"));
	}

	m_fTime += fTimeDelta;

	if (m_fTime >= fDestTime)
	{
		m_fTime = fDestTime - m_fTime;
		return true;
	}

	return false;
}

void CStelliaState_Base::Rage1_Tick(_float fTimeDelta)
{
	if (m_pStellia->Get_Bools(CStellia::BOSS_BOOLTYPE::BOSSBOOL_RAGE))
	{
		if (m_pStellia->Get_AccDamage() >= m_pStellia->Get_DestDamage())
		{
			_float fStunTime = 15.f;
			m_pStellia->Reset_RageAccDamage();
			m_pStellia->Set_Bools(CStellia::BOSS_BOOLTYPE::BOSSBOOL_RAGE, false);
			m_pStateMachineCom->Change_State(CStellia::STELLIA_COUNTERSTART, &fStunTime);
		}
	}
}

void CStelliaState_Base::Generate_Explosion(_uint iCount)
{
	CTransform* pTransform = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
	{
		MSG_BOX("Transform Get Failed.");
		return;
	}

	for (_uint i = 0; i < iCount; ++i)
	{
		Vec4 vExplosionPos = pTransform->Get_Position();
		vExplosionPos.x += GI->RandomFloat(-10.f, 10.f);
		vExplosionPos.y = m_pStellia->Get_OriginPos().y + 0.5f;
		vExplosionPos.z += GI->RandomFloat(-10.f, 10.f);

		CGameObject* pExplosion = nullptr;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Stellia_Explosion", m_pStellia, &pExplosion)))
		{
			MSG_BOX("Add Explosion Failed.");
			return;
		}

		CTransform* pIcicleTransform = pExplosion->Get_Component<CTransform>(L"Com_Transform");

		pIcicleTransform->Set_State(CTransform::STATE_POSITION, vExplosionPos);
	}
}


void CStelliaState_Base::Reset_Transform()
{
	// 스텔리아의 현재 트랜스폼을 초기 생성 포지션과 룩으로 되돌림

	Vec4 vOriginPos = m_pStellia->Get_OriginPos();
	vOriginPos.y -= 2.5f; /* 공중에서 떨어지지 않도록 높이를 최대한 낮춤 */

	m_pTransformCom->Set_Position(vOriginPos);
	m_pTransformCom->Set_LookAtByDir(m_pStellia->Get_OriginLook().ZeroY().ZeroW().Normalized());
}

void CStelliaState_Base::Set_LockOnStellia()
{
	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	if (nullptr != pFollowCam)
	{
		/* 기존 세팅 초기화 */
		{
			pFollowCam->Reset_WideView_To_DefaultView(true);
			pFollowCam->Set_Default_Position();
		}

		/* 락온 설정 */
		{
			pFollowCam->Set_LockBoneNumber(3);

			CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_MONSTER, L"Stellia");
			if (nullptr != pTarget)
				pFollowCam->Start_LockOn(pTarget, Cam_Target_Offset_LockOn_Stellia, Cam_LookAt_Offset_LockOn_Stellia);
		}
	}
}

void CStelliaState_Base::Set_LockOffStellia()
{
	// 카메라 블렌딩 시작 
	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));

	if (nullptr != pFollowCam)
	{
		/* 락온이었다면 해제 */
		if (pFollowCam->Is_LockOn())
			pFollowCam->Finish_LockOn(CGame_Manager::GetInstance()->Get_Player()->Get_Character());

		pFollowCam->Reset_WideView_To_DefaultView(true);
		pFollowCam->Set_Default_Position();
	}
}

void CStelliaState_Base::Free()
{
	__super::Free();
}
