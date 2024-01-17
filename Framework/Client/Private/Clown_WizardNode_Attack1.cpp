#include "stdafx.h"
#include "Clown_WizardNode_Attack1.h"

#include "GameInstance.h"

#include "BehaviorTree.h"
#include "Model.h"

CClown_WizardNode_Attack1::CClown_WizardNode_Attack1()
{
}

HRESULT CClown_WizardNode_Attack1::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CClown_WizardNode_Attack1::Start()
{
	m_bIsAtk = false;
	m_bIsFire = false;

	m_fLength = Vec4(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position()
		- m_tBTMonsterDesc.pOwnerTransform->Get_Position()).Length();

	if (m_fLength > 2.f)
	{
		m_bIsAtk = true;
		m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ClownWizard.ao|ClownWizard_Attack01"));
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
	}
}

CBTNode::NODE_STATE CClown_WizardNode_Attack1::Tick(const _float& fTimeDelta)
{
	if (!m_bIsAtk)
		return NODE_STATE::NODE_SUCCESS;
	else
	{
		if (m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_ClownWizard.ao|ClownWizard_Attack01"))
		{
			if (!m_bIsFire && m_tBTMonsterDesc.pOwnerModel->Get_CurrAnimationFrame() == 25)
			{
				m_bIsFire = true;
				GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Clown_Wizard_DarkBall"), m_tBTMonsterDesc.pOwner);
			}

		}
		return __super::Atk_BehaviorTick(1.f, fTimeDelta);
	}
}

CClown_WizardNode_Attack1* CClown_WizardNode_Attack1::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CClown_WizardNode_Attack1* pInstance = new CClown_WizardNode_Attack1();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CClown_WizardNode_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClown_WizardNode_Attack1::Free()
{
	__super::Free();
}
