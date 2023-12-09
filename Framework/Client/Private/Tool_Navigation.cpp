#include "stdafx.h"
#include "Tool_Navigation.h"
#include "imgui.h"

CTool_Navigation::CTool_Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool(pDevice, pContext)
{
}

HRESULT CTool_Navigation::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_Navigation::Tick(_float fTimeDelta)
{
	ImGui::Begin("Navigation_Tool");

	ImGui::End();
}



CTool_Navigation* CTool_Navigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Navigation* pInstance = new CTool_Navigation(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Navigation");
		return nullptr;
	}
	return pInstance;
}

void CTool_Navigation::Free()
{
	__super::Free();
}
