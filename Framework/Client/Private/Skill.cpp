#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill.h"

CSkill::CSkill()
	: CBase()
{
	
}



HRESULT CSkill::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (nullptr == pOwner)
		return E_FAIL;

	m_pCharacter = pOwner;
	Safe_AddRef(m_pCharacter);

	m_pCharacterStateMachine = m_pCharacter->Get_Component<CStateMachine>(L"Com_StateMachine");
	if (nullptr == m_pCharacterStateMachine)
		return E_FAIL;

	Safe_AddRef(m_pCharacterStateMachine);

	return S_OK;
}

void CSkill::Tick(_float fTimeDelta)
{
	if (false == m_bUseable)
	{
		m_fAccCoolTime += fTimeDelta;
		if (m_fAccCoolTime >= m_fCoolTime)
		{
			m_fAccCoolTime = m_fCoolTime;
			m_bUseable = true;
		}
	}

}

void CSkill::LateTick(_float fTimeDelta)
{
	
}

_bool CSkill::Use_Skill()
{
	if (true == m_bUseable)
	{
		m_bUseable = false;
		m_fAccCoolTime = 0.f; 
		return true;
	}
	else
	{
		return false;
	}
	
}


void CSkill::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pCharacter);
	Safe_Release(m_pCharacterStateMachine);
}
