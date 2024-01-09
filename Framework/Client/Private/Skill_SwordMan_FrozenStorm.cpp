#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_SwordMan_FrozenStorm.h"

CSkill_SwordMan_FrozenStorm::CSkill_SwordMan_FrozenStorm()
{
	
}



HRESULT CSkill_SwordMan_FrozenStorm::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_SwordMan_FrozenStorm::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_SwordMan_FrozenStorm::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_SwordMan_FrozenStorm::Use_Skill()
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


CSkill_SwordMan_FrozenStorm* CSkill_SwordMan_FrozenStorm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_SwordMan_FrozenStorm* pInstance = new CSkill_SwordMan_FrozenStorm;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_SwordMan_FrozenStorm");
		return nullptr;
	}

	return pInstance;
}

void CSkill_SwordMan_FrozenStorm::Free()
{
	__super::Free();
	
}
