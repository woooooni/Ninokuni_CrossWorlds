#include "stdafx.h"
#include "StelliaState_Rage2Loop.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"


CStelliaState_Rage2Loop::CStelliaState_Rage2Loop(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage2Loop::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fRespawnTime = 1.f;

	return S_OK;
}

void CStelliaState_Rage2Loop::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Start"));

}

void CStelliaState_Rage2Loop::Tick_State(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime >= m_fRespawnTime)
	{
		m_fTime = m_fRespawnTime - m_fTime;
		Generate_Explosion(2);
	}

	if (m_pModelCom->Get_CurrAnimationFrame() >= 45 && !m_pModelCom->Is_Tween())
	{
		m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Start"));
	}
}

void CStelliaState_Rage2Loop::Exit_State()
{
}

void CStelliaState_Rage2Loop::Generate_Explosion(_uint iCount)
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
		vExplosionPos.y = m_pStellia->Get_OriginPos().y - 1.5f; 
		vExplosionPos.z += GI->RandomFloat(-10.f, 10.f);

		CGameObject* pExplosion = nullptr;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Stellia_Explosion", m_pStellia, &pExplosion)))
		{
			MSG_BOX("Add Icicle Failed.");
			return;
		}

		CTransform* pIcicleTransform = pExplosion->Get_Component<CTransform>(L"Com_Transform");

		pIcicleTransform->Set_State(CTransform::STATE_POSITION, vExplosionPos);
	}
}

CStelliaState_Rage2Loop* CStelliaState_Rage2Loop::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage2Loop* pInstance = new CStelliaState_Rage2Loop(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage2Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage2Loop::Free()
{
	__super::Free();
}
