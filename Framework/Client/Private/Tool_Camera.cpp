#include "stdafx.h"
#include "Tool_Camera.h"

#include "imgui.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

#include "Camera.h"
#include "Camera_Free.h"
#include "Camera_Follow.h"

#include "Utils.h"
#include "FileUtils.h"

CTool_Camera::CTool_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool(pDevice, pContext)
{
}

HRESULT CTool_Camera::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_DebugDraw()))
		return E_FAIL;

	return S_OK;
}

void CTool_Camera::Tick(_float fTimeDelta)
{
	ImGui::Begin("Camera_Tool");

	/* Select Camera */
	Show_Select_Camera();

	/* Camera Prop */
	{
		CCamera* pCurCam = CCamera_Manager::GetInstance()->Get_CurCamera();
		if (nullptr != pCurCam)
		{
			/* Cut Scene */
			if (m_bShow_Prop_CutScene)
			{
				Show_Camera_Prop_CutScene(fTimeDelta);
				ImGui::End();
				return;
			}

			/* 카메라 공통 옵션 */
			Show_Camera_Prop_Default(pCurCam);

			/* 카메라 개별 옵션 */
			switch (pCurCam->Get_Type())
			{
			case CAMERA_TYPE::FREE :
				Show_Camera_Prop_Free(pCurCam);
				break;
			case CAMERA_TYPE::FOLLOW :
				Show_Camera_Prop_Follow(pCurCam);
				break;
			default:
				break;
			}
		}
	}

	ImGui::End();
}

HRESULT CTool_Camera::Render()
{
	Render_DebugDraw();

	return S_OK;
}

