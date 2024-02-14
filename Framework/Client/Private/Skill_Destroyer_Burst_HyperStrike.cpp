#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Destroyer_Burst_HyperStrike.h"

CSkill_Destroyer_Burst_HyperStrike::CSkill_Destroyer_Burst_HyperStrike()
{
	m_fCoolTime = 30.f;
}



HRESULT CSkill_Destroyer_Burst_HyperStrike::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Destroyer_Burst_HyperStrike::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Destroyer_Burst_HyperStrike::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Destroyer_Burst_HyperStrike::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::STATE::SKILL_BURST);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_Destroyer_Burst_HyperStrike* CSkill_Destroyer_Burst_HyperStrike::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Destroyer_Burst_HyperStrike* pInstance = new CSkill_Destroyer_Burst_HyperStrike;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Destroyer_Burst_HyperStrike");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Destroyer_Burst_HyperStrike::Free()
{
	__super::Free();
	
}
