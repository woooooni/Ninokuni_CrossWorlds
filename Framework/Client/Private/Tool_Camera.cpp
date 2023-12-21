#include "stdafx.h"
#include "Tool_Camera.h"

#include "Camera_Free.h"
#include "imgui.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

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

	/* 임시 코드 */
	CCamera_Free* pCam = dynamic_cast<CCamera_Free*>(CCamera_Manager::GetInstance()->Get_CurCamera());

	if (nullptr != pCam)
	{
		IMGUI_NEW_LINE;
	
		ImGui::PushItemWidth(100.f);

		/* Speed */
		{
			if (KEY_STATE::HOLD == GI->GetKeyState(KEY::Z))
			{
				pCam->m_fMoveSpeed += fTimeDelta * 10.f;
			}
			else if (KEY_STATE::HOLD == GI->GetKeyState(KEY::X))
			{
				pCam->m_fMoveSpeed -= fTimeDelta * 10.f;
			}

			_float fSpeed = pCam->m_fMoveSpeed;
			if (ImGui::DragFloat("Cam Cur Move Speed", &fSpeed, 0.1f, 0.f, 200.f))
			{
				pCam->m_fMoveSpeed = fSpeed;

				if (pCam->m_fMoveSpeed < 0.f)
					pCam->m_fMoveSpeed = 0.f;
			}


			static _float fDefaultSpeed = 50.f;
			ImGui::InputFloat("Cam Default Speed", &fDefaultSpeed);
			
			if (ImGui::Button("Default Move"))
			{
				pCam->m_fMoveSpeed = fDefaultSpeed;
				if (pCam->m_fMoveSpeed < 0.f)
					pCam->m_fMoveSpeed = 0.f;
			}

			ImGui::Text("Z : 스피드 업, X : 스피드 다운");
		}

		IMGUI_NEW_LINE;

		/* Rotate */
		{			
			_float fRotate = pCam->m_fRotateSpeed;
			
			if (ImGui::DragFloat("Cam Cur Rotate Speed", &fRotate, 0.01f, 0.f, 100.f))
			{
				pCam->m_fRotateSpeed = fRotate;
				if (pCam->m_fRotateSpeed < 0.f)
					pCam->m_fRotateSpeed = 0.f;
			}


			static _float fRotateSpeed = 0.5f;
			ImGui::InputFloat("Cam Default Rotate", &fRotateSpeed);


			if (ImGui::Button("Default Rotate"))
			{
				pCam->m_fRotateSpeed = fRotateSpeed;
				if (pCam->m_fRotateSpeed < 0.f)
					pCam->m_fRotateSpeed = 0.f;
			}
		}

		ImGui::PopItemWidth();


	}

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
