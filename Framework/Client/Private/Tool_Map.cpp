#include "stdafx.h"
#include "Tool_Map.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "FileUtils.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Utils.h"
#include "Picking_Manager.h"
#include "Camera_Manager.h"
#include "Mesh.h"
#include "Light.h"
#include <filesystem>
//#include "ImGuizmo.h"
#include "PipeLine.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Water.h"
#include "Animals.h"
#include "CurlingGame_Manager.h"
#include "Trigger.h"
#include "Ruby.h"


CTool_Map::CTool_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool(pDevice, pContext)
{
}

HRESULT CTool_Map::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pRendererCom = dynamic_cast<CRenderer*>(GI->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer")));
	if (nullptr == m_pRendererCom)
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Ready_DebugDraw()))
		return E_FAIL;
#endif // _DEBUG


	return S_OK;
}

void CTool_Map::Tick(_float fTimeDelta)
{
	ImGuiWindowFlags WindowFlags = 0;
	//if (!m_bWindowResizeFlag)
	//	WindowFlags |= ImGuiWindowFlags_NoResize;
	//if (!m_bWindowMoveFlag)
	//	WindowFlags |= ImGuiWindowFlags_NoMove;

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

#pragma region MapWater
		MapWaterSpace();
#pragma endregion MapWater

#pragma region AnimalPatrol
		MapAnimalPatrol();
#pragma endregion AnimalPatrol


		Picking();

#pragma region Fog
		Map_Fog(fTimeDelta);
#pragma endregion Fog
	}
	




	ImGui::End();
}

HRESULT CTool_Map::Render()
{
#ifdef _DEBUG
	if(FAILED(Render_DebugDraw()))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
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
			|| Pair.first.find(TEXT("Animal_")) != std::wstring::npos
			|| Pair.first.find(TEXT("Water")) != std::wstring::npos
			|| Pair.first.find(TEXT("Sky")) != std::wstring::npos
			|| Pair.first.find(TEXT("Aurora")) != std::wstring::npos)
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
	const map<const std::wstring, CGameObject*>& vecPrototypeList = GI->Find_Prototype_GameObjects(iLayerType);

	for (auto& Pair : vecPrototypeList)
	{
		if (ImGui::Selectable(CUtils::ToString(Pair.first).c_str()))
		{
			if (true == m_bAddNPC)
				GI->Add_GameObject(iLevelID, iLayerType, Pair.first);
		}

	}
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

void CTool_Map::BatchWater(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
	list<CGameObject*>& pGameObjects = GI->Find_GameObjects(iLevelID, iLayerType);

	for (auto& pObj : pGameObjects)
	{
		if (pObj->Get_ObjectType() != OBJ_TYPE::OBJ_WATER)
			continue;

		string ObjectID = std::to_string(pObj->Get_ObjectID());
		string ObjectTag = CUtils::ToString(pObj->Get_ObjectTag());

		string strName = ObjectTag + "_" + ObjectID;

		if (ImGui::Selectable(strName.c_str()))
			m_pSelectObj = pObj;

	}
}

void CTool_Map::BatchAnimal(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
	list<CGameObject*>& pGameObjects = GI->Find_GameObjects(iLevelID, iLayerType);

	for (auto& pObj : pGameObjects)
	{
		if (pObj->Get_ObjectType() != OBJ_TYPE::OBJ_ANIMAL)
			continue;

		string ObjectID = std::to_string(pObj->Get_ObjectID());
		string ObjectTag = CUtils::ToString(pObj->Get_ObjectTag());

		string strName = ObjectTag + "_" + ObjectID;

		if (ImGui::Selectable(strName.c_str()))
			m_pSelectObj = pObj;

	}
}

void CTool_Map::RomingClear()
{
	if (nullptr == m_pSelectObj)
		return;

	CGameNpc* pNpc = static_cast<CGameNpc*>(m_pSelectObj);

	pNpc->Set_NpcState(CGameNpc::NPC_STATE::NPC_IDLE);

	vector<Vec4>* pPoints = pNpc->Get_RoamingArea();
	pPoints->clear();

	pNpc->Set_CurRoamingIndex(pPoints->size());
}

void CTool_Map::RomingPointDelete()
{
	if (nullptr == m_pSelectObj)
		return;
}

void CTool_Map::Animals_PointsClear()
{
	if (nullptr == m_pSelectObj)
		return;

	CAnimals* pAnimal = static_cast<CAnimals*>(m_pSelectObj);
	vector<Vec4>* pPoints = pAnimal->Get_RomingPoints();
	pPoints->clear();

	pAnimal->Set_CurRomingPoint(pPoints->size());
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
				|| i == LAYER_TYPE::LAYER_GROUND)
				continue;

			list<CGameObject*>& GameObjects = GI->Find_GameObjects(LEVEL_TOOL, i);
			for (auto& Object : GameObjects)
			{
				if (true == Object->Get_Enable())
					continue;

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
#pragma region Npc Not Roming
	if (m_iNPCState != 1 && m_iControlState != 7 && false == m_bAnimalPointPick && false == m_bPlantMode)
	{
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
	else if (true == m_bPlantMode)
	{
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

						if (CPicking_Manager::GetInstance()->Is_Picking(pTransform, pMesh, false, &vPosition))
						{
							CGameObject* pPlant = GI->Clone_GameObject(m_pSelectObj->Get_PrototypeTag(), LAYER_TYPE::LAYER_GRASS);

							CTransform* pPlantTransform = pPlant->Get_Component<CTransform>(L"Com_Transform");
							pPlantTransform->Set_State(CTransform::STATE::STATE_POSITION, ::XMLoadFloat4(&vPosition));
							GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_GRASS, pPlant);

							return;
						}
					}
				}
			}
		}
	}
#pragma endregion Npc Not Roming

#pragma region Npc Roming
	else if (m_iNPCState == 1)
	{
		if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::RBTN))
		{
			if (nullptr != m_pSelectObj)
			{
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

							if (CPicking_Manager::GetInstance()->Is_Picking(pTransform, pMesh, false, &vPosition))
							{
								CTransform* pTargetTransform = m_pSelectObj->Get_Component<CTransform>(L"Com_Transform");
								vector<Vec4>* pPoints = static_cast<CGameNpc*>(m_pSelectObj)->Get_RoamingArea();
								if (false == static_cast<CGameNpc*>(m_pSelectObj)->Get_TurnOnPoint())
								{
									pPoints->push_back(vPosition);
									pTargetTransform->Set_State(CTransform::STATE::STATE_POSITION, ::XMLoadFloat4(&vPosition));
									static_cast<CGameNpc*>(m_pSelectObj)->Set_Point(true);
									return;
								}
								else
								{
									pPoints->push_back(vPosition);
									return;
								}


								break;
							}
						}
					}
				}
			}
		}
	}
