#include "stdafx.h"
#include "Tool_Effect.h"
#include "imgui.h"


CTool_Effect::CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{
}

HRESULT CTool_Effect::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_Effect::Tick(_float fTimeDelta)
{
	ImGui::Begin("Effect_Tool");

	ImGui::End();
}



CTool_Effect* CTool_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Effect* pInstance = new CTool_Effect(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Effect");
		return nullptr;
	}
	return pInstance;
}

void CTool_Effect::Free()
{
	__super::Free();
}
