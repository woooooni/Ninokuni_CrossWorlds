#include "stdafx.h"
#include "StelliaState_Rage2Loop.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "Effect_Manager.h"
#include "Effect.h"
#include "Stellia_Crystal.h"

CStelliaState_Rage2Loop::CStelliaState_Rage2Loop(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage2Loop::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fRespawnTime = 4.f;

	return S_OK;
}

void CStelliaState_Rage2Loop::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_TurnStand"));

	m_pStellia->Set_StelliaHit(false);

	if (nullptr == m_pShieldEffect)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Shield"),
			m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.05f, 0.f), _float3(38.f, 38.f, 38.f), _float3(0.f, 0.f, 0.f),
			m_pStellia, &m_pShieldEffect, false);
		if (nullptr != m_pShieldEffect)
		{
			Safe_AddRef(m_pShieldEffect);
			Vec3 vCreateColor = { 0.760f, 0.611f, 1.f };

			// 방어막 색상 변경
			CGameObject* pGameObject = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_PROP, L"Stellia_Crystal");
			if (nullptr != pGameObject)
			{
				CStellia_Crystal* pOriginalCrystal = dynamic_cast<CStellia_Crystal*>(pGameObject);
				if (nullptr != pOriginalCrystal)
				{
					Vec3 vColor = {};
					switch (pOriginalCrystal->Get_CrystalType())
					{
					case 0: // CRYSTAL_AURA
						vCreateColor   = Vec3(0.362f, 0.770f, 1.f);
						vColor         = Vec3(0.212f, 0.620f, 1.f);
						m_vDeleteColor = Vec3(0.062f, 0.470f, 0.85f);
						break;
					case 1: // CRYSTAL_SKY
						vCreateColor   = Vec3(0.865f, 1.f, 1.f);
						vColor         = Vec3(0.715f, 0.995f, 1.f);
						m_vDeleteColor = Vec3(0.565f, 0.845f, 0.85f);
						break;
					case 2: // CRYSTAL_GOLD
						vCreateColor   = Vec3(1.f, 1.f, 0.616f);
						vColor         = Vec3(1.f, 0.96f, 0.466f);
						m_vDeleteColor = Vec3(0.85f, 0.81f, 0.316f);
						break;
					}

					m_pShieldEffect->Set_Color(vColor);
				}
			}

			m_pShieldEffect->Create_Dissolve(73,                             // Index
				_float4(vCreateColor.x, vCreateColor.y, vCreateColor.z, 1.f),// Color
				5.f,   // Speed
				10.f); // Total;
		}
	}
}

void CStelliaState_Rage2Loop::Tick_State(_float fTimeDelta)
{
	//m_fTime += fTimeDelta;

	//if (m_fTime >= m_fRespawnTime)
	//{
	//	m_fTime = m_fRespawnTime - m_fTime;
	//	__super::Generate_Explosion(5);
	//}

	//if (m_pModelCom->Get_CurrAnimationFrame() >= 45 && !m_pModelCom->Is_Tween())
	//{
	//	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Start"));
	//}
}

void CStelliaState_Rage2Loop::Exit_State()
{
	if (nullptr != m_pShieldEffect)
	{
		m_pShieldEffect->Start_Dissolve(73,   // Index
			_float4(m_vDeleteColor.x, m_vDeleteColor.y, m_vDeleteColor.z, 1.f),// Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pShieldEffect);
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

	if (nullptr != m_pShieldEffect)
	{
		m_pShieldEffect->Start_Dissolve(73,   // Index
			_float4(m_vDeleteColor.x, m_vDeleteColor.y, m_vDeleteColor.z, 1.f),// Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pShieldEffect);
	}
}
