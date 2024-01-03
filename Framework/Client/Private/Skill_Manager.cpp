#include "stdafx.h"
#include "GameInstance.h"
#include "Skill_Manager.h"
#include "Utils.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CSkill_Manager)

CSkill_Manager::CSkill_Manager()
{

}

HRESULT CSkill_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (true == m_bReserved)
		return S_OK;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);



	m_bReserved = true;


	return S_OK;
}

void CSkill_Manager::Tick(_float fTimeDelta)
{

}

void CSkill_Manager::LateTick(_float fTimeDelta)
{
	
}


void CSkill_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
}



