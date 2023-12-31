#include "stdafx.h"
#include "Tool_Camera.h"

#include "imgui.h"

#include "GameInstance.h"
#include "Utils.h"
#include "FileUtils.h"

#include "Camera_Manager.h"

#include "Camera.h"
#include "Camera_Free.h"
#include "Camera_Follow.h"
#include "Camera_CutScene.h"

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

		/* 컷신은 별도 작업 */
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
	IMGUI_NEW_LINE;
	ImGui::Text("CutScene Camera Option (카메라 개별 옵션)");

	if (ImGui::BeginChild("CutScene Camera Option", ImVec2(0, 650.f), true))
	{
		CCamera_CutScene* pCutSceneCam = dynamic_cast<CCamera_CutScene*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE));

		if (nullptr != pCutSceneCam)
		{
			ImGui::PushItemWidth(220.f);
			{
				/* Func */
				{
					/* 컷신 시작 */
					if (ImGui::Button("Play CutScene"))
					{

					}
					IMGUI_SAME_LINE;

					/* 마크 숨기기 */
					ImGui::Checkbox("Show Mark", &m_bShowMarker);

				}
				IMGUI_NEW_LINE;

				vector<CAMERA_CUTSCENE_DESC> CutSceneDescs = pCutSceneCam->Get_CutSceneDescs();

				/* 전체 컷신 리스트 */
				{
					ImGui::Text(u8"전체 컷신 리스트 (count : %d)", CutSceneDescs.size());
					if (ImGui::BeginListBox("##CutScene_List", ImVec2(300.f, 70.f)))
					{
						for (size_t i = 0; i < CutSceneDescs.size(); ++i)
						{
							if (ImGui::Selectable(CutSceneDescs[i].strCutSceneName.c_str(), i == m_iCurCutSceneIndex))
							{
								m_iCurCutSceneIndex = i;
							}
						}
						ImGui::EndListBox();
					}
				}
				IMGUI_NEW_LINE;


				/* 현재 컷신 정보*/
				if(0 <= m_iCurCutSceneIndex)
				{
					/* 이름 편집 */
					{
						char szCutSceneName[MAX_PATH];
						sprintf_s(szCutSceneName, CutSceneDescs[m_iCurCutSceneIndex].strCutSceneName.c_str());
						if (ImGui::InputText(u8"컷신 이름 변경", szCutSceneName, MAX_PATH))
						{
							CutSceneDescs[m_iCurCutSceneIndex].strCutSceneName = szCutSceneName;
							pCutSceneCam->Change_CutSceneDesc(m_iCurCutSceneIndex, CutSceneDescs[m_iCurCutSceneIndex]);
						}
					}

					IMGUI_NEW_LINE;

					/* 포지션 편집 */
					{
						ImGui::Text(u8"카메라 포지션 리스트");
						_float3 fCamPositions[MAX_BEZIER_POINT];
						memcpy(fCamPositions, CutSceneDescs[m_iCurCutSceneIndex].vCamPositions, sizeof(Vec3) * MAX_BEZIER_POINT);

						for (size_t i = 0; i < MAX_BEZIER_POINT; i++)
						{
							float fPositions[3];
							memcpy(fPositions, &fCamPositions[i], sizeof(Vec3));

							string strLabelName = "Pos " + to_string(i);
							if(ImGui::DragFloat3(strLabelName.c_str(), fPositions, 0.05f, -20000.f, 20000.f, "%.1f"))
							{
								CutSceneDescs[m_iCurCutSceneIndex].vCamPositions[i] = { fPositions[0], fPositions[1], fPositions[2] };
								pCutSceneCam->Change_CutSceneDesc(m_iCurCutSceneIndex, CutSceneDescs[m_iCurCutSceneIndex]);
							}
						}
					}
					IMGUI_NEW_LINE;


					/* 룩앳 편집 */
					{
						ImGui::Text(u8"카메라 룩앳 리스트");
						_float3 fCamLookAts[MAX_BEZIER_POINT];
						memcpy(fCamLookAts, CutSceneDescs[m_iCurCutSceneIndex].vCamLookAts, sizeof(Vec3) * MAX_BEZIER_POINT);

						for (size_t i = 0; i < MAX_BEZIER_POINT; i++)
						{
							float fLookAt[3];
							memcpy(fLookAt, &fCamLookAts[i], sizeof(Vec3));

							string strLabelName = "Look " + to_string(i);
							if (ImGui::DragFloat3(strLabelName.c_str(), fLookAt, 0.05f, -20000.f, 20000.f, "%.1f"))
							{
								CutSceneDescs[m_iCurCutSceneIndex].vCamLookAts[i] = { fLookAt[0], fLookAt[1], fLookAt[2] };
								pCutSceneCam->Change_CutSceneDesc(m_iCurCutSceneIndex, CutSceneDescs[m_iCurCutSceneIndex]);
							}
						}
					}
					IMGUI_NEW_LINE;

					/* 컷신 시간 */
					if (ImGui::DragFloat("컷신 플레이 타임", &CutSceneDescs[m_iCurCutSceneIndex].fDuration, 0.1f, 0.1f, 100.f))
					{
						pCutSceneCam->Change_CutSceneDesc(m_iCurCutSceneIndex, CutSceneDescs[m_iCurCutSceneIndex]);
					}
					
					/* 시작 딜레이 시간 */
					if (ImGui::DragFloat(u8"시작 딜레이 시간", &CutSceneDescs[m_iCurCutSceneIndex].fStartDelayTime, 0.05f, 0.f, 100.f))
					{
						pCutSceneCam->Change_CutSceneDesc(m_iCurCutSceneIndex, CutSceneDescs[m_iCurCutSceneIndex]);
					}


					/* 종료 딜레이 시간 */
					if (ImGui::DragFloat(u8"종료 딜레이 시간", &CutSceneDescs[m_iCurCutSceneIndex].fFinishDelayTime, 0.05f, 0.f, 100.f))
					{
						pCutSceneCam->Change_CutSceneDesc(m_iCurCutSceneIndex, CutSceneDescs[m_iCurCutSceneIndex]);
					}

					IMGUI_NEW_LINE;

					/* 보간 모드 */
					{
						const char* Preview = LerpModeNames[(_uint)CutSceneDescs[m_iCurCutSceneIndex].eLerpMode].c_str();

						if (ImGui::BeginCombo(u8"보간 모드", Preview))
						{
							for (int iCurComboIndex = 0; iCurComboIndex < (_uint)LERP_MODE::TYPEEND; iCurComboIndex++)
							{
								const bool is_selected = (iCurComboIndex == (_uint)CutSceneDescs[m_iCurCutSceneIndex].eLerpMode);

								if (ImGui::Selectable(to_string(iCurComboIndex).c_str(), is_selected))
								{
									CutSceneDescs[m_iCurCutSceneIndex].eLerpMode = (LERP_MODE)iCurComboIndex;
									pCutSceneCam->Change_CutSceneDesc(m_iCurCutSceneIndex, CutSceneDescs[m_iCurCutSceneIndex]);
								}

								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}
							ImGui::EndCombo();
						}
					}

					IMGUI_NEW_LINE;
				}

				/* 추가 */
				if (ImGui::Button("Add CutScene"))
				{
					CAMERA_CUTSCENE_DESC desc;
					{
						desc.strCutSceneName = "New CutScene Data " + to_string(CutSceneDescs.size());

						Vec3 vCurCamLook = pCutSceneCam->Get_Transform()->Get_Look();

						Vec3 vCreateStartPos = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Position();
						vCreateStartPos += vCurCamLook * 30.f;

						for (size_t i = 0; i < 4; i++)
						{
							desc.vCamPositions[i] = vCreateStartPos + (vCurCamLook.ZeroY() * 30.f * i);
							desc.vCamPositions[i].x -= 10.f;

							desc.vCamLookAts[i] = vCreateStartPos + (vCurCamLook.ZeroY() * 30.f * i);
							desc.vCamLookAts[i].x += 10.f;
						}
					}

					pCutSceneCam->Add_CutSceneDesc(desc);

					m_iCurCutSceneIndex++;
				}
				IMGUI_SAME_LINE;

				/* 삭제 */
				if (ImGui::Button("Del CutScene"))
				{
					pCutSceneCam->Del_CutSceneDesc(CutSceneDescs[m_iCurCutSceneIndex].strCutSceneName);
					m_iCurCutSceneIndex--;
				}
				IMGUI_SAME_LINE;

				/* 저장 */
				if (ImGui::Button("Save CutScenes"))
				{
					pCutSceneCam->Save_CutSceneDescs();
				}
			}
			ImGui::PopItemWidth();
		}
	}
	ImGui::EndChild();



	///* 포지션 리스트*/
	//if (0 <= m_iTempPositionIndex)
	//{
	//	ImGui::Text("캠 포지션 리스트");
	//	const char* Preview = to_string(m_iTempPositionIndex).c_str();
	//	if (ImGui::BeginCombo(u8"포지션 인덱스", Preview))
	//	{
	//		/* 컷신 카메라는 별도 창에서 작업 하기 위해 인덱스를 -1 한다. */
	//		for (int iCurComboIndex = 0; iCurComboIndex < m_vTempPositions.size(); iCurComboIndex++)
	//		{
	//			const bool is_selected = (iCurComboIndex == m_iTempPositionIndex);

	//			if (ImGui::Selectable(to_string(iCurComboIndex).c_str(), is_selected))
	//			{
	//			m_iTempPositionIndex = iCurComboIndex;

	//			m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
	//			}

	//			if (is_selected)
	//				ImGui::SetItemDefaultFocus();
	//		}
	//		ImGui::EndCombo();
	//	}

	//	_float vPos[3] = { m_vTempPositions[m_iTempPositionIndex].x, m_vTempPositions[m_iTempPositionIndex].y , m_vTempPositions[m_iTempPositionIndex].z };
	//	if (ImGui::DragFloat3("포지션 수정", vPos))
	//	{
	//		memcpy(&m_vTempPositions[m_iTempPositionIndex], vPos, sizeof(Vec3));

	//		m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
	//	}
	//}


	///* 룩앳 리스트 */
	//if (0 <= m_iTempLookAtIndex)
	//{
	//	ImGui::Text("캠 룩앳 리스트");
	//	const char* Preview = to_string(m_iTempLookAtIndex).c_str();
	//	if (ImGui::BeginCombo(u8"룩앳 리스트", Preview))
	//	{
	//		for (int iCurComboIndex = 0; iCurComboIndex < m_vTempLookAts.size(); iCurComboIndex++)
	//		{
	//			const bool is_selected = (iCurComboIndex == m_iTempLookAtIndex);

	//			if (ImGui::Selectable(to_string(iCurComboIndex).c_str(), is_selected))
	//			{
	//				m_iTempLookAtIndex = iCurComboIndex;

	//				m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
	//			}

	//			if (is_selected)
	//				ImGui::SetItemDefaultFocus();
	//		}
	//		ImGui::EndCombo();
	//	}

	//	_float vLook[3] = { m_vTempLookAts[m_iTempLookAtIndex].x, m_vTempLookAts[m_iTempLookAtIndex].y , m_vTempLookAts[m_iTempLookAtIndex].z };
	//	if (ImGui::DragFloat3("룩앳 수정", vLook))
	//	{
	//		memcpy(&m_vTempLookAts[m_iTempLookAtIndex], vLook, sizeof(Vec3));

	//		m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
	//	}
	//}





	//IMGUI_NEW_LINE;
	//if (ImGui::Button("Add Position"))
	//{
	//	Vec3 vCurCamLook = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look();
	//	
	//	Vec3 vCreateStartPos = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Position();
	//	vCreateStartPos += vCurCamLook * 30.f;

	//	for (size_t i = 0; i < 4; i++)
	//	{
	//		Vec3 vCreatePos = vCreateStartPos + (vCurCamLook.ZeroY() * 30.f * i);

	//		m_vTempPositions.push_back(vCreatePos);
	//	}
	//	
	//	m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);

	//	m_iTempPositionIndex++;
	//}

	//IMGUI_SAME_LINE;
	//if (ImGui::Button("Add LookAt"))
	//{
	//	Vec3 vCurCamLook = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look();

	//	Vec3 vCreateStartPos = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Position();
	//	vCreateStartPos += vCurCamLook * 30.f;

	//	for (size_t i = 0; i < 4; i++)
	//	{
	//		Vec3 vCreatePos = vCreateStartPos + (vCurCamLook.ZeroY() * 30.f * i);

	//		m_vTempLookAts.push_back(vCreatePos);
	//	}

	//	m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);

	//	m_iTempLookAtIndex++;
	//}


	//if (!m_tDesc.bActive)
	//	m_tDesc.Start(m_tDesc.fEndTime, LERP_MODE::DEFAULT);
	//else
	//	m_tDesc.Update(fTimeDelta);


}

