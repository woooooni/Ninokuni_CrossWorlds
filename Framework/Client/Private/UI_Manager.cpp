#include "stdafx.h"
#include "..\Public\UI_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Effect.h"
#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"
#include "Character.h"
#include "Monster.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{

}

HRESULT CUI_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	


	return S_OK;
}


void CUI_Manager::Tick(_float fTimeDelta)
{


}

void CUI_Manager::LateTick(_float fTimeDelta)
{
	
}




void CUI_Manager::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
