#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_SwordMan_PerfectBlade.h"

CSkill_SwordMan_PerfectBlade::CSkill_SwordMan_PerfectBlade()
{
	
}



HRESULT CSkill_SwordMan_PerfectBlade::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
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


CSkill_SwordMan_PerfectBlade* CSkill_SwordMan_PerfectBlade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
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
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pOwner);
}
