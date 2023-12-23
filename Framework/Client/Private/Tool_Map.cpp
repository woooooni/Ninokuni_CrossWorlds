#include "stdafx.h"
#include "Tool_Map.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "FileUtils.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Utils.h"
#include "Picking_Manager.h"
#include "Mesh.h"
#include "Light.h"
#include <filesystem>
//#include "ImGuizmo.h"
#include "PipeLine.h"

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
	ImGuiWindowFlags WindowFlags = 0;
	if (!m_bWindowResizeFlag)
		WindowFlags |= ImGuiWindowFlags_NoResize;
	if (!m_bWindowMoveFlag)
		WindowFlags |= ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Inspector", NULL, WindowFlags))
	{
		ImGui::SetWindowSize(ImVec2(450.f, 900.f));

#pragma region MapObject
		MapObjectSpace();
#pragma endregion MapObject

#pragma region MapLight
		MapLightSpace();
#pragma endregion MapLight

#pragma region MapMonster
		MapMonsterSpace();
#pragma endregion MapMonster

#pragma region MapNPC
		MapNPCSpace();
#pragma endregion MapNPC

		int a = 0;

		Picking();
	}
	
	ImGui::End();
}

void CTool_Map::AddMapObject(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
	const map<const std::wstring, CGameObject*>& vecPrototypeList = GI->Find_Prototype_GameObjects(iLayerType);

	for (auto& Pair : vecPrototypeList)
	{
		wstring strLevelFirst = CUtils::ToWString(m_ImguiSelectableNameList[m_iCurrentLevel]);
		wstring strFinalPath = strLevelFirst + L"_";

		if (Pair.first.find(strLevelFirst.c_str()) != std::wstring::npos 
			|| Pair.first.find(TEXT("Common_")) != std::wstring::npos
			|| Pair.first.find(TEXT("Animal_")) != std::wstring::npos)
		{
			if (ImGui::Selectable(CUtils::ToString(Pair.first).c_str()))
			{
				if (true == m_bAddObject)
					GI->Add_GameObject(iLevelID, iLayerType, Pair.first);
			}
		}
	}
}

void CTool_Map::DeleteObject(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
	list<CGameObject*>& pGameObjects = GI->Find_GameObjects(iLevelID, iLayerType);

	auto iter = pGameObjects.begin();

	while (iter != pGameObjects.end())
	{
		if (nullptr == m_pSelectObj)
			break;

		if (m_pSelectObj->Get_ObjectID() == (*iter)->Get_ObjectID())
		{
			Safe_Release<CGameObject*>((*iter));
			iter = pGameObjects.erase(iter);
			m_pSelectObj = nullptr;
		}
		else
			++iter;

	}
}

void CTool_Map::DeleteLight(_uint iLightID)
{
	list<CLight*>* pLightlist = GI->Get_LightList();

	auto iter = pLightlist->begin();

	while (iter != pLightlist->end())
	{
		if (nullptr == m_pSelectLight)
			break;

		if (m_pSelectLight->Get_LightID() == (*iter)->Get_LightID())
		{
			Safe_Release<CLight*>((*iter));
			iter = pLightlist->erase(iter);
			m_pSelectLight = nullptr;
		}
		else
			++iter;
	}

	_uint iLightCount = 0;

	for (auto& pLight : *pLightlist)
		pLight->Set_LightID(iLightCount++);
}

void CTool_Map::BatchObject(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
	list<CGameObject*>& pGameObjects = GI->Find_GameObjects(iLevelID, iLayerType);

	for (auto& pObj : pGameObjects)
	{
		string ObjectID = std::to_string(pObj->Get_ObjectID());
		string ObjectTag = CUtils::ToString(pObj->Get_ObjectTag());

		string strName = ObjectTag + "_" + ObjectID;

		if (ImGui::Selectable(strName.c_str()))
			m_pSelectObj = pObj;
	}
}

void CTool_Map::AddMapMonster(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
	const map<const std::wstring, CGameObject*>& vecPrototypeList = GI->Find_Prototype_GameObjects(iLayerType);

	for (auto& Pair : vecPrototypeList)
	{
		if (ImGui::Selectable(CUtils::ToString(Pair.first).c_str()))
		{
			if (true == m_bAddMonster)
				GI->Add_GameObject(iLevelID, iLayerType, Pair.first);
		}
		
	}
}

void CTool_Map::AddMapNPC(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
}

void CTool_Map::BatchNPC(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
}

void CTool_Map::BatchDynamic(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
	list<CGameObject*>& pGameObjects = GI->Find_GameObjects(iLevelID, iLayerType);

	for (auto& pObj : pGameObjects)
	{
		string ObjectID = std::to_string(pObj->Get_ObjectID());
		string ObjectTag = CUtils::ToString(pObj->Get_ObjectTag());

		string strName = ObjectTag + "_" + ObjectID;

		if (ImGui::Selectable(strName.c_str()))
			m_pSelectObj = pObj;
	}
}

