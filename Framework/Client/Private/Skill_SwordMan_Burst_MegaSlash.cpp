#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_SwordMan_Burst_MegaSlash.h"

CSkill_SwordMan_Burst_MegaSlash::CSkill_SwordMan_Burst_MegaSlash()
{
	
}



HRESULT CSkill_SwordMan_Burst_MegaSlash::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_SwordMan_Burst_MegaSlash::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSkill_SwordMan_Burst_MegaSlash::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_SwordMan_Burst_MegaSlash::Use_Skill()
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


CSkill_SwordMan_Burst_MegaSlash* CSkill_SwordMan_Burst_MegaSlash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_SwordMan_Burst_MegaSlash* pInstance = new CSkill_SwordMan_Burst_MegaSlash;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_SwordMan_Burst_MegaSlash");
		return nullptr;
	}

	return pInstance;
}

void CSkill_SwordMan_Burst_MegaSlash::Free()
{
	__super::Free();
}