#pragma endregion Npc Roming
	else if (true == m_bAnimalPointPick)
	{
		if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::RBTN))
		{
			if (nullptr != m_pSelectObj)
			{
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

							if (CPicking_Manager::GetInstance()->Is_Picking(pTransform, pMesh, false, &vPosition))
							{
								CTransform* pTargetTransform = m_pSelectObj->Get_Component<CTransform>(L"Com_Transform");
								vector<Vec4>* pPoints = static_cast<CAnimals*>(m_pSelectObj)->Get_RomingPoints();
								if (false == static_cast<CAnimals*>(m_pSelectObj)->Get_TurnOnPoint())
								{
									pPoints->push_back(vPosition);
									pTargetTransform->Set_State(CTransform::STATE::STATE_POSITION, vPosition);
									static_cast<CAnimals*>(m_pSelectObj)->Set_TurnOnPoint(true);
									return;
								}
								else
								{
									pPoints->push_back(vPosition);
									return;
								}

								break;
							}
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
			} ImGui::SameLine();
			if (ImGui::RadioButton("SkyBox", (6 == m_iControlState)))
			{
				ChangeState();
				m_iControlState = 6;
			} ImGui::SameLine();

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
					case OBJ_TYPE::OBJ_SKY:
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_SKYBOX);
						break;
					case OBJ_TYPE::OBJ_WATER:
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_DYNAMIC);
						break;
					case OBJ_TYPE::OBJ_ANIMAL:
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_DYNAMIC);
						break;
					case OBJ_TYPE::OBJ_MINIGAME_STRUCTURE:
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_BUILDING);
						DeleteObject(LEVEL_TOOL, LAYER_TYPE::LAYER_PROP);
						break;
					}

				}

				ImGui::SameLine();

				if (ImGui::Button(u8"잔디 전부 삭제"))
				{
					list<CGameObject*>& pGameObjects = GI->Find_GameObjects(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_GRASS);

					auto iter = pGameObjects.begin();

					while (iter != pGameObjects.end())
					{
						if (nullptr == m_pSelectObj)
							break;

						if ((*iter)->Get_ObjectTag() == TEXT("Common_grass_01") || (*iter)->Get_ObjectTag() == TEXT("Common_grass_Small_01"))
						{
							Safe_Release<CGameObject*>((*iter));
							iter = pGameObjects.erase(iter);
						}
						else
							++iter;

					}
				}

				ImGui::PopItemWidth();
			}
			else
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), u8"[ 현재 선택된 오브젝트가 없습니다.]");

			ImGui::SameLine();

			if (m_iControlState != 7)
			{

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

				_bool bEnable = false;

				if (nullptr != m_pSelectObj)
					bEnable = m_pSelectObj->Get_Enable();

				if (ImGui::Checkbox("Enable", &bEnable))
					m_pSelectObj->Set_Enable(bEnable);

				ImGui::SameLine();

				_bool bIsQuest = false;

				if (nullptr != m_pSelectObj)
					bIsQuest = m_pSelectObj->IsQuestItem();

				if (ImGui::Checkbox("IsQuest", &bIsQuest))
					m_pSelectObj->Set_QuestItem(bIsQuest);

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
						else if (6 == m_iControlState)
							AddMapObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_SKYBOX);
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
						else if (6 == m_iControlState)
							BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_SKYBOX);
					}

					ImGui::ListBoxFooter();
				}

				ImGui::Spacing();

				if (ImGui::Button(u8"Save"))
					Save_Map_Data(m_strLevelName);

				ImGui::SameLine();

				if (ImGui::Button(u8"Load"))
					Load_Map_Data(m_strLevelName);

				if (ImGui::Button(u8"Dynamic_Save"))
					Save_Dynamic_Data(m_strLevelName);

				ImGui::SameLine();

				if (ImGui::Button(u8"Dynamic_Load"))
					Load_Dynamic_Data(m_strLevelName);
			}
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
			if (ImGui::RadioButton("LIGHT", (0 == m_iLightControlState)))
			{
				ChangeState();
				m_iLightControlState = 0;
			} /*ImGui::SameLine();
			if (ImGui::RadioButton("POINT", (1 == m_iLightControlState)))
			{
				ChangeState();
				m_iLightControlState = 1;
			} ImGui::SameLine();
			if (ImGui::RadioButton("SPOT", (2 == m_iLightControlState)))
			{
				ChangeState();
				m_iLightControlState = 2;
			} ImGui::SameLine();*/

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
				else if (LIGHTDESC::TYPE::TYPE_SPOT == m_pSelectLight->Get_LightDesc()->eType)
					strSelectLightType = "Spot";

				strSelectLightType += "_" + to_string(m_pSelectLight->Get_LightID());

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"선택된 라이트 : "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), strSelectLightType.c_str());

				if (ImGui::CollapsingHeader("LightInfo"))
				{
					LIGHTDESC* rLightDesc = m_pSelectLight->Get_ModifyLightDesc();

					if (LIGHTDESC::TYPE::TYPE_DIRECTIONAL == rLightDesc->eType)
					{
						if (ImGui::ColorEdit3("Diffuse", &rLightDesc->vTempColor.x, ImGuiColorEditFlags_Float)) {} ImGui::NextColumn();
						if (ImGui::ColorEdit3("AmbientLower", &rLightDesc->vAmbientLowerColor.x, ImGuiColorEditFlags_Float)) {} ImGui::NextColumn();
						if (ImGui::ColorEdit3("AmbientUpper", &rLightDesc->vAmbientUpperColor.x, ImGuiColorEditFlags_Float)) {} ImGui::NextColumn();
						if (ImGui::DragFloat3("Direction", &m_LightHelper.vRotationDeg.x, 1.f, -180.f, 180.f, "%.fdeg"))
						{
							XMVECTOR vRotQuat = ::XMQuaternionRotationRollPitchYaw(
								::XMConvertToRadians(m_LightHelper.vRotationDeg.x),
								::XMConvertToRadians(m_LightHelper.vRotationDeg.y),
								::XMConvertToRadians(m_LightHelper.vRotationDeg.z));

							XMVECTOR vRotQuatInverse = ::XMQuaternionInverse(vRotQuat);
							XMVECTOR vLightDir = ::XMQuaternionMultiply(vRotQuat, ::XMVectorSet(0.f, 0.f, 1.f, 0.f));
							::XMStoreFloat3(&rLightDesc->vTempDirection, ::XMQuaternionMultiply(vLightDir, vRotQuatInverse));
						}
					}

					if (LIGHTDESC::TYPE::TYPE_POINT == rLightDesc->eType)
					{
						if (ImGui::ColorEdit3("Diffuse", &rLightDesc->vTempColor.x, ImGuiColorEditFlags_Float)) {} ImGui::NextColumn();
						if (ImGui::DragFloat3("Position", &rLightDesc->vTempPosition.x)) {}
						if (ImGui::DragFloat("Range", &rLightDesc->fTempRange)) {} ImGui::NextColumn();
					}
					else if (LIGHTDESC::TYPE::TYPE_SPOT == rLightDesc->eType)
					{
						if (ImGui::ColorEdit3("Diffuse", &rLightDesc->vTempColor.x, ImGuiColorEditFlags_Float)) {} ImGui::NextColumn();
						if (ImGui::DragFloat3("Position", &rLightDesc->vTempPosition.x)) {}
						if (ImGui::DragFloat3("Direction", &m_LightHelper.vRotationDeg.x, 1.f, -180.f, 180.f, "%.fdeg"))
						{
							XMVECTOR vRotQuat = ::XMQuaternionRotationRollPitchYaw(
								::XMConvertToRadians(m_LightHelper.vRotationDeg.x),
								::XMConvertToRadians(m_LightHelper.vRotationDeg.y),
								::XMConvertToRadians(m_LightHelper.vRotationDeg.z));

							XMVECTOR vRotQuatInverse = ::XMQuaternionInverse(vRotQuat);
							XMVECTOR vLightDir = ::XMQuaternionMultiply(vRotQuat, ::XMVectorSet(0.f, 0.f, 1.f, 0.f));
							::XMStoreFloat3(&rLightDesc->vTempDirection, ::XMQuaternionMultiply(vLightDir, vRotQuatInverse));
						} ImGui::NextColumn();
						if (ImGui::DragFloat("Range", &rLightDesc->fTempRange, 0.1f, 1.0f, 100.0f)) {} ImGui::NextColumn();
						if (ImGui::DragFloat("OuterAngle", &rLightDesc->fOuterAngle, 0.1f, 1.0f, 100.0f)) {} ImGui::NextColumn();
						if (ImGui::DragFloat("InnerAngle", &rLightDesc->fInnerAngle, 0.1f, 1.0f, 100.0f)) {};
					}
				}

				if (ImGui::Button(u8"선택된 조명 삭제"))
					DeleteLight(m_pSelectLight->Get_LightID());

				ImGui::SameLine();

				if (ImGui::Checkbox("조명 거리 컬링", &m_pSelectLight->Get_ModifyLightDesc()->bNonCull)) {};

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

							if ("DIRECTIONAL" == pLightName[i])
							{
								lightDesc.vTempColor = Vec3(1.f, 1.f, 1.f);
								lightDesc.vAmbientLowerColor = Vec3(0.5f, 0.5f, 0.5f);
								lightDesc.vTempDirection = Vec3(1.f, -1.f, 1.f);
								lightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
							}
							else if ("POINT" == pLightName[i])
							{
								lightDesc.vTempColor = Vec3(1.0f, 1.0f, 1.0f);
								lightDesc.fTempRange = 5.0f;
								lightDesc.vTempPosition = Vec3(0.0f, 0.0f, 0.0f);
								lightDesc.eType = LIGHTDESC::TYPE::TYPE_POINT;
							}
							else if ("SPOT" == pLightName[i])
							{
								lightDesc.vTempPosition = Vec3(0.0f, 0.0f, 0.0f);
								lightDesc.vTempDirection = Vec3(0.0f, -1.0f, 0.0f);
								lightDesc.vTempColor = Vec3(1.0f, 1.0f, 1.0f);
								lightDesc.fTempRange = 60.0f;
								lightDesc.fOuterAngle = 65.0f;
								lightDesc.fInnerAngle = 55.0f;
								lightDesc.eType = LIGHTDESC::TYPE::TYPE_SPOT;
							}

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
						else if (2 == pLight->Get_LightDesc()->eType)
							strSelectLightType = "Spot";

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
				Save_Light_Data(m_strLevelName + TEXT(" Light"));

			ImGui::SameLine();

			if (ImGui::Button(u8"Load"))
				Load_Light_Data(m_strLevelName + TEXT(" Light"));

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
			if (ImGui::RadioButton("Patrol Setting", (1 == m_iNPCState)))
				m_iNPCState = 1;

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

		
			if (m_iNPCState == 0)
			{
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

				if (true == m_bAddNPC)
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
			else if (m_iNPCState == 1)
			{
				ImGuiStyle& imguiStyle = ImGui::GetStyle();

				const _float splitterButton = 10.0f;
				ImGui::Dummy(ImVec2(0.0f, splitterButton * 0.5f));
				ImGui::Indent(5.0f);
				ImGui::Text("NPC State Setting");
				ImGui::Unindent(5.0f);
				ImGui::Dummy(ImVec2(0.0f, splitterButton * 0.5f));

				ImGui::BeginChild("Settings", ImVec2(ImGui::GetWindowSize().x - (imguiStyle.ScrollbarSize / 1.5f),
					275.0f), true);
				ImGui::PushItemWidth(150.0f);

				if (ImGui::CollapsingHeader("Setting Panel", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Text("Setting Option");	ImGui::SameLine(ImGui::GetWindowWidth() - 90.0f);
					ImGui::Text("State");			ImGui::SameLine(ImGui::GetWindowWidth() - 123.0f);

					static const char* szLevelType = nullptr;

					if (ImGui::BeginCombo("##Set State", szLevelType))
					{
						for (_uint i = 0; i < CGameNpc::NPC_STATE::NPC_END; ++i)
						{
							_bool IsSelected = (szLevelType == m_NpcStateSelectableName[i]);

							if (ImGui::Selectable(m_NpcStateSelectableName[i], IsSelected))
							{
								if (nullptr == m_pSelectObj)
									break;

								// TODO NpcState
								// 세팅 해놓고 나중에 Get으로 저장할 때 불러오면 됨.

								if (m_NpcStateSelectableName[i] == "IDLE")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_IDLE);
								else if (m_NpcStateSelectableName[i] == "TALK")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_TALK);
								else if (m_NpcStateSelectableName[i] == "MOVE_ONEWAY")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_MOVE_ONEWAY);
								else if (m_NpcStateSelectableName[i] == "MOVE_TWOWAY")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_MOVE_TWOWAY);
								else if (m_NpcStateSelectableName[i] == "NPC_UNIQUENPC_WALK")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_UNIQUENPC_WALK);
								else if (m_NpcStateSelectableName[i] == "NPC_UNIQUENPC_RUN")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_UNIQUENPC_RUN);
								else if (m_NpcStateSelectableName[i] == "NPC_UNIQUENPC_TALK")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_UNIQUENPC_TALK);
								else if (m_NpcStateSelectableName[i] == "BACKDOWN")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_BACKDOWN);
								else if (m_NpcStateSelectableName[i] == "FRONTDOWN")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_FRONTDOWN);
								else if (m_NpcStateSelectableName[i] == "ATTACKIDLE")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_ATTACK_IDLE);
								else if (m_NpcStateSelectableName[i] == "ATTACK")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_ATTACK);
								else if (m_NpcStateSelectableName[i] == "NPC_UNIQUENPC_SEAT")
									static_cast<CGameNpc*>(m_pSelectObj)->Set_NpcState(CGameNpc::NPC_STATE::NPC_UNIQUENPC_SEAT);
							}
						}
						ImGui::EndCombo();
					}
					ImGui::Separator();

					ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.4f, 0.4f, 0.5f, 1.0f));

					if (ImGui::CollapsingHeader("Points", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::Separator();
						
						if (nullptr != m_pSelectObj)
						{
							vector<Vec4>* romingPoints = static_cast<CGameNpc*>(m_pSelectObj)->Get_RoamingArea();
							_uint iSize = romingPoints->size();

							for (_uint i = 0; i < iSize; ++i)
							{
								ImGui::DragFloat3(("Points_" + std::to_string(i)).c_str(), &(*romingPoints)[i].x); 
								ImGui::Spacing();
							}

							CGameNpc::NPC_STAT* eStat = static_cast<CGameNpc*>(m_pSelectObj)->Get_Stat();
							ImGui::DragFloat("Speed", &(eStat->fSpeed), 0.1f, 0.1f, 20.0f);

							if (ImGui::Button("Undo", ImVec2(80.0f, 20.0f)))
							{
								if (romingPoints->size() > 0)
								{
									romingPoints->pop_back();
									if (static_cast<CGameNpc*>(m_pSelectObj)->Get_CurRoamingIndex() >= romingPoints->size())
										static_cast<CGameNpc*>(m_pSelectObj)->Set_CurRoamingIndex(static_cast<CGameNpc*>(m_pSelectObj)->Get_CurRoamingIndex() - 1);
								}

							} ImGui::SameLine();

							if (ImGui::Button("All Romove", ImVec2(150.0f, 20.0f))) { RomingClear(); } ImGui::SameLine();
							
						}
					}

					ImGui::PopItemWidth();
					ImGui::PopStyleColor();
				}


				ImGui::EndChild();
			}
		}
		ImGui::EndChild();
	}
}

