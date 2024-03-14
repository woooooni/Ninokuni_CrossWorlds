#include "stdafx.h"
#include "StelliaState_SpawnStand.h"

#include "Stellia.h"

#include "GameInstance.h"
#include "Vfx.h"
#include "Decal.h"

CStelliaState_SpawnStand::CStelliaState_SpawnStand(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_SpawnStand::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_SpawnStand::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_TurnStand"));
}

void CStelliaState_SpawnStand::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
}

void CStelliaState_SpawnStand::Exit_State()
{
	/* 마법진 데칼 지우기 */
	CGameObject* pGameObject = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_EFFECT, L"Decal_Stellia_Spawn_MagicCircle");
	if (nullptr != pGameObject)
	{
		CDecal* pDecal = dynamic_cast<CDecal*>(pGameObject);
		if (nullptr != pDecal)
		{
			pDecal->Start_AlphaDeleate();
			//Safe_Release(pDecal);
		}
	}
}

CStelliaState_SpawnStand* CStelliaState_SpawnStand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_SpawnStand* pInstance = new CStelliaState_SpawnStand(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_SpawnStand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_SpawnStand::Free()
{
	__super::Free();
}

