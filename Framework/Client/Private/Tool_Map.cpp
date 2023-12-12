#include "stdafx.h"
#include "Tool_Map.h"
#include "imgui.h"
#include "FileUtils.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Utils.h"
#include "Picking_Manager.h"
#include "Mesh.h"

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
	if (ImGui::Begin("Inspector"))
	{
		if (ImGui::CollapsingHeader("[ MapTool ]"))
		{
			if (ImGui::BeginChild("Child_List", ImVec2(0, 300.f), true))
			{
				if (ImGui::RadioButton("Buildings", (0 == m_iControlState)))
				{
					m_pSelectObj = nullptr;
					m_bAddObject = false;
					m_iControlState = 0;
				} ImGui::SameLine();
				if (ImGui::RadioButton("Props", (1 == m_iControlState)))
				{
					m_pSelectObj = nullptr;
					m_bAddObject = false;
					m_iControlState = 1;
				} ImGui::SameLine();
				if (ImGui::RadioButton("Grounds", (2 == m_iControlState)))
				{
					m_pSelectObj = nullptr;
					m_bAddObject = false;
					m_iControlState = 2;
				} ImGui::SameLine();
				/*if(ImGui::RadioButton(""))*/

		
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
						CTransform* pTransform = m_pSelectObj->Get_Component<CTransform>(L"Com_Transform");
						if (nullptr == pTransform)
						{
							ImGui::End();
							return;
						}

						Vec3 vScaled = pTransform->Get_Scale();
						XMVECTOR vRotation = pTransform->Get_WorldRotation();
						XMMATRIX vPos = pTransform->Get_WorldMatrix();
						XMVECTOR vWorldPosition = vPos.r[3];
						

						ImGui::PushItemWidth(150.f);
						ImGui::DragFloat3("Position", &vWorldPosition.m128_f32[0], 0.1f, 0.1f, 20.f);
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
								break;
						}
						
					}
					ImGui::PopItemWidth();
				}
				else
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), u8"[ 현재 선택된 오브젝트가 없습니다.]");

				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				if (ImGui::Button(u8"오브젝트 추가"))
				{
					m_pSelectObj = nullptr;
					m_bAddObject = true;

				}ImGui::SameLine();

				if (ImGui::Button(u8"배치된 오브젝트"))
					m_bAddObject = false;

				if (true == m_bAddObject)
				{
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"추가할 오브젝트 선택");

					if (ImGui::ListBoxHeader("##ASSETLIST", ImVec2(300.0f, 0.0f)))
					{
						if(0 == m_iControlState)
							AddBuildings();
						else if (1 == m_iControlState) {}
						else if (2 == m_iControlState) {}
					}

					ImGui::ListBoxFooter();
				}
				else
				{
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"맵에 배치된 오브젝트 리스트");
					if (ImGui::ListBoxHeader("##OBJECTLIST", ImVec2(300.0f, 0.0f)))
					{
						if(0 == m_iControlState)
							BatchObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_BUILDING);
						else if (1 == m_iControlState) {}
						else if (2 == m_iControlState) {}
					}

					ImGui::ListBoxFooter();
				}
			}
			ImGui::EndChild();
		}

		Picking();
	}
	ImGui::End();
}

void CTool_Map::AddBuildings()
{
	const map<const std::wstring, CGameObject*>& vecPrototypeList = GI->Find_Prototype_GameObjects(LAYER_TYPE::LAYER_BUILDING);

	for (auto& Pair : vecPrototypeList)
	{
		if (ImGui::Selectable(CUtils::ToString(Pair.first).c_str()))
		{
			if (true == m_bAddObject)
				GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_BUILDING, Pair.first);
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

void CTool_Map::BatchObject(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
	list<CGameObject*>& pGameObjects = GI->Find_GameObjects(LEVEL_TOOL, LAYER_TYPE::LAYER_BUILDING);

	for (auto& pObj : pGameObjects)
	{
		string ObjectID = std::to_string(pObj->Get_ObjectID());
		string ObjectTag = CUtils::ToString(pObj->Get_ObjectTag());

		string strName = ObjectTag + "_" + ObjectID;

		if (ImGui::Selectable(strName.c_str()))
			m_pSelectObj = pObj;
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
				|| i == LAYER_TYPE::LAYER_GROUND
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
					_float4 vPosition;
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
		for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
		{
			if (i == LAYER_TYPE::LAYER_CAMERA
				|| i == LAYER_TYPE::LAYER_TERRAIN
				|| i == LAYER_TYPE::LAYER_BACKGROUND
				|| i == LAYER_TYPE::LAYER_SKYBOX
				|| i == LAYER_TYPE::LAYER_UI
				|| i == LAYER_TYPE::LAYER_GROUND
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
					_float4 vPosition;
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

//if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::LBTN))
//{
//	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
//	{
//		if (i == LAYER_TYPE::LAYER_CAMERA
//			|| i == LAYER_TYPE::LAYER_TERRAIN
//			|| i == LAYER_TYPE::LAYER_BACKGROUND
//			|| i == LAYER_TYPE::LAYER_SKYBOX
//			|| i == LAYER_TYPE::LAYER_UI
//			|| i == LAYER_TYPE::LAYER_GROUND
//			|| i == LAYER_TYPE::LAYER_EFFECT
//			|| i == LAYER_TYPE::LAYER_GRASS)
//			continue;
//
//		list<CGameObject*>& GameObjects = GI->Find_GameObjects(LEVEL_TOOL, i);
//		for (auto& Object : GameObjects)
//		{
//			CTransform* pTransform = Object->Get_Component<CTransform>(L"Com_Transform");
//			CModel* pModel = Object->Get_Component<CModel>(L"Com_Model");
//			if (pTransform == nullptr)
//				continue;
//
//			if (pModel == nullptr)
//				continue;
//
//			for (auto& pMesh : pModel->Get_Meshes())
//			{
//				_float4 vPosition;
//				if (CPicking_Manager::GetInstance()->Is_Picking(pTransform, pMesh, false, &vPosition))
//				{
//					m_pTarget = Object;
//					break;
//				}
//			}
//		}
//
//	}
//}


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