void CTool_Map::MapWaterSpace()
{
	if (ImGui::CollapsingHeader("[ Water Tool ]"))
	{
		ImGui::TextColored(ImVec4(0.05f, 0.2f, 0.8f, 1.f), u8"Ocean");

		if (m_pSelectObj != nullptr && m_pSelectObj->Get_ObjectType() == OBJ_WATER)
		{
			ImGui::PushID(m_pSelectObj->Get_ObjectTag().c_str() + m_pSelectObj->Get_ObjectID());
			ImGui::NextColumn();

			if (ImGui::CollapsingHeader("[ Water Info ]"))
			{
				ImGui::Separator();

				ImGui::NextColumn();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), u8"GerstnerWave Option");
				ImGui::NextColumn();

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), u8"VertexOption");
				
				CWater::VS_GerstnerWave& vsWave = static_cast<CWater*>(m_pSelectObj)->Get_VSGerstnerWave();
				Vec2 A = vsWave.A;
				Vec2 F = vsWave.F;
				Vec2 S = vsWave.S;
				Vec4 D = vsWave.D;

				if (ImGui::DragFloat2("VsWaveAmplitude", &A.x, 0.001f, 0.001f, 10.0f))
				{
					vsWave.A.x = A.x;
					vsWave.A.y = A.y;
				}

				if (ImGui::DragFloat2("VsWaveFrequency", &F.x, 0.001f, 0.001f, 10.0f))
				{
					vsWave.F.x = F.x;
					vsWave.F.y = F.y;
				}

				if (ImGui::DragFloat2("VsWaveSteepness", &S.x, 0.001f, 0.001f, 10.0f))
				{
					vsWave.S.x = S.x;
					vsWave.S.y = S.y;
				}

				if (ImGui::DragFloat4("VsWaveDir", &D.x, 0.001f, 0.001f, 10.0f))
				{
					vsWave.D.x = D.x;
					vsWave.D.y = D.y;
					vsWave.D.z = D.z;
					vsWave.D.w = D.w;
				}

				_float& Damper = static_cast<CWater*>(m_pSelectObj)->Get_Damper();

				ImGui::DragFloat("VsDamper", &Damper, 0.001f, 0.001f, 10.0f);
					

				ImGui::NextColumn();

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), u8"PixelOption");


				CWater::PS_GerstnerWave& psWave = static_cast<CWater*>(m_pSelectObj)->Get_PSGerstnerWave();

				_float fresnelBias = psWave.fresnelBias;
				_float fresnelPower = psWave.fresnelPower;
				_float waterAmount = psWave.waveAmount;
				_float reflectAmount = psWave.reflectAmount;

				_float bumpScale = psWave.bumpScale;

				Vec4 ShallowWaterColor = psWave.vShallowWaterColor;
				Vec4 DeepWaterColor = psWave.vDeepWaterColor;

				if (ImGui::DragFloat("PsfresnelBias", &fresnelBias, 0.001f, 0.001f, 10.0f))
					psWave.fresnelBias = fresnelBias;

				if (ImGui::DragFloat("PsfresnelPower", &fresnelPower, 0.001f, 0.001f, 10.0f))
					psWave.fresnelPower = fresnelPower;

				if (ImGui::DragFloat("PswaterAmount", &waterAmount, 0.001f, 0.001f, 10.0f))
					psWave.waveAmount = waterAmount;

				if (ImGui::DragFloat("PsreflectAmount", &reflectAmount, 0.001f, 0.001f, 10.0f))
					psWave.reflectAmount = reflectAmount;

				if (ImGui::DragFloat("PsbumpScale", &bumpScale, 0.001f, 0.001f, 10.0f))
					psWave.bumpScale = bumpScale;

				if (ImGui::ColorEdit3("PsShallowWaterColor", &ShallowWaterColor.x, 0.001f))
					psWave.vShallowWaterColor = ShallowWaterColor;

				if (ImGui::ColorEdit3("PsDeepWaterColor", &DeepWaterColor.x, 0.001f))
					psWave.vDeepWaterColor = DeepWaterColor;
			}

			ImGui::PopID();
		}
		
	}
}