void CTool_Map::DeleteDynamic(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
	list<CGameObject*>& pGameObjects = GI->Find_GameObjects(iLevelID, iLayerType);

	auto iter = pGameObjects.begin();

	while (iter != pGameObjects.end())
	{
		if (nullptr == m_pSelectObj)
			break;

		if (m_pSelectObj->Get_ObjectID() == (*iter)->Get_ObjectID())
		{
			Safe_Release<CGameObject*>((*iter));
			iter = pGameObjects.erase(iter);
			m_pSelectObj = nullptr;
		}
		else
			++iter;

	}
}

void CTool_Map::Picking()
{
	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::LBTN))
	{
		for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
		{
			if (i == LAYER_TYPE::LAYER_CAMERA
				|| i == LAYER_TYPE::LAYER_TERRAIN
				|| i == LAYER_TYPE::LAYER_BACKGROUND
				|| i == LAYER_TYPE::LAYER_SKYBOX
				|| i == LAYER_TYPE::LAYER_UI
				|| i == LAYER_TYPE::LAYER_EFFECT
				|| i == LAYER_TYPE::LAYER_GRASS)
				continue;

			list<CGameObject*>& GameObjects = GI->Find_GameObjects(LEVEL_TOOL, i);
			for (auto& Object : GameObjects)
			{
				CTransform* pTransform = Object->Get_Component<CTransform>(L"Com_Transform");
				CModel* pModel = Object->Get_Component<CModel>(L"Com_Model");
				if (pTransform == nullptr)
					continue;

				if (pModel == nullptr)
					continue;

				for (auto& pMesh : pModel->Get_Meshes())
				{
					Vec4 vPosition;
					if (CPicking_Manager::GetInstance()->Is_Picking(pTransform, pMesh, false, &vPosition))
					{
						m_pSelectObj = Object;
						break;
					}
				}
			}
		}
	}

	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::RBTN))
	{
		if (nullptr == m_pSelectObj)
			return;

		for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
		{
			if (i == LAYER_TYPE::LAYER_CAMERA
				|| i == LAYER_TYPE::LAYER_TERRAIN
				|| i == LAYER_TYPE::LAYER_BACKGROUND
				|| i == LAYER_TYPE::LAYER_SKYBOX
				|| i == LAYER_TYPE::LAYER_UI
				|| i == LAYER_TYPE::LAYER_EFFECT)
				continue;

			list<CGameObject*>& GameObjects = GI->Find_GameObjects(LEVEL_TOOL, i);

			for (auto& Object : GameObjects)
			{
				CTransform* pTransform = Object->Get_Component<CTransform>(L"Com_Transform");
				CModel* pModel = Object->Get_Component<CModel>(L"Com_Model");
				if (pTransform == nullptr)
					continue;

				if (pModel == nullptr)
					continue;

				for (auto& pMesh : pModel->Get_Meshes())
				{
					Vec4 vPosition;

					if (true == m_bPlantMode)
					{
						if (CPicking_Manager::GetInstance()->Is_Picking(pTransform, pMesh, false, &vPosition))
						{
							CGameObject* pPlant = GI->Clone_GameObject(m_pSelectObj->Get_PrototypeTag(), LAYER_TYPE::LAYER_GRASS);

							CTransform* pPlantTransform = pPlant->Get_Component<CTransform>(L"Com_Transform");
							pPlantTransform->Set_State(CTransform::STATE::STATE_POSITION, ::XMLoadFloat4(&vPosition));
							GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_GRASS, pPlant);

							return;
						}
					}
					else
					{
						if (CPicking_Manager::GetInstance()->Is_Picking(pTransform, pMesh, false, &vPosition))
						{
							CTransform* pTargetTransform = m_pSelectObj->Get_Component<CTransform>(L"Com_Transform");
							pTargetTransform->Set_State(CTransform::STATE::STATE_POSITION, ::XMLoadFloat4(&vPosition));

							break;
						}
					}
				}
			}
		}
	}

}

