#include "stdafx.h"
#include "Tool_Map.h"
#include "imgui.h"
#include "FileUtils.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Utils.h"
#include "Picking_Manager.h"
#include "Mesh.h"
#include <filesystem>

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
						if (0 == m_iControlState)
							AddMapObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_BUILDING);
						else if (1 == m_iControlState)
							AddMapObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_PROP);
						else if (2 == m_iControlState)
							AddMapObject(LEVELID::LEVEL_TOOL, LAYER_TYPE::LAYER_GROUND);
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
					}

					ImGui::ListBoxFooter();
				}

				ImGui::Spacing();

				if (ImGui::Button(u8"Save"))
					Save_Map_Data(TEXT("Evermore"));

				ImGui::SameLine();

				if (ImGui::Button(u8"Load"))
					Load_Map_Data(TEXT("Evermore"));

			}
			ImGui::EndChild();
		}

		Picking();
	}
	ImGui::End();
}

void CTool_Map::AddMapObject(LEVELID iLevelID, LAYER_TYPE iLayerType)
{
	const map<const std::wstring, CGameObject*>& vecPrototypeList = GI->Find_Prototype_GameObjects(iLayerType);

	for (auto& Pair : vecPrototypeList)
	{
		if (ImGui::Selectable(CUtils::ToString(Pair.first).c_str()))
		{
			if (true == m_bAddObject)
				GI->Add_GameObject(iLevelID, iLayerType, Pair.first);
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

		if (nullptr == m_pSelectObj)
			return;

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
			|| i == LAYER_TYPE::LAYER_PROJECTILE
			|| i == LAYER_TYPE::LAYER_EFFECT
			|| i == LAYER_TYPE::LAYER_TRAIL
			|| i == LAYER_TYPE::LAYER_NPC)
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
			|| i == LAYER_TYPE::LAYER_PROJECTILE
			|| i == LAYER_TYPE::LAYER_EFFECT
			|| i == LAYER_TYPE::LAYER_TRAIL
			|| i == LAYER_TYPE::LAYER_NPC)
			continue;

		GI->Clear_Layer(LEVEL_TOOL, i);

		// 2. ObjectCount
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
	MSG_BOX("Map_Loaded.");
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