void CTool_Map::MapAnimalPatrol()
{
	m_bAnimalPointPick = false;

	if (ImGui::CollapsingHeader("[ Animals Patrol ]"))
	{
		ImGuiStyle& imguiStyle = ImGui::GetStyle();
		m_bAnimalPointPick = true;

		const _float splitterButton = 10.0f;
		ImGui::Dummy(ImVec2(0.0f, splitterButton * 0.5f));
		ImGui::Indent(5.0f);
		ImGui::Text("Animals Patrol Setting");
		ImGui::Unindent(5.0f);
		ImGui::Dummy(ImVec2(0.0f, splitterButton * 0.5f));

		if (ImGui::BeginChild("Child_Animals", ImVec2(0.0f, 600.0f), true))
		{
			if (ImGui::ListBoxHeader("##Animals List", ImVec2(300.0f, 0.0f)))
				BatchAnimal(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_DYNAMIC);

			ImGui::ListBoxFooter();
		}

		ImGui::PushItemWidth(150.0f);
		if (ImGui::CollapsingHeader("Points", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Separator();

			if (nullptr != m_pSelectObj)
			{
				vector<Vec4>* romingPoints = static_cast<CAnimals*>(m_pSelectObj)->Get_RomingPoints();
				_uint iSize = romingPoints->size();

				for (_uint i = 0; i < iSize; ++i)
				{
					ImGui::DragFloat3(("Points_" + std::to_string(i)).c_str(), &(*romingPoints)[i].x);
					ImGui::Spacing();
				}

				_float* fSpeed = static_cast<CAnimals*>(m_pSelectObj)->Get_Speed();
				ImGui::DragFloat("Speed", fSpeed, 0.1f, 0.1f, 20.0f);

				if (ImGui::Button("Undo", ImVec2(80.0f, 20.0f)))
				{
					if (romingPoints->size() > 0)
					{
						romingPoints->pop_back();
						if (static_cast<CAnimals*>(m_pSelectObj)->Get_CurRomingPoint() >= romingPoints->size())
							static_cast<CGameNpc*>(m_pSelectObj)->Set_CurRoamingIndex(static_cast<CAnimals*>(m_pSelectObj)->Get_CurRomingPoint() - 1);
					}

				} ImGui::SameLine();

				if (ImGui::Button("All Romove", ImVec2(150.0f, 20.0f))) { Animals_PointsClear(); } ImGui::SameLine();

			}
		}
		ImGui::PopItemWidth();

		ImGui::EndChild();
	}
}

void CTool_Map::Map_Fog(_float fTimeDelta)
{
	CRenderer::FOG_DESC FogDesc = m_pRendererCom->Get_FogDesc();

	
	// m_pRendererCom->c
	_float4 vFogColor = m_pRendererCom->Get_FogColor();
	ImGui::DragFloat4("FogColor", (_float*)&vFogColor, 0.01f);
	m_pRendererCom->Set_FogColor(vFogColor);

	

	static _float2 vUVSpeed = { 0.f, 0.f };
	ImGui::DragFloat2("UV_Speed", (_float*)&vUVSpeed, 0.01f);

	FogDesc.fUVAcc.x += vUVSpeed.x * fTimeDelta;
	FogDesc.fUVAcc.y += vUVSpeed.y * fTimeDelta;

	ImGui::DragFloat("FogDistanceValue", &FogDesc.fFogDistanceValue, 0.01f);
	ImGui::DragFloat("FogHeightValue", &FogDesc.fFogHeightValue, 0.01f);
	ImGui::DragFloat("FogStartDepth", &FogDesc.fFogStartDepth, 0.01f);
	ImGui::DragFloat("FogStartDistance", &FogDesc.fFogStartDistance, 0.01f);

	ImGui::DragFloat("FogDistanceDensity", &FogDesc.fFogDistanceDensity, 0.01f);
	ImGui::DragFloat("FogHeightDensity", &FogDesc.fFogHeightDensity, 0.01f);
	

	

	m_pRendererCom->Set_FogDesc(FogDesc);
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
			|| i == LAYER_TYPE::LAYER_UI
			|| i == LAYER_TYPE::LAYER_PLAYER
			|| i == LAYER_TYPE::LAYER_MONSTER
			|| i == LAYER_TYPE::LAYER_WEAPON
			|| i == LAYER_TYPE::LAYER_PROJECTILE
			|| i == LAYER_TYPE::LAYER_EFFECT
			|| i == LAYER_TYPE::LAYER_TRAIL
			|| i == LAYER_TYPE::LAYER_NPC
			|| i == LAYER_TYPE::LAYER_CHARACTER
			|| i == LAYER_TYPE::LAYER_DYNAMIC)
			continue;

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

				// 6. Quest Item
				_bool IsQuest = Object->IsQuestItem();
				File->Write<_bool>(IsQuest);
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

	m_pSelectObj = nullptr;

	GI->Clear_PhysX_Ground();
	GI->Clear_Layer(LEVEL_TOOL, LAYER_TYPE::LAYER_DYNAMIC);

	vector<CGameObject*>* pStadium = CCurlingGame_Manager::GetInstance()->Get_Stadium();
	pStadium->clear();

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (i == LAYER_TYPE::LAYER_CAMERA
			|| i == LAYER_TYPE::LAYER_TERRAIN
			|| i == LAYER_TYPE::LAYER_BACKGROUND
			|| i == LAYER_TYPE::LAYER_UI
			|| i == LAYER_TYPE::LAYER_PLAYER
			|| i == LAYER_TYPE::LAYER_WEAPON
			|| i == LAYER_TYPE::LAYER_PROJECTILE
			|| i == LAYER_TYPE::LAYER_EFFECT
			|| i == LAYER_TYPE::LAYER_TRAIL
			|| i == LAYER_TYPE::LAYER_NPC
			|| i == LAYER_TYPE::LAYER_WEAPON
			|| i == LAYER_TYPE::LAYER_MONSTER
			|| i == LAYER_TYPE::LAYER_CHARACTER
			|| i == LAYER_TYPE::LAYER_DYNAMIC)
			continue;

		GI->Clear_Layer(LEVEL_TOOL, i);
		{
			_uint iObjectCount = File->Read<_uint>();

			for (_uint j = 0; j < iObjectCount; ++j)
			{
				// 3. Object_Prototype_Tag
				wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
				wstring strObjectTag = CUtils::ToWString(File->Read<string>());

				// 6. Obejct States
				_float4 vRight, vUp, vLook, vPos;

				File->Read<_float4>(vRight);
				File->Read<_float4>(vUp);
				File->Read<_float4>(vLook);
				File->Read<_float4>(vPos);

				OBJECT_INIT_DESC Init_Data = {};
				Init_Data.vStartPosition = vPos;

				CGameObject* pObj = nullptr;

				if (FAILED(GI->Add_GameObject(LEVEL_TOOL, i, strPrototypeTag, &Init_Data, &pObj)))
				{
					MSG_BOX("Load_Map_Objects_Failed.");
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

				pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
				pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
				pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
				pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));

				_bool IsQuest;
				File->Read<_bool>(IsQuest);  
				pObj->Set_QuestItem(IsQuest);

				if (OBJ_TYPE::OBJ_MINIGAME_STRUCTURE == pObj->Get_ObjectType())
					pStadium->push_back(pObj);
			}
		}
	}

	list<CGameObject*> Grounds = GI->Find_GameObjects(LEVEL_TOOL, LAYER_TYPE::LAYER_GROUND);
	for (auto& Ground : Grounds)
	{
		if (OBJ_TYPE::OBJ_MINIGAME_STRUCTURE == Ground->Get_ObjectType())
			continue;

		if (FAILED(GI->Add_Ground(Ground,
			Ground->Get_Component<CModel>(L"Com_Model"),
			Ground->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("맵툴에서 피직스 생성에 실패했습니다.");
		}
	}

	list<CGameObject*> Buildings = GI->Find_GameObjects(LEVEL_TOOL, LAYER_TYPE::LAYER_BUILDING);
	for (auto& Building : Buildings)
	{
		if (OBJ_TYPE::OBJ_MINIGAME_STRUCTURE == Building->Get_ObjectType())
			continue;

		if (FAILED(GI->Add_Building(Building,
			Building->Get_Component<CModel>(L"Com_Model"),
			Building->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix())))
		{
			MSG_BOX("피직스 빌딩 생성에 실패했습니다.");
		}
	}

	CHARACTER_TYPE eCharacterType;

	switch (g_ePlayCharacter)
	{
	case LOAD_CHARACTER_TYPE::SWORDMAN_CH:
		eCharacterType = CHARACTER_TYPE::SWORD_MAN;
		break;
	case LOAD_CHARACTER_TYPE::DESTROYER_CH:
		eCharacterType = CHARACTER_TYPE::DESTROYER;
		break;
	case LOAD_CHARACTER_TYPE::ENGINEER_CH:
		eCharacterType = CHARACTER_TYPE::ENGINEER;
		break;
	default:
		eCharacterType = CHARACTER_TYPE::SWORD_MAN;
		break;
	}

	if (FAILED(CGame_Manager::GetInstance()->Get_Player()->Set_Character(eCharacterType, Vec4(0.f, 0.f, 0.f, 1.f), false)))
		return E_FAIL;

	if (!CCamera_Manager::GetInstance()->Is_Empty_Camera(CAMERA_TYPE::FOLLOW))
	{
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(CGame_Manager::GetInstance()->Get_Player()->Get_Character());
	}

	//CTrigger::TRIGGER_DESC TriggerDesc;
	//TriggerDesc.eTriggerType = TRIGGER_TYPE::TRIGGER_WITCH_WOOD_ENTER;
	//TriggerDesc.strMapName = L"";
	//TriggerDesc.vStartPosition = { 101.275f, -4.906f, 28.147f, 1.f };
	//TriggerDesc.vExtents = { 5.f, 5.f, 5.f };

	//if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_PROP, TEXT("Prototype_GameObject_Trigger"), &TriggerDesc)))
	//	return E_FAIL;


	MSG_BOX("Map_Loaded.");
	return S_OK;

}

HRESULT CTool_Map::Save_Dynamic_Data(const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L"Dynamic.map";

	auto path = filesystem::path(strMapFilePath);
	filesystem::create_directories(path.parent_path());


	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Write);

		// 2. ObjectCount
	list<CGameObject*>& GameObjects = GI->Find_GameObjects(LEVEL_TOOL, LAYER_DYNAMIC);
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

		_uint objectType = Object->Get_ObjectType();
		File->Write<_uint>(objectType);

		if (Object->Get_ObjectType() == OBJ_TYPE::OBJ_WATER)
		{
			CWater::VS_GerstnerWave vsWave = static_cast<CWater*>(Object)->Get_VSGerstnerWave();
			File->Write<CWater::VS_GerstnerWave>(vsWave);
			CWater::PS_GerstnerWave psWave = static_cast<CWater*>(Object)->Get_PSGerstnerWave();
			File->Write<CWater::PS_GerstnerWave>(psWave);
			_float damp = static_cast<CWater*>(Object)->Get_Damper();
			File->Write<_float>(damp);
		}
		else if (Object->Get_ObjectType() == OBJ_TYPE::OBJ_ANIMAL)
		{
			vector<Vec4>* pPoints = static_cast<CAnimals*>(Object)->Get_RomingPoints();
			File->Write<_uint>(pPoints->size());

			if (pPoints->size() != 0)
			{
				CAnimals* pAnimals = static_cast<CAnimals*>(Object);

				for (auto& iter : *pPoints)
					File->Write<Vec4>(iter); // 0 
				
				_float* pSpeed = pAnimals->Get_Speed();
				File->Write<_float>(*pSpeed); // 0 
			}
		}
	}
	
	MSG_BOX("Dynamic_Saved.");
	return S_OK;
}

