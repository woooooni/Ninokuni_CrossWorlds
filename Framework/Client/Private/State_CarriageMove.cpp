#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\State_CarriageMove.h"
#include "Player.h"
#include "Game_Manager.h"
#include "WitchWood.h"

#include "Ruby.h"
#include "RubyCarriage.h"
#include "Particle_Manager.h"

CState_CarriageMove::CState_CarriageMove(CStateMachine* pMachine)
	: CState(pMachine)
{

}

HRESULT CState_CarriageMove::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_CarriageMove::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);

	if (false == m_bFirst)
	{
		m_bFirst = true;
		
		// LEVEL
		_uint iCurLevel = GI->Get_CurrentLevel();
		m_pRuby = static_cast<CRuby*>(GI->Find_GameObject(iCurLevel, LAYER_TYPE::LAYER_NPC, TEXT("Ruby")));
	}

	_float fCamDistance = m_pModelCom->Get_Owner()->Get_CamDistance();
	GI->Play_Sound(TEXT("Witch_Carriage.mp3"), CHANNELID::SOUND_VOICE_WITCH_QUEST, 1.0f, false, fCamDistance);
}

void CState_CarriageMove::Tick_State(_float fTimeDelta)
{
	// TODO
	// 루비의 애니메이션이 바뀌고 루비를 따라다녀야한다.
	CParticle_Manager::GetInstance()->Tick_Generate_Particle_To_Matrix(&m_fTimeEffect[0], 0.5f, fTimeDelta, TEXT("Particle_Carriage_Stone"), m_pTransformCom->Get_WorldMatrix(),
		Vec3(-1.1f, 0.0f, -2.7f), Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
	CParticle_Manager::GetInstance()->Tick_Generate_Particle_To_Matrix(&m_fTimeEffect[1], 0.5f, fTimeDelta, TEXT("Particle_Carriage_Stone"), m_pTransformCom->Get_WorldMatrix(),
		Vec3(1.1f, 0.0f, -2.7f), Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));

	if (nullptr != m_pRuby)
	{
		_bool bTake = static_cast<CRubyCarriage*>(m_pOwner)->TakeTheCarriage();
		CTransform* pTransform = m_pRuby->Get_Component<CTransform>(TEXT("Com_Transform"));
		Matrix RubyWorld = pTransform->Get_WorldMatrix();

		if (true == bTake)
			m_pTransformCom->Set_WorldMatrix(RubyWorld);

		if (true == m_pRuby->Get_QuestSection(CRuby::ESCORT_SECTION::SECTION1) || true == m_pRuby->Get_QuestSection(CRuby::ESCORT_SECTION::SECTION2))
		{
			if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
				m_pStateMachineCom->Change_State(CRubyCarriage::STATE_IDLE);
		}
		else if (false == bTake)
		{
			if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
				m_pStateMachineCom->Change_State(CRubyCarriage::STATE_IDLE);
		}
	}
}

void CState_CarriageMove::Exit_State()
{
	m_fTime = 0.0f;

}

CState_CarriageMove* CState_CarriageMove::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_CarriageMove* pInstance = new CState_CarriageMove(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_CarriageMove");
		Safe_Release<CState_CarriageMove*>(pInstance);
	}

	return pInstance;
}

void CState_CarriageMove::Free()
{
	__super::Free();
}
