#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_SwordMan_SpinningAssault.h"

CSkill_SwordMan_SpinningAssault::CSkill_SwordMan_SpinningAssault()
{
	
}



HRESULT CSkill_SwordMan_SpinningAssault::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 3.f;

	return S_OK;
}

void CSkill_SwordMan_SpinningAssault::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSkill_SwordMan_SpinningAssault::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_SwordMan_SpinningAssault::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::CLASS_SKILL_2);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_SwordMan_SpinningAssault* CSkill_SwordMan_SpinningAssault::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_SwordMan_SpinningAssault* pInstance = new CSkill_SwordMan_SpinningAssault;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_SwordMan_SpinningAssault");
		return nullptr;
	}

	return pInstance;
}

void CSkill_SwordMan_SpinningAssault::Free()
{
	__super::Free();
}
