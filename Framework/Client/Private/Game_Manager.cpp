#include "stdafx.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "Utils.h"
#include "Player.h"

IMPLEMENT_SINGLETON(CGame_Manager)

CGame_Manager::CGame_Manager()
{

}

HRESULT CGame_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (true == m_bReserved)
		return S_OK;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	m_pPlayer = CPlayer::Create();

	if (nullptr == m_pPlayer)
		return E_FAIL;

	m_bReserved = true;


	return S_OK;
}

void CGame_Manager::Tick(_float fTimeDelta)
{
	if(nullptr != m_pPlayer)
		m_pPlayer->Tick(fTimeDelta);
}

void CGame_Manager::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pPlayer)
		m_pPlayer->LateTick(fTimeDelta);
}


void CGame_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
}