void CTool_Map::MapObjectSpace()
{
	if (ImGui::CollapsingHeader("[ MapTool ]"))
	{

		if (ImGui::BeginChild("Child_List", ImVec2(0, 300.f), true))
		{
			if (ImGui::RadioButton("Buildings", (0 == m_iControlState)))
			{
				ChangeState();
				m_iControlState = 0;
			} ImGui::SameLine();
			if (ImGui::RadioButton("Props", (1 == m_iControlState)))
			{
				ChangeState();
				m_iControlState = 1;
			} ImGui::SameLine();
			if (ImGui::RadioButton("Grounds", (2 == m_iControlState)))
			{
				ChangeState();
				m_iControlState = 2;
			} ImGui::SameLine();
			if (ImGui::RadioButton("Plants", (3 == m_iControlState)))
			{
				ChangeState();
				m_iControlState = 3;
			} ImGui::SameLine();
			if (ImGui::RadioButton("TreeRock", (4 == m_iControlState)))
			{
				ChangeState();
				m_iControlState = 4;
			}
			if (ImGui::RadioButton("Dynamic", (5 == m_iControlState)))
			{
				ChangeState();
				m_iControlState = 5;
			}

			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), u8"오브젝트");
			if (nullptr != m_pSelectObj)
			{ 
				_bool iChange = 0;
				ImGui::PushItemWidth(50);

				string SelectObjectID = std::to_string(m_pSelectObj->Get_ObjectID());
				string SelectObjectTag = CUtils::ToString(m_pSelectObj->Get_ObjectTag());
				string SlectstrName = SelectObjectTag + "_" + SelectObjectID;

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"선택된 오브젝트 : "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), SlectstrName.c_str());

				if (ImGui::CollapsingHeader("Movement"))
				{
					int lastUsing = 0;

					CTransform* pTransform = m_pSelectObj->Get_Component<CTransform>(L"Com_Transform");
					if (nullptr == pTransform)
					{
						ImGui::End();
						return;
					}

#pragma region IMGUIZMO
#pragma endregion IMGUIZMO

					Vec3 vScaled = pTransform->Get_Scale();
					XMVECTOR vRotation = pTransform->Get_WorldRotation();
					XMMATRIX vPos = pTransform->Get_WorldMatrix();
					XMVECTOR vWorldPosition = vPos.r[3];

					ImGui::PushItemWidth(150.f);
					ImGui::DragFloat3("Position", &vWorldPosition.m128_f32[0], 0.1f, -1000.f, 1000.f);
					ImGui::DragFloat3("Rotation", &vRotation.m128_f32[0], 1.f);
					ImGui::DragFloat3("Scale", &vScaled.x, 0.01f, 0.01f, 100.f);
					ImGui::PopItemWidth();

					pTransform->Set_Scale(vScaled);
					pTransform->FixRotation(vRotation.m128_f32[0], vRotation.m128_f32[1], vRotation.m128_f32[2]);
					pTransform->Set_State(CTransform::STATE::STATE_POSITION, vWorldPosition);
				}

				if (ImGui::Button(u8"선택된 오브젝트 삭제"))
				{
					// TODO
					OBJ_TYPE eType = static_cast<OBJ_TYPE>(m_pSelectObj->Get_ObjectType());

					switch (eType)
					{
					case OBJ_TYPE::OBJ_BUILDING:
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_BUILDING);
						break;
					case OBJ_TYPE::OBJ_PROP:
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_PROP);
						break;
					case OBJ_TYPE::OBJ_GROUND:
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_GROUND);
						break;
					case OBJ_TYPE::OBJ_GRASS:
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_GRASS);
						break;
					case OBJ_TYPE::OBJ_TREEROCK:
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_TREEROCK);
						break;
					case OBJ_TYPE::OBJ_DYNAMIC:
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_DYNAMIC);
						break;
					}

				}
				ImGui::PopItemWidth();
			}
			else
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), u8"[ 현재 선택된 오브젝트가 없습니다.]");

			ImGui::SameLine();

			ImGui::PushItemWidth(110.f);

			static const char* szLevelType = nullptr;

			if (ImGui::BeginCombo(u8"Level_Type", szLevelType))
			{
				for (_uint i = 0; i < LEVEL_LIST_END; ++i)
				{
					_bool IsSelected = (szLevelType == m_ImguiSelectableNameList[i]);

					if (ImGui::Selectable(m_ImguiSelectableNameList[i], IsSelected))
					{
						szLevelType = m_ImguiSelectableNameList[i];
						m_iCurrentLevel = i;
						m_strLevelName = CUtils::ToWString(m_ImguiSelectableNameList[i]);
					}
				}
				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			if (ImGui::Button(u8"오브젝트 추가"))
			{
				m_pSelectObj = nullptr;
				m_bAddObject = true;

			}ImGui::SameLine();

			if (ImGui::Button(u8"배치된 오브젝트"))
				m_bAddObject = false;

			ImGui::SameLine();

			if (3 == m_iControlState)
			{
				if (ImGui::Checkbox(u8"다중 심기 모드", &m_bPlantMode))
				{
				}
			}

			if (true == m_bAddObject)
			{
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"추가할 오브젝트 선택");

				if (ImGui::ListBoxHeader("##ASSETLIST", ImVec2(300.0f, 0.0f)))
				{
					if (0 == m_iControlState)
						AddMapObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_BUILDING);
					else if (1 == m_iControlState)
						AddMapObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_PROP);
					else if (2 == m_iControlState)
						AddMapObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_GROUND);
					else if (3 == m_iControlState)
						AddMapObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_GRASS);
					else if (4 == m_iControlState)
						AddMapObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_TREEROCK);
					else if (5 == m_iControlState)
						AddMapObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_DYNAMIC);
				}

				ImGui::ListBoxFooter();
			}
			else
			{
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"맵에 배치된 오브젝트 리스트");
				if (ImGui::ListBoxHeader("##OBJECTLIST", ImVec2(300.0f, 0.0f)))
				{
					if (0 == m_iControlState)
						BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_BUILDING);
					else if (1 == m_iControlState)
						BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_PROP);
					else if (2 == m_iControlState)
						BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_GROUND);
					else if (3 == m_iControlState)
						BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_GRASS);
					else if (4 == m_iControlState)
						BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_TREEROCK);
					else if (5 == m_iControlState)
						BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_DYNAMIC);
				}

				ImGui::ListBoxFooter();
			}

			ImGui::Spacing();

			if (ImGui::Button(u8"Save"))
				Save_Map_Data(m_strLevelName);

			ImGui::SameLine();

			if (ImGui::Button(u8"Load"))
				Load_Map_Data(m_strLevelName);

		}
		ImGui::EndChild();
	}
}