HRESULT CTool_Map::Load_Dynamic_Data(const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L"Dynamic.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	m_pSelectObj = nullptr;

	GI->Clear_Layer(LEVEL_TOOL, LAYER_TYPE::LAYER_DYNAMIC);


	_uint iObjectCount = File->Read<_uint>();
	for (_uint j = 0; j < iObjectCount; ++j)
	{
		// 3. Object_Prototype_Tag
		wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
		wstring strObjectTag = CUtils::ToWString(File->Read<string>());

		// 6. Obejct States
		_float4 vRight, vUp, vLook, vPos;

		File->Read<_float4>(vRight);
		File->Read<_float4>(vUp);
		File->Read<_float4>(vLook);
		File->Read<_float4>(vPos);

		_uint objectType;
		File->Read<_uint>(objectType);


		OBJECT_INIT_DESC Init_Data = {};
		Init_Data.vStartPosition = vPos;
		CGameObject* pObj = nullptr;
		if (FAILED(GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_DYNAMIC, strPrototypeTag, &Init_Data, &pObj)))
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

		if (pObj->Get_ObjectType() == OBJ_TYPE::OBJ_WATER)
		{
			CWater::VS_GerstnerWave vsWave;
			File->Read<CWater::VS_GerstnerWave>(vsWave);
			CWater::PS_GerstnerWave psWave;
			File->Read<CWater::PS_GerstnerWave>(psWave);
			_float damp;
			File->Read<_float>(damp);

			static_cast<CWater*>(pObj)->Set_VSGerstnerWave(vsWave);
			static_cast<CWater*>(pObj)->Set_PSGerstnerWave(psWave);
			static_cast<CWater*>(pObj)->Set_Damper(damp);
		}
		else if (pObj->Get_ObjectType() == OBJ_TYPE::OBJ_ANIMAL)
		{
			_uint iSize;
			File->Read<_uint>(iSize);

			if (iSize != 0)
			{
				CAnimals* pAnimals = static_cast<CAnimals*>(pObj);
				vector<Vec4> Points;
				Points.reserve(iSize);

				for (_uint i = 0; i < iSize; ++i)
				{
					Vec4 vPoint;
					File->Read<Vec4>(vPoint);
					Points.push_back(vPoint);
				}

				pAnimals->Set_RomingPoints(Points);
		
				_float* pSpeed = pAnimals->Get_Speed();
				File->Read<_float>(*pSpeed); // 0

				vPos = Points.front();
			}
		}

		pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
		pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
		pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
		pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
	}

	MSG_BOX("Dynamic_Loaded.");
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
			pFile->Write<Vec3>(pLightDesc->vTempColor);
			pFile->Write<Vec3>(pLightDesc->vAmbientLowerColor);
			pFile->Write<Vec3>(pLightDesc->vAmbientUpperColor);
			pFile->Write<Vec3>(pLightDesc->vTempDirection);
		}
		else if (LIGHTDESC::TYPE_POINT == pLightDesc->eType)
		{
			// ID
			pFile->Write<_uint>(pLight->Get_LightID());

			// State
			pFile->Write<Vec3>(pLightDesc->vTempPosition);
			pFile->Write<_float>(pLightDesc->fTempRange);
			pFile->Write<Vec3>(pLightDesc->vTempColor);
			pFile->Write<_bool>(pLightDesc->bNonCull);
		}
		else if (LIGHTDESC::TYPE::TYPE_SPOT == pLightDesc->eType)
		{
			pFile->Write<_uint>(pLight->Get_LightID());

			pFile->Write<Vec3>(pLightDesc->vTempPosition);
			pFile->Write<Vec3>(pLightDesc->vTempDirection);
			pFile->Write<Vec3>(pLightDesc->vTempColor);
			pFile->Write<_float>(pLightDesc->fTempRange);
			pFile->Write<_float>(pLightDesc->fOuterAngle);
			pFile->Write<_float>(pLightDesc->fInnerAngle);
			pFile->Write<_bool>(pLightDesc->bNonCull);
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
	list<CLight*>* pLightlist = GI->Get_LightList();
	for (auto& pLight : *pLightlist)
		Safe_Release<CLight*>(pLight);

	pLightlist->clear();

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
			Vec3 vDiffuse, vAmbientLowerColor, vAmbientUpperColor, vDirection;
			pFile->Read<Vec3>(vDiffuse);
			pFile->Read<Vec3>(vAmbientLowerColor);
			pFile->Read<Vec3>(vAmbientUpperColor);
			pFile->Read<Vec3>(vDirection);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempColor = vDiffuse;
			LightDesc.vAmbientLowerColor = vAmbientLowerColor;
			LightDesc.vAmbientUpperColor = vAmbientUpperColor;
			LightDesc.vTempDirection = vDirection;
		}
		else if (LIGHTDESC::TYPE_POINT == iLightType)
		{
			pFile->Read<_uint>(iLightID);

			// State
			Vec3 vPos, vColor;
			_float fRange;
			_bool bNonCull;
			pFile->Read<Vec3>(vPos);
			pFile->Read<_float>(fRange);
			pFile->Read<Vec3>(vColor);
			pFile->Read	<_bool>(bNonCull);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempPosition = vPos;
			LightDesc.fTempRange = fRange;
			LightDesc.vTempColor = vColor;
			LightDesc.bNonCull = bNonCull;
		}
		else if (LIGHTDESC::TYPE_SPOT == iLightType)
		{
			pFile->Read<_uint>(iLightID);

			Vec3 vPos, vColor, vDirection;
			_float fTempRange, fOuterAngle, fInnerAngle;
			_bool bNonCull;
			pFile->Read<Vec3>(vPos);
			pFile->Read<Vec3>(vDirection);
			pFile->Read<Vec3>(vColor);
			pFile->Read<_float>(fTempRange);
			pFile->Read<_float>(fOuterAngle);
			pFile->Read<_float>(fInnerAngle);
			pFile->Read<_bool>(bNonCull);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempPosition = vPos;
			LightDesc.vTempDirection = vDirection;
			LightDesc.vTempColor = vColor;
			LightDesc.fTempRange = fTempRange;
			LightDesc.fOuterAngle = fOuterAngle;
			LightDesc.fInnerAngle = fInnerAngle;
			LightDesc.bNonCull = bNonCull;
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

			// 6. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			File->Read<_float4>(vRight);
			File->Read<_float4>(vUp);
			File->Read<_float4>(vLook);
			File->Read<_float4>(vPos);


			OBJECT_INIT_DESC Init_Data = {};
			Init_Data.vStartPosition = vPos;
			CGameObject* pObj = nullptr;

			if (FAILED(GI->Add_GameObject(LEVEL_TOOL, i, strPrototypeTag, &Init_Data, &pObj)))
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
		File->Write<_uint>(GameObjects.size()); // O

 		for (auto& Object : GameObjects)
		{
			CTransform* pTransform = Object->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTransform)
			{
				MSG_BOX("Find_Transform_Failed.");
				return E_FAIL;
			}

			// 3. Object_Prototype_Tag
			File->Write<string>(CUtils::ToString(Object->Get_PrototypeTag())); // O
			 
			// 4. Object_Tag
			File->Write<string>(CUtils::ToString(Object->Get_ObjectTag())); // 0 

			// 5. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			XMStoreFloat4(&vRight, pTransform->Get_State(CTransform::STATE_RIGHT));
			XMStoreFloat4(&vUp, pTransform->Get_State(CTransform::STATE_UP));
			XMStoreFloat4(&vLook, pTransform->Get_State(CTransform::STATE_LOOK));
			XMStoreFloat4(&vPos, pTransform->Get_State(CTransform::STATE_POSITION));

			File->Write<_float4>(vRight); // 0 
			File->Write<_float4>(vUp); // 0 
			File->Write<_float4>(vLook); // 0 
			File->Write<_float4>(vPos); // 0 

			_uint ObjectType = Object->Get_ObjectType();
			File->Write<_uint>(ObjectType); // 0 

			if (OBJ_TYPE::OBJ_NPC == ObjectType)
			{
				CGameNpc* pNpc = static_cast<CGameNpc*>(Object);
				vector<Vec4>* pPoints = pNpc->Get_RoamingArea();

				_uint iSize = pPoints->size();
				File->Write<_uint>(iSize); // 0 

				_uint eState = pNpc->Get_State();

				if (eState == CGameNpc::NPC_STATE::NPC_END)
					eState = CGameNpc::NPC_STATE::NPC_IDLE;

				File->Write<_uint>(eState); // 0 

				if (pPoints->size() != 0)
				{
					for (auto& iter : *pPoints)
						File->Write<Vec4>(iter); // 0 
				}

				CGameNpc::NPC_STAT* eStat = pNpc->Get_Stat();
				File->Write<CGameNpc::NPC_STAT>(*eStat); // 0 
			}
		}
	}

	MSG_BOX("NPC_Saved.");
	return S_OK;
}

