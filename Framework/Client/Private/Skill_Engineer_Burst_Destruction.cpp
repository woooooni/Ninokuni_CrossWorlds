#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Engineer_Burst_Destruction.h"

CSkill_Engineer_Burst_Destruction::CSkill_Engineer_Burst_Destruction()
{
	
}



HRESULT CSkill_Engineer_Burst_Destruction::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 30.f;

	return S_OK;
}

void CSkill_Engineer_Burst_Destruction::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Engineer_Burst_Destruction::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Engineer_Burst_Destruction::Use_Skill()
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


CSkill_Engineer_Burst_Destruction* CSkill_Engineer_Burst_Destruction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Engineer_Burst_Destruction* pInstance = new CSkill_Engineer_Burst_Destruction;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Engineer_Burst_Destruction");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Engineer_Burst_Destruction::Free()
{
	__super::Free();
}
