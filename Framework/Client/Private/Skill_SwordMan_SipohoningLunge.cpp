#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_SwordMan_SipohoningLunge.h"

CSkill_SwordMan_SipohoningLunge::CSkill_SwordMan_SipohoningLunge()
{
	
}



HRESULT CSkill_SwordMan_SipohoningLunge::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 5.f;

	return S_OK;
}

void CSkill_SwordMan_SipohoningLunge::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_SwordMan_SipohoningLunge::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_SwordMan_SipohoningLunge::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::CLASS_SKILL_1);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_SwordMan_SipohoningLunge* CSkill_SwordMan_SipohoningLunge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_SwordMan_SipohoningLunge* pInstance = new CSkill_SwordMan_SipohoningLunge;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_SwordMan_SipohoningLunge");
		return nullptr;
	}

	return pInstance;
}

void CSkill_SwordMan_SipohoningLunge::Free()
{
	__super::Free();
	
}
