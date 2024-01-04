#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Engineer_BurstCall.h"

CSkill_Engineer_BurstCall::CSkill_Engineer_BurstCall()
{
	
}



HRESULT CSkill_Engineer_BurstCall::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Engineer_BurstCall::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Engineer_BurstCall::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


CSkill_Engineer_BurstCall* CSkill_Engineer_BurstCall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
{
	CSkill_Engineer_BurstCall* pInstance = new CSkill_Engineer_BurstCall;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Engineer_BurstCall");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Engineer_BurstCall::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pOwner);
}