void CTool_Map::MapLightSpace()
{
	if (ImGui::CollapsingHeader("[ Light Tool ]"))
	{

		if (ImGui::BeginChild("Light_Child_List", ImVec2(0, 300.f), true))
		{
			if (ImGui::RadioButton("DIRECTION", (0 == m_iLightControlState)))
			{
				ChangeState();
				m_iLightControlState = 0;
			} ImGui::SameLine();
			if (ImGui::RadioButton("POINT", (1 == m_iLightControlState)))
			{
				ChangeState();
				m_iLightControlState = 1;
			} ImGui::SameLine();
			if (ImGui::RadioButton("SPOT", (2 == m_iLightControlState)))
			{
				ChangeState();
				m_iLightControlState = 2;
			} ImGui::SameLine();

			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), u8"조명");
			if (nullptr != m_pSelectLight)
			{
				_bool iChange = 0;
				ImGui::PushItemWidth(250);

				string strSelectLightType = "";
				
				if (LIGHTDESC::TYPE::TYPE_POINT == m_pSelectLight->Get_LightDesc()->eType)
					strSelectLightType = "Point";
				else if (LIGHTDESC::TYPE::TYPE_DIRECTIONAL == m_pSelectLight->Get_LightDesc()->eType)
					strSelectLightType = "Directional";

				strSelectLightType += "_" + to_string(m_pSelectLight->Get_LightID());

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"선택된 라이트 : "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), strSelectLightType.c_str());

				if (ImGui::CollapsingHeader("LightInfo"))
				{
					LIGHTDESC* rLightDesc = m_pSelectLight->Get_ModifyLightDesc();

					if (ImGui::ColorEdit3("Diffuse", &rLightDesc->vDiffuse.x, ImGuiColorEditFlags_Float)) {} ImGui::NextColumn();
					if (ImGui::ColorEdit3("Ambient", &rLightDesc->vAmbient.x, ImGuiColorEditFlags_Float)) {} ImGui::NextColumn();
					if (ImGui::DragFloat3("Direction", &m_LightHelper.vRotationDeg.x, 1.f, -180.f, 180.f, "%.fdeg"))
					{
						XMVECTOR vRotQuat = ::XMQuaternionRotationRollPitchYaw(
							::XMConvertToRadians(m_LightHelper.vRotationDeg.x),
							::XMConvertToRadians(m_LightHelper.vRotationDeg.y),
							::XMConvertToRadians(m_LightHelper.vRotationDeg.z));

						XMVECTOR vRotQuatInverse = ::XMQuaternionInverse(vRotQuat);
						XMVECTOR vLightDir = ::XMQuaternionMultiply(vRotQuat, ::XMVectorSet(0.f, 0.f, 1.f, 0.f));
						::XMStoreFloat4(&rLightDesc->vDirection, ::XMQuaternionMultiply(vLightDir, vRotQuatInverse));
					}

					if (LIGHTDESC::TYPE::TYPE_POINT == rLightDesc->eType)
					{

					}
					else if (LIGHTDESC::TYPE::TYPE_DIRECTIONAL == rLightDesc->eType)
					{

					}
				}

				if (ImGui::Button(u8"선택된 조명 삭제"))
					DeleteLight(m_pSelectLight->Get_LightID());

				ImGui::PopItemWidth();
			}
			else
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), u8"[ 현재 선택된 조명이 없습니다.]");

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			if (ImGui::Button(u8"조명 추가"))
			{
				m_pSelectLight = nullptr;
				m_bAddLight = true;

			}ImGui::SameLine();

			if (ImGui::Button(u8"배치된 조명"))
				m_bAddLight = false;

			ImGui::SameLine();

			if (true == m_bAddLight)
			{
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"추가할 조명 선택");

				if (ImGui::ListBoxHeader("##ASSETLIST", ImVec2(300.0f, 0.0f)))
				{
					const string pLightName[3] = { "DIRECTIONAL", "POINT", "SPOT" };

					for (_uint i = 0; i < 3; ++i)
					{
						if (ImGui::Selectable(pLightName[i].c_str()))
						{
							LIGHTDESC lightDesc;
							::ZeroMemory(&lightDesc, sizeof(LIGHTDESC));

							lightDesc.vDiffuse = Vec4(1.f, 1.f, 1.f, 1.f);
							lightDesc.vAmbient = Vec4(1.f, 1.f, 1.f, 1.f);
							lightDesc.vDirection = Vec4(1.f, 1.f, 1.f, 1.f);

							if (0 == m_iControlState) //Direction
								lightDesc.eType = LIGHTDESC::TYPE::TYPE_DIRECTIONAL;

							GI->Add_Light(m_pDevice, m_pContext, lightDesc);
						}
					}

				}
				ImGui::ListBoxFooter();
			}
			else
			{
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"맵에 배치된 조명 리스트");
				if (ImGui::ListBoxHeader("##LIGHTLIST", ImVec2(300.0f, 0.0f)))
				{
					const list<class CLight*>* lightList = GI->Get_LightList();

					_uint iLightCount = lightList->size();

					for (auto& pLight : *lightList)
					{
						string strSelectLightType = "";

						if (0 == pLight->Get_LightDesc()->eType)
							strSelectLightType = "Point";
						else if (1 == pLight->Get_LightDesc()->eType)
							strSelectLightType = "Directional";

						strSelectLightType += "_" + to_string(pLight->Get_LightID());

						if (ImGui::Selectable(strSelectLightType.c_str()))
							m_pSelectLight = pLight;
					}


					//if (0 == m_iControlState)
					//	BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_BUILDING);
					//else if (1 == m_iControlState)
					//	BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_PROP);
					//else if (2 == m_iControlState)
					//	BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_GROUND);
					//else if (3 == m_iControlState)
					//	BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_GRASS);
					//else if (4 == m_iControlState)
					//	BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_TREEROCK);
				}

				ImGui::ListBoxFooter();
			}

			ImGui::Spacing();

			if (ImGui::Button(u8"Save"))
				Save_Light_Data(TEXT("Evermore Light"));

			ImGui::SameLine();

			if (ImGui::Button(u8"Load"))
				Load_Light_Data(TEXT("Evermore Light"));

		}
		ImGui::EndChild();
	}
}

