#include "stdafx.h"
#include "Tool_Camera.h"

#include "imgui.h"

#include "GameInstance.h"
#include "Camera_Manager.h"


#include "Camera_Free.h"
#include "Camera_Follow.h"

#include "Utils.h"

CTool_Camera::CTool_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool(pDevice, pContext)
{
}

HRESULT CTool_Camera::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_Camera::Tick(_float fTimeDelta)
{
	ImGui::Begin("Camera_Tool");

	/* Select Camera */
	{
		const _int iCurCamIndex = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key();

		if (0 <= iCurCamIndex)
		{
			const char* Preview = CameraCharNames[iCurCamIndex];
			if (ImGui::BeginCombo("Select Camera", Preview))
			{
				for (int iCurComboIndex = 0; iCurComboIndex < CAMERA_TYPE::TYPE_END; iCurComboIndex++)
				{
					const bool is_selected = (iCurCamIndex == iCurComboIndex);

					if (ImGui::Selectable(CameraCharNames[iCurComboIndex], is_selected))
						CCamera_Manager::GetInstance()->Set_CurCamera((CAMERA_TYPE)iCurComboIndex);

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
	}
	
	/*if (ImGui::BeginChild("Child_List", ImVec2(0, 300.f), true))
	{
		
	}
	ImGui::EndChild();*/

	ImGui::End();
}



CTool_Camera* CTool_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Camera* pInstance = new CTool_Camera(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Camera");
		return nullptr;
	}
	return pInstance;
}

void CTool_Camera::Free()
{
	__super::Free();
}
