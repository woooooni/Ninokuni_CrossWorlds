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


	return S_OK;
}

void CTool_Camera::Tick(_float fTimeDelta)
{
	ImGui::Begin("Camera_Tool");

	/* Temp */
	Show_Temp();

	/* Select Camera */
	Show_Select_Camera();
	
	/* Camera Prop */
	{
		CCamera* pCurCam = CCamera_Manager::GetInstance()->Get_CurCamera();
		if (nullptr != pCurCam)
		{
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

void CTool_Camera::Show_Temp()
{
	if (ImGui::TreeNode(u8"Temp"))
	{
		static _bool bLoadedMap = false;
		if (ImGui::Button(u8"맵 로드"))
		{
			if (bLoadedMap)
			{
				ImGui::TreePop();
				return;
			}
			bLoadedMap = true;

			wstring strMapFileName = L"Evermore";
			wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L".map";

			shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
			File->Open(strMapFilePath, FileMode::Read);

			for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
			{
				if (i == LAYER_TYPE::LAYER_CAMERA
					|| i == LAYER_TYPE::LAYER_TERRAIN
					|| i == LAYER_TYPE::LAYER_BACKGROUND
					|| i == LAYER_TYPE::LAYER_SKYBOX
					|| i == LAYER_TYPE::LAYER_UI
					|| i == LAYER_TYPE::LAYER_PLAYER
					|| i == LAYER_TYPE::LAYER_WEAPON
					|| i == LAYER_TYPE::LAYER_PROJECTILE
					|| i == LAYER_TYPE::LAYER_EFFECT
					|| i == LAYER_TYPE::LAYER_TRAIL
					|| i == LAYER_TYPE::LAYER_NPC
					|| i == LAYER_TYPE::LAYER_WEAPON)
					continue;

				GI->Clear_Layer(LEVEL_TOOL, i);

				{
					_uint iObjectCount = File->Read<_uint>();

					for (_uint j = 0; j < iObjectCount; ++j)
					{
						// 3. Object_Prototype_Tag
						wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
						wstring strObjectTag = CUtils::ToWString(File->Read<string>());

						CGameObject* pObj = nullptr;
						if (FAILED(GI->Add_GameObject(LEVEL_TOOL, i, strPrototypeTag, nullptr, &pObj)))
						{
							MSG_BOX("Load_Objects_Failed.");
							return;
						}

						if (nullptr == pObj)
						{
							MSG_BOX("Add_Object_Failed.");
							return;
						}
						pObj->Set_ObjectTag(strObjectTag);

						CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
						if (nullptr == pTransform)
						{
							MSG_BOX("Get_Transform_Failed.");
							return;
						}

						// 6. Obejct States
						_float4 vRight, vUp, vLook, vPos;

						File->Read<_float4>(vRight);
						File->Read<_float4>(vUp);
						File->Read<_float4>(vLook);
						File->Read<_float4>(vPos);

						pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
						pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
						pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
						pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
					}
				}
			}

			list<CGameObject*> Grounds = GI->Find_GameObjects(LEVEL_TOOL, LAYER_TYPE::LAYER_GROUND);
			for (auto& Ground : Grounds)
			{
				if (FAILED(GI->Add_Ground(Ground,
					Ground->Get_Component<CModel>(L"Com_Model"),
					Ground->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
				{
					MSG_BOX("카메라 툴에서 피직스 생성에 실패했습니다.");
				}
			}
		}
		//IMGUI_SAME_LINE;


		/* 플레이어 로드 */
		static _bool bLoadedPlayer = false;

		static _float fCreatePos[3] = { 0.f, 7.f, 0.f };
		ImGui::PushItemWidth(150.f);
		ImGui::DragFloat3("플레이어 로드 포지션", fCreatePos, 1.f, -5000.f, 5000.f, "%.1f");
		ImGui::PopItemWidth();

		if (ImGui::Button(u8"플레이어 로드"))
		{
			if (!bLoadedPlayer)
			{
				bLoadedPlayer = true;

				CGameObject* pCharacter = nullptr;

				if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_CHARACTER, TEXT("Prototype_GameObject_Character_SwordMan"), nullptr, &pCharacter)))
				{
					MSG_BOX("카메라 툴에서 플레이어 로드를 실패했습니다.");
					return;
				}

				if (!CCamera_Manager::GetInstance()->Is_Empty_Camera(CAMERA_TYPE::FOLLOW))
				{
					CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(pCharacter);
					CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(pCharacter);

					Vec3 vPos;
					memcpy(&vPos, fCreatePos, sizeof(Vec3));
					pCharacter->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION, vPos);
				}
			}
		}
		ImGui::TreePop();
	}
	
	IMGUI_NEW_LINE;
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
			for (int iCurComboIndex = 0; iCurComboIndex < CAMERA_TYPE::TYPE_END; iCurComboIndex++)
			{
				const bool is_selected = (eCurCamIndex == iCurComboIndex);

				if (ImGui::Selectable(CameraCharNames[iCurComboIndex], is_selected))
				{
					CCamera_Manager::GetInstance()->Set_CurCamera((CAMERA_TYPE)iCurComboIndex);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
	}
}

void CTool_Camera::Show_Camera_Prop_Free(CCamera* pCurCam)
{
	IMGUI_NEW_LINE;
	ImGui::Text("카메라 옵션 변경");

	if (ImGui::BeginChild("Free Camera Option", ImVec2(0, 300.f), true))
	{
		CCamera_Free* pFreeCam = dynamic_cast<CCamera_Free*>(pCurCam);
		if (nullptr != pFreeCam)
		{
			ImGui::PushItemWidth(100.f);
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
	ImGui::Text("카메라 옵션 변경");

	if (ImGui::BeginChild("Follow Camera Option", ImVec2(0, 700.f), true))
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(pCurCam);

		if (nullptr != pFollowCam)
		{
			ImGui::PushItemWidth(150.f);
			{
				/* 투영 관련 */
				{
					CCamera::PROJ_DESC tProjDesc = pFollowCam->Get_ProjDesc();

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
						ImGui::DragFloat(u8"Aspect", &tProjDesc.fAspect, 0.05f, 0.f, 10.f);

						/*if (ImGui::Button("Default Aspect"))
							tProjDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;*/
					}

					/* Near */
					{
						ImGui::DragFloat(u8"Near", &tProjDesc.fNear, 0.05f, 0.01f, 1.f);

						/*if (ImGui::Button("Default Near"))
							tProjDesc.fNear = 0.2f;*/
					}

					/* Far */
					{
						ImGui::DragFloat(u8"Far", &tProjDesc.fFar, 0.05f, 0.f, 2000.f);

						/*if (ImGui::Button("Default Far"))
							tProjDesc.fFar = 1000.f;*/
					}

					pFollowCam->Set_ProjDesc(tProjDesc);
				}

				/* 오프셋 */
				{
					/* 타겟 오프셋 */
					{
						Vec4	vTargetOffset		= pFollowCam->Get_TargetOffset();
						_float	fTargetOffset[3]	= { vTargetOffset.x, vTargetOffset.y, vTargetOffset.z };
						if (ImGui::DragFloat3(u8"Target OffSet", fTargetOffset))
						{
							pFollowCam->Set_TargetOffSet(Vec4{ fTargetOffset[0], fTargetOffset[1], fTargetOffset[2], 1.f });
						}
					}
					
					/* 룩앳 오프셋 */
					{
						Vec4	vLookAtOffset		= pFollowCam->Get_LookAtOffset();
						_float	fLookAtOffset[3]	= { vLookAtOffset.x, vLookAtOffset.y, vLookAtOffset.z };
						if (ImGui::DragFloat3(u8"LookAt Offset", fLookAtOffset))
						{
							pFollowCam->Set_LookAtOffSet(Vec4{ fLookAtOffset[0], fLookAtOffset[1], fLookAtOffset[2], 1.f });
						}
					}
				}

				/* 민감도 */
				{
					Vec2 vSeneitivity = pFollowCam->Get_MouseSensitivity();

					ImGui::DragFloat(u8"Seneitivity X (X축 민감도)", &vSeneitivity.x, 0.025f);
					ImGui::DragFloat(u8"Seneitivity Y (Y축 민감도)", &vSeneitivity.y, 0.025f);

					pFollowCam->Set_MouseSensitivity_X(vSeneitivity.x);
					pFollowCam->Set_MouseSensitivity_Y(vSeneitivity.y);

				}


				/* 디스턴스 */
				{
					_float fDistance = pFollowCam->Get_Distance();
					if (ImGui::DragFloat(u8"Distance(타겟과의 거리)", &fDistance, 0.5f, 0.1f, 100.f))
					{
						pFollowCam->Set_Distance(fDistance);
					}
				}

				/* 댐핑 */
				{
					/* 액티브 */
					_bool bDamping = pFollowCam->Is_Damping();
					if (ImGui::Checkbox(u8"Damping On Off", &bDamping))
					{
						pFollowCam->Set_Damping(bDamping);
					}


					/* 계수 */
					_float fCoefficient = pFollowCam->Get_DampingCoefficient();
					if (ImGui::DragFloat(u8"Damping Coefficient(댐핑 계수)", &fCoefficient, 0.002f, 0.002f, 1.f))
					{
						pFollowCam->Set_DampingCoefficient(fCoefficient);
					}
				}


				














			}
			ImGui::PopItemWidth();
		}
	}
	ImGui::EndChild();
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