void CTool_Map::MapMonsterSpace()
{
	if (ImGui::CollapsingHeader("[ Monster Tool ]"))
	{
		if (ImGui::BeginChild("Monster_Child_List", ImVec2(0, 300.f), true))
		{
			if (ImGui::RadioButton("Kind", (0 == m_iMonsterState)))
			{
				ChangeState();
				m_iMonsterState = 0;
			} ImGui::SameLine();

			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), u8"몬스터 오브젝트");
			if (nullptr != m_pSelectObj)
			{
				_bool iChange = 0;
				ImGui::PushItemWidth(50);

				string SelectObjectID = std::to_string(m_pSelectObj->Get_ObjectID());
				string SelectObjectTag = CUtils::ToString(m_pSelectObj->Get_ObjectTag());
				string SlectstrName = SelectObjectTag + "_" + SelectObjectID;

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"선택된 몬스터 : "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), SlectstrName.c_str());

				if (ImGui::CollapsingHeader("Movement"))
				{
					int lastUsing = 0;

					CTransform* pTransform = m_pSelectObj->Get_Component<CTransform>(L"Com_Transform");
					if (nullptr == pTransform)
					{
						ImGui::End();
						return;
					}

#pragma region IMGUIZMO
#pragma endregion IMGUIZMO

					Vec3 vScaled = pTransform->Get_Scale();
					XMVECTOR vRotation = pTransform->Get_WorldRotation();
					XMMATRIX vPos = pTransform->Get_WorldMatrix();
					XMVECTOR vWorldPosition = vPos.r[3];

					ImGui::PushItemWidth(150.f);
					ImGui::DragFloat3("Position", &vWorldPosition.m128_f32[0], 0.1f, -1000.f, 1000.f);
					ImGui::DragFloat3("Rotation", &vRotation.m128_f32[0], 1.f);
					ImGui::DragFloat3("Scale", &vScaled.x, 0.01f, 0.01f, 100.f);
					ImGui::PopItemWidth();

					pTransform->Set_Scale(vScaled);
					pTransform->FixRotation(vRotation.m128_f32[0], vRotation.m128_f32[1], vRotation.m128_f32[2]);
					pTransform->Set_State(CTransform::STATE::STATE_POSITION, vWorldPosition);
				}

				if (ImGui::Button(u8"선택된 몬스터 삭제"))
					DeleteObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_MONSTER);

				ImGui::PopItemWidth();
			}
			else
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), u8"[ 현재 선택된 몬스터가 없습니다.]");

			ImGui::SameLine();

			ImGui::PushItemWidth(110.f);

			static const char* szLevelType = nullptr;

			if (ImGui::BeginCombo(u8"Level_Type", szLevelType))
			{
				for (_uint i = 0; i < 2; ++i)
				{
					_bool IsSelected = (szLevelType == m_ImguiLevelName[i]);

					if (ImGui::Selectable(m_ImguiLevelName[i], IsSelected))
					{
						szLevelType = m_ImguiLevelName[i];
						m_iMonsterLevel = i;
						m_strLevelMonsterName = CUtils::ToWString(m_ImguiLevelName[i]);
					}
				}
				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			if (ImGui::Button(u8"몬스터 추가"))
			{
				m_pSelectObj = nullptr;
				m_bAddMonster = true;

			}ImGui::SameLine();

			if (ImGui::Button(u8"배치된 몬스터"))
				m_bAddMonster = false;

			if (true == m_bAddMonster)
			{
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"추가할 몬스터 선택");

				if (ImGui::ListBoxHeader("##ASSETLIST", ImVec2(300.0f, 0.0f)))
					AddMapMonster(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_MONSTER);

				ImGui::ListBoxFooter();
			}
			else
			{
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"맵에 배치된 몬스터 리스트");
				if (ImGui::ListBoxHeader("##OBJECTLIST", ImVec2(300.0f, 0.0f)))
				{
					if (0 == m_iMonsterState)
						BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_MONSTER);
				}

				ImGui::ListBoxFooter();
			}

			ImGui::Spacing();

			if (ImGui::Button(u8"Save"))
				Save_Monster_Data(m_strLevelMonsterName);

			ImGui::SameLine();

			if (ImGui::Button(u8"Load"))
				Load_Monster_Data(m_strLevelMonsterName);

		}
		ImGui::EndChild();
	}
}

