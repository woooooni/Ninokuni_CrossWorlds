#include "stdafx.h"
#include "Tool_Terrain.h"
#include "imgui.h"


CTool_Terrain::CTool_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{
}

HRESULT CTool_Terrain::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_Terrain::Tick(_float fTimeDelta)
{
	ImGui::Begin("Terrain_Tool");

	ImGui::End();
}



CTool_Terrain* CTool_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Terrain* pInstance = new CTool_Terrain(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Terrain");
		return nullptr;
	}
	return pInstance;
}

void CTool_Terrain::Free()
{
	__super::Free();
}
