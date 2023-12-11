#include "stdafx.h"
#include "Tool_Map.h"
#include "imgui.h"
#include "FileUtils.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Utils.h"

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
				if (ImGui::RadioButton("Object", (0 == m_iControlState)))
				{
					m_iControlState = 0;
				} ImGui::SameLine();
				if (ImGui::RadioButton("Height", (1 == m_iControlState)))
				{
					m_iControlState = 1;
				} ImGui::SameLine();
				if (ImGui::RadioButton("Texture", (2 == m_iControlState)))
				{
					m_iControlState = 2;
				} ImGui::SameLine();

				if (0 == m_iControlState)
				{
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), u8"오브젝트");
					if (nullptr != m_pSelectObj)
					{
						_bool iChange = 0;
						ImGui::PushItemWidth(50);
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), u8"선택된 오브젝트 : "); ImGui::SameLine();
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), CUtils::ToString(m_pSelectObj->Get_ObjectTag()).c_str());
						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 0.5f), "Position"); ImGui::SameLine();

						if (ImGui::CollapsingHeader("Movement"))
						{
							XMVECTOR vWorldScale = ::XMVectorSet(1.f, 1.f, 1.f, 1.f);
							
							
							ImGui::DragFloat3("Scale", &vWorldScale.m128_f32[0], 0.1f, 0.1f, 20.f);
						}

						if (ImGui::Button(u8"선택된 오브젝트 삭제"))
						{
							// TODO
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

					if (true == m_bAddObject)
					{
						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"추가할 오브젝트 선택");
						if (ImGui::ListBoxHeader("##ASSETLIST", ImVec2(300.0f, 0.0f)))
						{

						}

						ImGui::ListBoxFooter();
					}
					else
					{
						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), u8"맵에 배치된 오브젝트 리스트");
						if (ImGui::ListBoxHeader("##OBJECTLIST", ImVec2(300.0f, 0.0f)))
						{


							ImGui::ListBoxFooter();
						}
					}
				}
				else if (1 == m_iControlState)
				{

				}
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();
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