void CTool_Map::MapNPCSpace()
{
	if (ImGui::CollapsingHeader("[ NPC Tool ]"))
	{
		if (ImGui::BeginChild("NPC_Child_List", ImVec2(0, 300.f), true))
		{
			if (ImGui::RadioButton("Kind", (0 == m_iNPCState)))
			{
				ChangeState();
				m_iNPCState = 0;
			} ImGui::SameLine();

			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), u8"NPC 오브젝트");
			if (nullptr != m_pSelectObj)
			{
				_bool iChange = 0;
				ImGui::PushItemWidth(50);

				string SelectObjectID = std::to_string(m_pSelectObj->Get_ObjectID());
				string SelectObjectTag = CUtils::ToString(m_pSelectObj->Get_ObjectTag());
				string SlectstrName = SelectObjectTag + "_" + SelectObjectID;

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"선택된 NPC : "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), SlectstrName.c_str());

				if (ImGui::CollapsingHeader("Movement"))
				{
					int lastUsing = 0;

					CTransform* pTransform = m_pSelectObj->Get_Component<CTransform>(L"Com_Transform");
					if (nullptr == pTransform)
					{
						ImGui::End();
						return;
					}

#pragma region IMGUIZMO
#pragma endregion IMGUIZMO

					Vec3 vScaled = pTransform->Get_Scale();
					XMVECTOR vRotation = pTransform->Get_WorldRotation();
					XMMATRIX vPos = pTransform->Get_WorldMatrix();
					XMVECTOR vWorldPosition = vPos.r[3];

					ImGui::PushItemWidth(150.f);
					ImGui::DragFloat3("Position", &vWorldPosition.m128_f32[0], 0.1f, -1000.f, 1000.f);
					ImGui::DragFloat3("Rotation", &vRotation.m128_f32[0], 1.f);
					ImGui::DragFloat3("Scale", &vScaled.x, 0.01f, 0.01f, 100.f);
					ImGui::PopItemWidth();

					pTransform->Set_Scale(vScaled);
					pTransform->FixRotation(vRotation.m128_f32[0], vRotation.m128_f32[1], vRotation.m128_f32[2]);
					pTransform->Set_State(CTransform::STATE::STATE_POSITION, vWorldPosition);
				}

				if (ImGui::Button(u8"선택된 NPC 삭제"))
					DeleteObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_NPC);

				ImGui::PopItemWidth();
			}
			else
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), u8"[ 현재 선택된 NPC가 없습니다.]");

			ImGui::SameLine();

			ImGui::PushItemWidth(110.f);

			static const char* szLevelType = nullptr;

			if (ImGui::BeginCombo(u8"Level_Type", szLevelType))
			{
				for (_uint i = 0; i < LEVEL_LIST_END; ++i)
				{
					_bool IsSelected = (szLevelType == m_ImguiSelectableNPCNameList[i]);

					if (ImGui::Selectable(m_ImguiSelectableNPCNameList[i], IsSelected))
					{
						szLevelType = m_ImguiSelectableNPCNameList[i];
						m_iMonsterLevel = i;
						m_strLevelNPCName = CUtils::ToWString(m_ImguiSelectableNPCNameList[i]);
					}
				}
				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			if (ImGui::Button(u8"NPC 추가"))
			{
				m_pSelectObj = nullptr;
				m_bAddNPC = true;

			}ImGui::SameLine();

			if (ImGui::Button(u8"배치된 NPC"))
				m_bAddNPC = false;

			if (true == m_bAddMonster)
			{
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"추가할 NPC 선택");

				if (ImGui::ListBoxHeader("##ASSETLIST", ImVec2(300.0f, 0.0f)))
					AddMapNPC(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_NPC);

				ImGui::ListBoxFooter();
			}
			else
			{
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"맵에 배치된 NPC 리스트");
				if (ImGui::ListBoxHeader("##OBJECTLIST", ImVec2(300.0f, 0.0f)))
				{
					if (0 == m_iNPCState)
						BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_NPC);
				}

				ImGui::ListBoxFooter();
			}

			ImGui::Spacing();

			if (ImGui::Button(u8"Save"))
				Save_NPC_Data(m_strLevelNPCName);

			ImGui::SameLine();

			if (ImGui::Button(u8"Load"))
				Load_NPC_Data(m_strLevelNPCName);

		}
		ImGui::EndChild();
	}
}

