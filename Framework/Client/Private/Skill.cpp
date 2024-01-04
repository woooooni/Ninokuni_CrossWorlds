#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill.h"

CSkill::CSkill()
	: CBase()
{
	
}



HRESULT CSkill::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (nullptr == pOwner)
		return E_FAIL;

	m_pOwner = pOwner;
	Safe_AddRef(m_pOwner);

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


void CSkill::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pOwner);
}
