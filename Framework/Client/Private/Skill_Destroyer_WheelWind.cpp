#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Destroyer_WheelWind.h"

CSkill_Destroyer_WheelWind::CSkill_Destroyer_WheelWind()
{
	
}



HRESULT CSkill_Destroyer_WheelWind::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Destroyer_WheelWind::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Destroyer_WheelWind::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Destroyer_WheelWind::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_Destroyer_WheelWind* CSkill_Destroyer_WheelWind::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Destroyer_WheelWind* pInstance = new CSkill_Destroyer_WheelWind;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Destroyer_WheelWind");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Destroyer_WheelWind::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pOwner);
}