void CTool_Map::ChangeState()
{
	m_bAddObject = false;
	m_bAddMonster = false;
	m_bAddNPC = false;

	m_bPlantMode = false;
	m_pSelectObj = nullptr;

	m_pSelectLight = nullptr;
	m_bAddLight = false;
}


HRESULT CTool_Map::Save_Map_Data(const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L".map";

	auto path = filesystem::path(strMapFilePath);
	filesystem::create_directories(path.parent_path());


	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Write);
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
			|| i == LAYER_TYPE::LAYER_NPC)
			continue;


		//if (/*i == LAYER_TYPE::LAYER_TERRAIN ||*/
		//	i == LAYER_TYPE::LAYER_BUILDING ||
		//	i == LAYER_TYPE::LAYER_GRASS ||
		//	i == LAYER_TYPE::LAYER_GROUND ||
		//	i == LAYER_TYPE::LAYER_TREEROCK ||
		//	i == LAYER_TYPE::LAYER_PROP)
		{
			// 2. ObjectCount
			list<CGameObject*>& GameObjects = GI->Find_GameObjects(LEVEL_TOOL, i);
			File->Write<_uint>(GameObjects.size());

			for (auto& Object : GameObjects)
			{
				CTransform* pTransform = Object->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr == pTransform)
				{
					MSG_BOX("Find_Transform_Failed.");
					return E_FAIL;
				}

				// 3. Object_Prototype_Tag
				File->Write<string>(CUtils::ToString(Object->Get_PrototypeTag()));

				// 4. Object_Tag
				File->Write<string>(CUtils::ToString(Object->Get_ObjectTag()));

				// 5. Obejct States
				_float4 vRight, vUp, vLook, vPos;

				XMStoreFloat4(&vRight, pTransform->Get_State(CTransform::STATE_RIGHT));
				XMStoreFloat4(&vUp, pTransform->Get_State(CTransform::STATE_UP));
				XMStoreFloat4(&vLook, pTransform->Get_State(CTransform::STATE_LOOK));
				XMStoreFloat4(&vPos, pTransform->Get_State(CTransform::STATE_POSITION));

				File->Write<_float4>(vRight);
				File->Write<_float4>(vUp);
				File->Write<_float4>(vLook);
				File->Write<_float4>(vPos);
			}
		}
	}

	MSG_BOX("Map_Saved.");
	return S_OK;
}

HRESULT CTool_Map::Load_Map_Data(const wstring& strMapFileName)
{
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
					return E_FAIL;
				}

				if (nullptr == pObj)
				{
					MSG_BOX("Add_Object_Failed.");
					return E_FAIL;
				}
				pObj->Set_ObjectTag(strObjectTag);

				CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr == pTransform)
				{
					MSG_BOX("Get_Transform_Failed.");
					return E_FAIL;
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
	MSG_BOX("Map_Loaded.");
	return S_OK;

}

HRESULT CTool_Map::Save_Light_Data(const wstring& strLightFilePath)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strLightFilePath + L"/" + strLightFilePath + L".light";

	shared_ptr<CFileUtils> pFile = make_shared<CFileUtils>();
	pFile->Open(strMapFilePath, FileMode::Write);

	list<CLight*>* pLightList = GI->Get_LightList();
	pFile->Write<_uint>(pLightList->size());
	// 라이트 개수

	for (auto& pLight : *pLightList)
	{
		const LIGHTDESC* pLightDesc = pLight->Get_LightDesc();

		// Type
		pFile->Write<_uint>(pLightDesc->eType);

		if (LIGHTDESC::TYPE_DIRECTIONAL == pLightDesc->eType)
		{
			// ID
			pFile->Write<_uint>(pLight->Get_LightID());
			
			// State
			pFile->Write<Vec4>(pLightDesc->vDiffuse);
			pFile->Write<Vec4>(pLightDesc->vAmbient);
			pFile->Write<Vec4>(pLightDesc->vDirection);
		}
	}

	return S_OK;
}

