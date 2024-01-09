#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Destroyer_LeafSlam.h"

CSkill_Destroyer_LeafSlam::CSkill_Destroyer_LeafSlam()
{
	
}



HRESULT CSkill_Destroyer_LeafSlam::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Destroyer_LeafSlam::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CSkill_Destroyer_LeafSlam::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Destroyer_LeafSlam::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		m_pCharacterStateMachine->Change_State(CCharacter::CLASS_SKILL_2);
		return true;
	}
	else
	{
		return false;
	}
}


CSkill_Destroyer_LeafSlam* CSkill_Destroyer_LeafSlam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Destroyer_LeafSlam* pInstance = new CSkill_Destroyer_LeafSlam;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Destroyer_LeafSlam");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Destroyer_LeafSlam::Free()
{
	__super::Free();
}
