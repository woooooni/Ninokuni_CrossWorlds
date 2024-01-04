#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Engineer_ExplosionShot.h"

CSkill_Engineer_ExplosionShot::CSkill_Engineer_ExplosionShot()
{
	
}



HRESULT CSkill_Engineer_ExplosionShot::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Engineer_ExplosionShot::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Engineer_ExplosionShot::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


CSkill_Engineer_ExplosionShot* CSkill_Engineer_ExplosionShot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner)
{
	CSkill_Engineer_ExplosionShot* pInstance = new CSkill_Engineer_ExplosionShot;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Engineer_ExplosionShot");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Engineer_ExplosionShot::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pOwner);
}