void CTool_Camera::Show_Select_Camera()
{
	const CAMERA_TYPE eCurCamIndex = (CAMERA_TYPE)CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Type();

	if (0 <= eCurCamIndex)
	{
		ImGui::Text("카메라 리스트");
		const char* Preview = CameraCharNames[eCurCamIndex];
		ImGui::PushItemWidth(150.f);
		if (ImGui::BeginCombo(u8"현재 카메라", Preview))
		{
			/* 컷신 카메라는 별도 창에서 작업 하기 위해 인덱스를 -1 한다. */
			for (int iCurComboIndex = 0; iCurComboIndex < CAMERA_TYPE::CAMERA_TYPE_END - 1; iCurComboIndex++)
			{
				const bool is_selected = (eCurCamIndex == iCurComboIndex);

				if (ImGui::Selectable(CameraCharNames[iCurComboIndex], is_selected))
				{
					if (CAMERA_TYPE::CUTSCENE == iCurComboIndex)
						iCurComboIndex = CAMERA_TYPE::FREE;

					CCamera_Manager::GetInstance()->Set_CurCamera((CAMERA_TYPE)iCurComboIndex);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PushItemWidth(300.f);


		if (ImGui::Checkbox("컷신 작업", &m_bShow_Prop_CutScene))
		{
			if (!m_bShow_Prop_CutScene)
			{
				Clear_CutSceneCache();
			}
		}
			

		ImGui::PopItemWidth();
	}
}

void CTool_Camera::Show_Camera_Prop_Default(CCamera* pCurCam)
{
	IMGUI_NEW_LINE;
	ImGui::Text("Camera Public Option (카메라 공통 옵션)");

	if (ImGui::BeginChild("Public Camera Option", ImVec2(0, 450.f), true))
	{
		ImGui::PushItemWidth(150.f);
		{
			/* 투영 관련 */
			{
				CCamera::PROJ_DESC tProjDesc = pCurCam->Get_ProjDesc();

				/* Fov */
				{
					_float fFov = XMConvertToDegrees(tProjDesc.tLerpFov.fCurValue);
					ImGui::DragFloat(u8"Fov", &fFov, 0.1f, 10.f, 100.f);
					tProjDesc.tLerpFov.fCurValue = XMConvertToRadians(fFov);

					/*if (ImGui::Button("Default Fov"))
						tProjDesc.tLerpFov.fCurValue = Cam_Fov_Follow_Default;*/
				}

				/* Aspect */
				{
					ImGui::DragFloat(u8"Aspect", &tProjDesc.fAspect, 0.05f, 0.1f, 10.f);

					if (ImGui::Button("Default Aspect"))
						tProjDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
				}

				/* Near */
				{
					ImGui::DragFloat(u8"Near", &tProjDesc.fNear, 0.05f, 0.01f, 1.f);

					/*if (ImGui::Button("Default Near"))
						tProjDesc.fNear = 0.2f;*/
				}

				/* Far */
				{
					ImGui::DragFloat(u8"Far", &tProjDesc.fFar, 0.05f, 1.f, 2000.f);

					/*if (ImGui::Button("Default Far"))
						tProjDesc.fFar = 1000.f;*/
				}

				pCurCam->Set_ProjDesc(tProjDesc);
			}
			IMGUI_NEW_LINE;

			/* 오프셋 */
			{
				/* 타겟 오프셋 */
				{
					Vec4	vTargetOffset = pCurCam->Get_TargetOffset();
					_float	fTargetOffset[3] = { vTargetOffset.x, vTargetOffset.y, vTargetOffset.z };
					if (ImGui::DragFloat3(u8"Target OffSet", fTargetOffset))
					{
						pCurCam->Set_TargetOffSet(Vec4{ fTargetOffset[0], fTargetOffset[1], fTargetOffset[2], 1.f });
					}
				}

				/* 룩앳 오프셋 */
				{
					Vec4	vLookAtOffset = pCurCam->Get_LookAtOffset();
					_float	fLookAtOffset[3] = { vLookAtOffset.x, vLookAtOffset.y, vLookAtOffset.z };
					if (ImGui::DragFloat3(u8"LookAt Offset", fLookAtOffset))
					{
						pCurCam->Set_LookAtOffSet(Vec4{ fLookAtOffset[0], fLookAtOffset[1], fLookAtOffset[2], 1.f });
					}
				}
			}
			IMGUI_NEW_LINE;

			/* 민감도 */
			{
				Vec2 vSeneitivity = pCurCam->Get_MouseSensitivity();

				ImGui::DragFloat(u8"Seneitivity X (X축 민감도)", &vSeneitivity.x, 0.025f);
				ImGui::DragFloat(u8"Seneitivity Y (Y축 민감도)", &vSeneitivity.y, 0.025f);

				pCurCam->Set_MouseSensitivity_X(vSeneitivity.x);
				pCurCam->Set_MouseSensitivity_Y(vSeneitivity.y);
			}
			IMGUI_NEW_LINE;

			/* 디스턴스 */
			{
				_float fDistance = pCurCam->Get_Distance();
				if (ImGui::DragFloat(u8"Distance(타겟과의 거리)", &fDistance, 0.5f, 0.1f, 100.f))
				{
					pCurCam->Set_Distance(fDistance);
				}
			}
			IMGUI_NEW_LINE;

			/* Shake */
			{
				_bool bShake = pCurCam->Is_Shake();
				ImGui::Checkbox(u8"Is Shake", &bShake);

				static _float fAmplitudeInput = 0.05f;
				ImGui::DragFloat(u8"Shake Amplitude (쉐이킹 진폭) ", &fAmplitudeInput, 0.01f, 0.f, 5.f);

				static _float fFrquencyInput = 20.f;
				ImGui::DragFloat(u8"Shake Frquency (쉐이킹 빈도) ", &fFrquencyInput, 0.01f, 0.f, 100.f);

				static _float fDurationInput = 0.3f;
				ImGui::DragFloat(u8"Shake Duration (쉐이킹 시간) ", &fDurationInput, 0.01f, 0.f, 10.f);

				if (ImGui::Button(u8"Start Shake(쉐이킹 시작)"))
				{
					pCurCam->Start_Shake(fAmplitudeInput, fFrquencyInput, fDurationInput);
				}
			}
		}
		ImGui::PopItemWidth();
		
	}
	ImGui::EndChild();
}

void CTool_Camera::Show_Camera_Prop_Free(CCamera* pCurCam)
{
	IMGUI_NEW_LINE;
	ImGui::Text("Free Camera Option (카메라 개별 옵션)");

	if (ImGui::BeginChild("Free Camera Option", ImVec2(0, 200.f), true))
	{
		CCamera_Free* pFreeCam = dynamic_cast<CCamera_Free*>(pCurCam);
		if (nullptr != pFreeCam)
		{
			ImGui::PushItemWidth(150.f);
			{
				/* 카메라 이동속도 */
				_float fCameraMoveSpeed = pFreeCam->Get_MoveSpeed();

				if (ImGui::DragFloat(u8"카메라 이동 속도", &fCameraMoveSpeed, 0.5f))
				{
					pFreeCam->Set_MoveSpeed(fCameraMoveSpeed);
				}

				/* 카메라 회전 속도 */
				_float fCameraRotateSpeed = pFreeCam->Get_RotateSpeed();
				if (ImGui::DragFloat(u8"카메라 회전 속도", &fCameraRotateSpeed, 0.1f))
				{
					pFreeCam->Set_RotateSpeed(fCameraRotateSpeed);
				}
			}
			ImGui::PopItemWidth();
		}
	}
	ImGui::EndChild();
}

void CTool_Camera::Show_Camera_Prop_Follow(CCamera* pCurCam)
{
	IMGUI_NEW_LINE;
	ImGui::Text("Follow Camera Option (카메라 개별 옵션)");

	if (ImGui::BeginChild("Follow Camera Option", ImVec2(0, 200.f), true))
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(pCurCam);

		if (nullptr != pFollowCam)
		{
			ImGui::PushItemWidth(150.f);
			{
				/* 댐핑 */
				{
					/* 액티브 */
					_bool bDamping = pFollowCam->Is_Damping();
					if (ImGui::Checkbox(u8"Damping On Off", &bDamping))
					{
						pFollowCam->Set_Damping(bDamping);
					}

					/* 댐핑 계수 */
					_float fCoefficient = pFollowCam->Get_DampingCoefficient();
					if (ImGui::DragFloat(u8"Damping Coefficient(댐핑 계수)", &fCoefficient, 0.002f, 0.002f, 1.f))
					{
						pFollowCam->Set_DampingCoefficient(fCoefficient);
					}

					/* 역방향시 댐핑 계수 배율 */
					_float fMag = pFollowCam->Get_DampingCoefficientBackMag();
					if (ImGui::DragFloat(u8"역방향 댐핑 계수 비율", &fMag, 0.1f, 1.f, 10.f))
					{
						pFollowCam->Set_DampingCoefficientBackMag(fMag);
					}

					/* 역방향 기준 각도 */
					_float fRad = pFollowCam->Get_DampingBackLimitRad();
					if (ImGui::DragFloat(u8"역방향 기준 각도 (rad)", &fRad, 0.1f, 1.f, 3.14f))
					{
						pFollowCam->Set_DampingBackLimitRad(fRad);
					}
				}
				IMGUI_NEW_LINE;

				/* 회전 */
				{
					/* Max Y */
					_float fMaxRotDeltaY = pFollowCam->Get_MaxRotationLimitDeltaY();
					if (ImGui::DragFloat(u8"Y축 회전 델타 최댓값 ", &fMaxRotDeltaY, 0.01f, -1.f, 1.f))
					{
						pFollowCam->Set_MaxRotationLimitDeltaY(fMaxRotDeltaY);
					}

					/* Min Y */
					_float fMinRotDeltaY = pFollowCam->Get_MinRotationLimitDeltaY();
					if (ImGui::DragFloat(u8"Y축 회전 델타 최솟값 ", &fMinRotDeltaY, 0.01f, -1.f, 1.f))
					{
						pFollowCam->Set_MinRotationLimitDeltaY(fMinRotDeltaY);
					}
				}
			}
			ImGui::PopItemWidth();
		}
	}
	ImGui::EndChild();
}

void CTool_Camera::Show_Camera_Prop_CutScene(_float fTimeDelta)
{
	/* 플레이 */
	IMGUI_NEW_LINE;
	if (ImGui::Checkbox("Play CutScene", &m_bPlay))
	{
		if (m_bPlay)
		{
			m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
		}
	}

	if (m_bPlay)
	{
		if (m_tDesc.bActive)
		{
			Vec3 vPos = Calculate_Bezier(true);
			Vec4 vFinalPos = { vPos.x, vPos.y, vPos.z, 1.f };

			Vec3 vLook = Calculate_Bezier(false);
			Vec4 vFinalLook = { vLook.x, vLook.y, vLook.z, 1.f };
			CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FREE)->Get_Transform()->Set_State(CTransform::STATE_POSITION, vFinalPos);
			CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FREE)->Get_Transform()->LookAt(vFinalLook);
		}
		else
			m_bPlay = false;
	}
	

	/* 포지션 리스트*/
	if (0 <= m_iTempPositionIndex)
	{
		ImGui::Text("캠 포지션 리스트");
		const char* Preview = to_string(m_iTempPositionIndex).c_str();
		if (ImGui::BeginCombo(u8"포지션 인덱스", Preview))
		{
			/* 컷신 카메라는 별도 창에서 작업 하기 위해 인덱스를 -1 한다. */
			for (int iCurComboIndex = 0; iCurComboIndex < m_vTempPositions.size(); iCurComboIndex++)
			{
				const bool is_selected = (iCurComboIndex == m_iTempPositionIndex);

				if (ImGui::Selectable(to_string(iCurComboIndex).c_str(), is_selected))
				{
				m_iTempPositionIndex = iCurComboIndex;

				m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		_float vPos[3] = { m_vTempPositions[m_iTempPositionIndex].x, m_vTempPositions[m_iTempPositionIndex].y , m_vTempPositions[m_iTempPositionIndex].z };
		if (ImGui::DragFloat3("포지션 수정", vPos))
		{
			memcpy(&m_vTempPositions[m_iTempPositionIndex], vPos, sizeof(Vec3));

			m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
		}
	}


	/* 룩앳 리스트 */
	if (0 <= m_iTempLookAtIndex)
	{
		ImGui::Text("캠 룩앳 리스트");
		const char* Preview = to_string(m_iTempLookAtIndex).c_str();
		if (ImGui::BeginCombo(u8"룩앳 리스트", Preview))
		{
			for (int iCurComboIndex = 0; iCurComboIndex < m_vTempLookAts.size(); iCurComboIndex++)
			{
				const bool is_selected = (iCurComboIndex == m_iTempLookAtIndex);

				if (ImGui::Selectable(to_string(iCurComboIndex).c_str(), is_selected))
				{
					m_iTempLookAtIndex = iCurComboIndex;

					m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		_float vLook[3] = { m_vTempLookAts[m_iTempLookAtIndex].x, m_vTempLookAts[m_iTempLookAtIndex].y , m_vTempLookAts[m_iTempLookAtIndex].z };
		if (ImGui::DragFloat3("룩앳 수정", vLook))
		{
			memcpy(&m_vTempLookAts[m_iTempLookAtIndex], vLook, sizeof(Vec3));

			m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
		}
	}





	IMGUI_NEW_LINE;
	if (ImGui::Button("Add Position"))
	{
		Vec3 vPos = Vec3::Zero;

		if (!m_vTempPositions.empty())
			vPos = m_vTempPositions.back();

		m_vTempPositions.push_back(vPos);

		m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);

		m_iTempPositionIndex++;
	}

	IMGUI_SAME_LINE;
	if (ImGui::Button("Add LookAt"))
	{
		Vec3 vPos = Vec3::Zero;

		if (!m_vTempLookAts.empty())
			vPos = m_vTempLookAts.back();

		m_vTempLookAts.push_back(vPos);

		m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);

		m_iTempLookAtIndex++;
	}


	if (!m_tDesc.bActive)
		m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
	else
		m_tDesc.Update(fTimeDelta);




	_float fSpeed = m_tDesc.fEndTime;
	if (ImGui::DragFloat("Speed", &fSpeed, 0.1f, 0.1f, 100.f))
	{
		m_tDesc.fEndTime = fSpeed;
		
	}


	//if (FAILED(CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE)))
	//	return;

	//IMGUI_NEW_LINE;
	//ImGui::Text("CutScene Camera Option (카메라 개별 옵션)");

	//map<string, vector<CAMERA_CUT_SCENE_EVENT_DESC>> CutSceneEvents = CCamera_Manager::GetInstance()->Get_Action_CutSceneEvents();

	//if (CutSceneEvents.empty())
	//{
	//	ImGui::Text("현재 등록된 컷신 이벤트가 없습니다.");
	//	return;
	//}

	//if (ImGui::BeginChild("CutScene Camera Option", ImVec2(0, 200.f), true))
	//{
	//	ImGui::PushItemWidth(150.f);

	//	/* 전체 컷신 이벤트 리스트 */
	//	ImGui::Text("현재 등록된 컷신 이벤트");

	//	if (0 <= m_iCurCutSceneEventIndex)
	//	{
	//		const char* Preview = m_strCurCutSceneEventName.c_str();

	//		if (ImGui::BeginListBox("##CutScenes_List", ImVec2(150.f, 150.f)))
	//		{
	//			for (size_t i = 0; i < CutSceneEvents.size(); ++i)
	//			{
	//				string strCutSceneEventName = CCamera_Manager::GetInstance()->Get_Action_CutSceneEvent_Name(i).c_str();

	//				if (ImGui::Selectable(strCutSceneEventName.c_str(), i == m_iCurCutSceneEventIndex))
	//				{
	//					m_iCurCutSceneEventIndex = i;

	//					m_strCurCutSceneEventName = CCamera_Manager::GetInstance()->Get_Action_CutSceneEvent_Name(m_iCurCutSceneEventIndex);
	//				}
	//			}
	//			ImGui::EndListBox();
	//		}
	//	}

	//	/* Add Event */
	//	if (ImGui::Button("Add CutScene Event"))
	//	{
	//		CCamera_Manager::GetInstance()->Add_Action_CutSceneEvent(m_strCurCutSceneEventName);
	//	}
	//	IMGUI_SAME_LINE;


	//	ImGui::PopItemWidth();
	//}
	//ImGui::EndChild();
}

void CTool_Camera::Clear_CutSceneCache()
{
	m_bShow_Prop_CutScene = false;
	m_iCurCutSceneEventIndex = -1;
	m_strCurCutSceneEventName = "";
}

Vec3 CTool_Camera::Calculate_Bezier(_bool bPos)
{
	vector<Vec3> vPath;


	if (bPos)
		vPath = m_vTempPositions;
	else
		vPath = m_vTempLookAts;


	if (4 > vPath.size())
		return Vec3::Zero;
		
	_float fNormalizedTime = m_tDesc.fCurTime / m_tDesc.fEndTime;
	fNormalizedTime = min(1.0f, max(0.0f, fNormalizedTime));

	Vec3 v0 = Vec3::Lerp(vPath[0], vPath[1], fNormalizedTime);
	Vec3 v1 = Vec3::Lerp(vPath[1], vPath[2], fNormalizedTime);
	Vec3 v2 = Vec3::Lerp(vPath[2], vPath[3], fNormalizedTime);

	Vec3 r0 = Vec3::Lerp(v0, v1, fNormalizedTime);
	Vec3 r1 = Vec3::Lerp(v1, v2, fNormalizedTime);

	return Vec3::Lerp(r0, r1, fNormalizedTime);
}

HRESULT CTool_Camera::Ready_DebugDraw()
{
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(GI->Get_Context());
	m_pEffect = new BasicEffect(GI->Get_Device());

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCodes = nullptr;
	size_t			iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(GI->Get_Device()->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;

	_float	fRadius = 0.5f;
	Vec3	vOrigin = { 0.f, fRadius * 0.5f, 0.f };
	m_pSphere = new BoundingSphere(vOrigin, fRadius);

	if (nullptr == m_pSphere)
		return E_FAIL;


	m_vTempPositions.push_back(Vec3{ 0.f, 10.f, 0.f });
	m_vTempPositions.push_back(Vec3{ -30.f, 30.f, 30.f });
	m_vTempPositions.push_back(Vec3{ -30.f, 40.f, 60.f });
	m_vTempPositions.push_back(Vec3{ 0.f, 20.f, 90.f });

	m_iTempPositionIndex = 0;

	m_tDesc.fEndTime = 1.f;

	return S_OK;
}

HRESULT CTool_Camera::Render_DebugDraw()
{
	if (m_bPlay)
		return S_OK;

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();
	{
		/* 포지션 위치 */
		m_pSphere->Center = Calculate_Bezier(true);
		DX::Draw(m_pBatch, *m_pSphere, Colors::Red);

		/* 룩앳 위치 */
		m_pSphere->Center = Calculate_Bezier(false);
		DX::Draw(m_pBatch, *m_pSphere, Colors::Green);

		/* 포지션 경로 */
		for (size_t i = 0; i < m_vTempPositions.size(); i++)
		{
			m_pSphere->Center = m_vTempPositions[i];
			DX::Draw(m_pBatch, *m_pSphere, Colors::Chocolate);
		}

		/* 룩앳 경로*/
		for (size_t i = 0; i < m_vTempLookAts.size(); i++)
		{
			m_pSphere->Center = m_vTempLookAts[i];
			DX::Draw(m_pBatch, *m_pSphere, Colors::Black);
		}
	}
	m_pBatch->End();

	return S_OK;
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

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pSphere);
	Safe_Release(m_pInputLayout);
}
