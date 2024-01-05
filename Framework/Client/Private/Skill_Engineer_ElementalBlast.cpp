#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Engineer_ElementalBlast.h"

CSkill_Engineer_ElementalBlast::CSkill_Engineer_ElementalBlast()
{
	
}



HRESULT CSkill_Engineer_ElementalBlast::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Engineer_ElementalBlast::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Engineer_ElementalBlast::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Engineer_ElementalBlast::Use_Skill()
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


CSkill_Engineer_ElementalBlast* CSkill_Engineer_ElementalBlast::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Engineer_ElementalBlast* pInstance = new CSkill_Engineer_ElementalBlast;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Engineer_ElementalBlast");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Engineer_ElementalBlast::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pOwner);
}