void CTool_Camera::Clear_CutSceneCache()
{
	m_bShow_Prop_CutScene = false;
	
	m_iCurCutSceneIndex = -1;

	m_bShowMarker = false;
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

	return S_OK;
}

HRESULT CTool_Camera::Render_DebugDraw()
{

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();
	{
		///* 포지션 위치 */
		//m_pSphere->Center = Calculate_Bezier(true);
		//DX::Draw(m_pBatch, *m_pSphere, Colors::Red);

		///* 룩앳 위치 */
		//m_pSphere->Center = Calculate_Bezier(false);
		//DX::Draw(m_pBatch, *m_pSphere, Colors::Green);

		///* 포지션 경로 */
		//for (size_t i = 0; i < m_vTempPositions.size(); i++)
		//{
		//	m_pSphere->Center = m_vTempPositions[i];
		//	DX::Draw(m_pBatch, *m_pSphere, Colors::Chocolate);
		//}

		///* 룩앳 경로*/
		//for (size_t i = 0; i < m_vTempLookAts.size(); i++)
		//{
		//	m_pSphere->Center = m_vTempLookAts[i];
		//	DX::Draw(m_pBatch, *m_pSphere, Colors::Black);
		//}
	}
	m_pBatch->End();

	return S_OK;
}

vector<Vec3> CTool_Camera::Subdivide_Bezier(const vector<Vec3>& controlPoints, int numSegments)
{
	vector<Vec3> result;

	for (int i = 0; i <= numSegments; ++i) 
	{
		float t = static_cast<float>(i) / numSegments;

		Vec3 v0 = Vec3::Lerp(controlPoints[0], controlPoints[1], t);
		Vec3 v1 = Vec3::Lerp(controlPoints[1], controlPoints[2], t);
		Vec3 v2 = Vec3::Lerp(controlPoints[2], controlPoints[3], t);

		Vec3 r0 = Vec3::Lerp(v0, v1, t);
		Vec3 r1 = Vec3::Lerp(v1, v2, t);

		result.push_back(Vec3::Lerp(r0, r1, t));
	}

	return result;
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
