#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_SwordMan_PerfectBlade.h"

CSkill_SwordMan_PerfectBlade::CSkill_SwordMan_PerfectBlade()
{
	
}



HRESULT CSkill_SwordMan_PerfectBlade::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_SwordMan_PerfectBlade::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSkill_SwordMan_PerfectBlade::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_SwordMan_PerfectBlade::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::CLASS_SKILL_0);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_SwordMan_PerfectBlade* CSkill_SwordMan_PerfectBlade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_SwordMan_PerfectBlade* pInstance = new CSkill_SwordMan_PerfectBlade;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_SwordMan_PerfectBlade");
		return nullptr;
	}

	return pInstance;
}

void CSkill_SwordMan_PerfectBlade::Free()
{
	__super::Free();
}
