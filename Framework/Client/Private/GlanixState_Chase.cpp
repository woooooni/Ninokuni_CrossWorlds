#include "stdafx.h"
#include "GlanixState_Chase.h"

#include "Glanix.h"

CGlanixState_Chase::CGlanixState_Chase(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Chase::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Chase::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Battlerun"));
}

void CGlanixState_Chase::Tick_State(_float fTimeDelta)
{
	if (m_pPlayer != nullptr)
		m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());
	
	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRunSpeed, fTimeDelta);

	if (m_pGlanix->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
	{
		if (m_iAtkIndex >= m_vecAtkState.size())
			m_iAtkIndex = 0;

		m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
	}
}

void CGlanixState_Chase::Exit_State()
{
}

CGlanixState_Chase* CGlanixState_Chase::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Chase* pInstance = new CGlanixState_Chase(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Chase::Free()
{
	__super::Free();
}

