#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Engineer_TimeLab.h"

CSkill_Engineer_TimeLab::CSkill_Engineer_TimeLab()
{
	
}



HRESULT CSkill_Engineer_TimeLab::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 30.f;

	return S_OK;
}

void CSkill_Engineer_TimeLab::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Engineer_TimeLab::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Engineer_TimeLab::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::SKILL_SPECIAL_1);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_Engineer_TimeLab* CSkill_Engineer_TimeLab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Engineer_TimeLab* pInstance = new CSkill_Engineer_TimeLab;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Engineer_TimeLab");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Engineer_TimeLab::Free()
{
	__super::Free();
}
