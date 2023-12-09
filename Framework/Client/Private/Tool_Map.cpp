#include "stdafx.h"
#include "Tool_Map.h"
#include "imgui.h"

CTool_Map::CTool_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool(pDevice, pContext)
{
}

HRESULT CTool_Map::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_Map::Tick(_float fTimeDelta)
{
	ImGui::Begin("Map_Tool");

	ImGui::End();
}



CTool_Map* CTool_Map::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Map* pInstance = new CTool_Map(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Map");
		return nullptr;
	}
	return pInstance;
}

void CTool_Map::Free()
{
	__super::Free();
}