HRESULT CTool_Map::Load_Light_Data(const wstring& strLightFilePath)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strLightFilePath + L"/" + strLightFilePath + L".light";

	shared_ptr<CFileUtils> pFile = make_shared<CFileUtils>();
	pFile->Open(strMapFilePath, FileMode::Read);

	_uint iLightSize = 0;
	pFile->Read<_uint>(iLightSize);
	// 라이트 개수

	for (_uint i = 0; i < iLightSize; ++i)
	{
		LIGHTDESC LightDesc;
		::ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

		// Type
		_uint iLightType = 0;
		_uint iLightID = 0;

		pFile->Read<_uint>(iLightType);

		if (LIGHTDESC::TYPE_DIRECTIONAL == iLightType)
		{
			// ID
			pFile->Read<_uint>(iLightID);

			// State
			Vec4 vDiffuse, vAmbient, vDirection;
			pFile->Read<Vec4>(vDiffuse);
			pFile->Read<Vec4>(vAmbient);
			pFile->Read<Vec4>(vDirection);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vDiffuse = vDiffuse;
			LightDesc.vAmbient = vAmbient;
			LightDesc.vDirection = vDirection;
		}

		if(FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CTool_Map::Save_Monster_Data(const wstring& strMonsterFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMonsterFileName + L"/" + strMonsterFileName + L"Monster.map";

	auto path = filesystem::path(strMapFilePath);
	filesystem::create_directories(path.parent_path());


	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Write);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_MONSTER != i)
			continue;

			// 2. ObjectCount
		list<CGameObject*>& GameObjects = GI->Find_GameObjects(LEVEL_TOOL, i);
		File->Write<_uint>(GameObjects.size());

		for (auto& Object : GameObjects)
		{
			CTransform* pTransform = Object->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTransform)
			{
				MSG_BOX("Find_Transform_Failed.");
				return E_FAIL;
			}

			// 3. Object_Prototype_Tag
			File->Write<string>(CUtils::ToString(Object->Get_PrototypeTag()));

			// 4. Object_Tag
			File->Write<string>(CUtils::ToString(Object->Get_ObjectTag()));

			// 5. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			XMStoreFloat4(&vRight, pTransform->Get_State(CTransform::STATE_RIGHT));
			XMStoreFloat4(&vUp, pTransform->Get_State(CTransform::STATE_UP));
			XMStoreFloat4(&vLook, pTransform->Get_State(CTransform::STATE_LOOK));
			XMStoreFloat4(&vPos, pTransform->Get_State(CTransform::STATE_POSITION));

			File->Write<_float4>(vRight);
			File->Write<_float4>(vUp);
			File->Write<_float4>(vLook);
			File->Write<_float4>(vPos);
		}
	}

	MSG_BOX("Monster_Saved.");
	return S_OK;
}

HRESULT CTool_Map::Load_Monster_Data(const wstring& strMonsterFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMonsterFileName + L"/" + strMonsterFileName + L"Monster.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_MONSTER != i)
			continue;

		GI->Clear_Layer(LEVEL_TOOL, i);

		
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
				return E_FAIL;
			}

			if (nullptr == pObj)
			{
				MSG_BOX("Add_Object_Failed.");
				return E_FAIL;
			}
			pObj->Set_ObjectTag(strObjectTag);

			CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTransform)
			{
				MSG_BOX("Get_Transform_Failed.");
				return E_FAIL;
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
	MSG_BOX("Monster_Loaded.");
	return S_OK;
}

HRESULT CTool_Map::Save_NPC_Data(const wstring& strNPCFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strNPCFileName + L"/" + strNPCFileName + L"NPC.map";

	auto path = filesystem::path(strMapFilePath);
	filesystem::create_directories(path.parent_path());


	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Write);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_NPC != i)
			continue;

		// 2. ObjectCount
		list<CGameObject*>& GameObjects = GI->Find_GameObjects(LEVEL_TOOL, i);
		File->Write<_uint>(GameObjects.size());

		for (auto& Object : GameObjects)
		{
			CTransform* pTransform = Object->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTransform)
			{
				MSG_BOX("Find_Transform_Failed.");
				return E_FAIL;
			}

			// 3. Object_Prototype_Tag
			File->Write<string>(CUtils::ToString(Object->Get_PrototypeTag()));

			// 4. Object_Tag
			File->Write<string>(CUtils::ToString(Object->Get_ObjectTag()));

			// 5. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			XMStoreFloat4(&vRight, pTransform->Get_State(CTransform::STATE_RIGHT));
			XMStoreFloat4(&vUp, pTransform->Get_State(CTransform::STATE_UP));
			XMStoreFloat4(&vLook, pTransform->Get_State(CTransform::STATE_LOOK));
			XMStoreFloat4(&vPos, pTransform->Get_State(CTransform::STATE_POSITION));

			File->Write<_float4>(vRight);
			File->Write<_float4>(vUp);
			File->Write<_float4>(vLook);
			File->Write<_float4>(vPos);
		}
	}

	MSG_BOX("NPC_Saved.");
	return S_OK;
}

HRESULT CTool_Map::Load_NPC_Data(const wstring& strNPCFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strNPCFileName + L"/" + strNPCFileName + L"NPC.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_MONSTER != i)
			continue;

		GI->Clear_Layer(LEVEL_TOOL, i);


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
				return E_FAIL;
			}

			if (nullptr == pObj)
			{
				MSG_BOX("Add_Object_Failed.");
				return E_FAIL;
			}
			pObj->Set_ObjectTag(strObjectTag);

			CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTransform)
			{
				MSG_BOX("Get_Transform_Failed.");
				return E_FAIL;
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
	MSG_BOX("Monster_Loaded.");
	return S_OK;
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