HRESULT CTool_Map::Load_NPC_Data(const wstring& strNPCFileName)
{
	m_pSelectObj = nullptr;

	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strNPCFileName + L"/" + strNPCFileName + L"NPC.map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_NPC != i)
			continue;

		GI->Clear_Layer(LEVEL_TOOL, i);


		_uint iObjectCount = File->Read<_uint>(); // 0 

		for (_uint j = 0; j < iObjectCount; ++j)
		{
			// 3. Object_Prototype_Tag
			wstring strPrototypeTag = CUtils::ToWString(File->Read<string>()); // 0 
			wstring strObjectTag = CUtils::ToWString(File->Read<string>()); // 0 

			// 6. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			File->Read<_float4>(vRight); // 0 
			File->Read<_float4>(vUp); // 0 
			File->Read<_float4>(vLook); // 0 
			File->Read<_float4>(vPos); // 0 

			 
			OBJECT_INIT_DESC Init_Data = {};
			Init_Data.vStartPosition = vPos;
			CGameObject* pObj = nullptr;
			if (FAILED(GI->Add_GameObject(LEVEL_TOOL, i, strPrototypeTag, &Init_Data, &pObj)))
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

			_uint ObjectType;
			File->Read<_uint>(ObjectType); // 0 

			if (OBJ_TYPE::OBJ_NPC == ObjectType)
			{
				CGameNpc* pNpc = static_cast<CGameNpc*>(pObj);
				
				_uint iSize;
				File->Read<_uint>(iSize); // 0 

				_uint eState;
				File->Read<_uint>(eState); // 0 


				if (iSize != 0)
				{
					vector<Vec4> Points;
					Points.reserve(iSize);

					for (_uint i = 0; i < iSize; ++i)
					{
						Vec4 vPoint;
						File->Read<Vec4>(vPoint); // 0 
						Points.push_back(vPoint);
					}

					pNpc->Set_RoamingArea(Points);

					if (Points.size() != 0)
					{
						vPos = Points.front();
						pNpc->Set_Point(true);
					}
				}

				CGameNpc::NPC_STAT eStat;
				File->Read<CGameNpc::NPC_STAT>(eStat); // 0 

				pNpc->Set_NpcState(static_cast<CGameNpc::NPC_STATE>(eState));
				pNpc->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(eState);
				pNpc->Set_Stat(eStat);
			}


			pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
			pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
			pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
			pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
		}

	}
	MSG_BOX("Npc_Loaded.");
	return S_OK;
}

