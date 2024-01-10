#include "stdafx.h"
#include "Tool_Terrain.h"
#include "imgui.h"
#include "GameInstance.h"

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

	ImGui::Text("m_vEye");
	if (ImGui::DragFloat4("##m_vEye", &m_vEye.x))
	{
		//GI->Set_ShadowView(LEVEL_TOOL,
		//	XMVectorSet(m_vEye.x, m_vEye.y, m_vEye.z, m_vEye.w),
		//	XMVectorSet(m_vAt.x, m_vAt.y, m_vAt.z, m_vAt.w),
		//	XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, m_vUp.w));
	}

	ImGui::Text("m_vAt");
	if (ImGui::DragFloat4("##m_vAt", &m_vAt.x))
	{
		//GI->Set_ShadowView(LEVEL_TOOL,
		//	XMVectorSet(m_vEye.x, m_vEye.y, m_vEye.z, m_vEye.w),
		//	XMVectorSet(m_vAt.x, m_vAt.y, m_vAt.z, m_vAt.w),
		//	XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, m_vUp.w));
	}

	ImGui::Text("m_vUp");
	if (ImGui::DragFloat4("##m_vUp", &m_vUp.x))
	{
		//GI->Set_ShadowView(LEVEL_TOOL,
		//	XMVectorSet(m_vEye.x, m_vEye.y, m_vEye.z, m_vEye.w),
		//	XMVectorSet(m_vAt.x, m_vAt.y, m_vAt.z, m_vAt.w),
		//	XMVectorSet(m_vUp.x, m_vUp.y, m_vUp.z, m_vUp.w));
	}

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
