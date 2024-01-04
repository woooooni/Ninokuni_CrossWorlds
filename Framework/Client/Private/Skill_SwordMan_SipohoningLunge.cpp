#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_SwordMan_SipohoningLunge.h"

CSkill_SwordMan_SipohoningLunge::CSkill_SwordMan_SipohoningLunge()
{
	
}



HRESULT CSkill_SwordMan_SipohoningLunge::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

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


CSkill_SwordMan_SipohoningLunge* CSkill_SwordMan_SipohoningLunge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
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
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pOwner);
}