#ifdef _DEBUG
HRESULT CTool_Map::Ready_DebugDraw()
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

HRESULT CTool_Map::Render_DebugDraw()
{
	if (nullptr == m_pSelectObj)
		return S_OK;

	if (OBJ_TYPE::OBJ_NPC != m_pSelectObj->Get_ObjectType() && OBJ_TYPE::OBJ_ANIMAL != m_pSelectObj->Get_ObjectType())
		return S_OK;

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();

	if (OBJ_TYPE::OBJ_NPC == m_pSelectObj->Get_ObjectType())
	{
		CGameNpc* pNpc = static_cast<CGameNpc*>(m_pSelectObj);
		vector<Vec4>* pPoints = pNpc->Get_RoamingArea();

		if (pPoints->size() != 0)
		{
			for (auto& iter : *pPoints)
			{
				m_pSphere->Center = Vec3(iter.x, iter.y, iter.z);
				DX::Draw(m_pBatch, *m_pSphere, Colors::LightCyan);
			}
		}
	}
	else if (OBJ_TYPE::OBJ_ANIMAL == m_pSelectObj->Get_ObjectType())
	{
		CAnimals* pAnimals = static_cast<CAnimals*>(m_pSelectObj);
		vector<Vec4>* pPoints = pAnimals->Get_RomingPoints();

		if (pPoints->size() != 0)
		{
			for (auto& iter : *pPoints)
			{
				m_pSphere->Center = Vec3(iter.x, iter.y, iter.z);
				DX::Draw(m_pBatch, *m_pSphere, Colors::LightCyan);
			}
		}
	}



	m_pBatch->End();

	return S_OK;
}
#endif // !_DEBUG



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
	Safe_Release(m_pRendererCom);

#ifdef _DEBUG
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pSphere);
	Safe_Release(m_pInputLayout);
#endif // _DEBUG

}
