#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_SwordMan_AcaneBarrier.h"

CSkill_SwordMan_AcaneBarrier::CSkill_SwordMan_AcaneBarrier()
{
	
}



HRESULT CSkill_SwordMan_AcaneBarrier::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_SwordMan_AcaneBarrier::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_SwordMan_AcaneBarrier::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_SwordMan_AcaneBarrier::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::SKILL_SPECIAL_0);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_SwordMan_AcaneBarrier* CSkill_SwordMan_AcaneBarrier::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_SwordMan_AcaneBarrier* pInstance = new CSkill_SwordMan_AcaneBarrier;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_SwordMan_AcaneBarrier");
		return nullptr;
	}

	return pInstance;
}

void CSkill_SwordMan_AcaneBarrier::Free()
{
	__super::Free();
	
}
