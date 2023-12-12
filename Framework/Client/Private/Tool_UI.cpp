#include "stdafx.h"
#include "Tool_UI.h"
#include "imgui.h"
#include "GameInstance.h"
#include "GameObject.h"


CTool_UI::CTool_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool(pDevice, pContext)
{
}

HRESULT CTool_UI::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_UI::Tick(_float fTimeDelta)
{
	ImVec2 Buttonsize = { 100, 50 };
	wstring TargetTag;

	ImGui::Begin("UI_Tool");

	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
	static ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns;

	ImGui::BeginTabBar("UI Tab", tab_bar_flags);

	if (ImGui::BeginTabItem("Texture"))
	{
		ImGui::Text("\t");
		ImGui::Text("Target : ");

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Transform"))
	{
		// Position, Rotation, Scale, UI속성
		// UI 속성 : UI의 고유 속성. ID
		// Frame, Map, Logo, Font, HPBar, ItemSlot 등
		// Rotation, Depth Z

		ImGui::Text("\t");
		ImGui::Text("Target : ");
		ImGui::SameLine();
		if (nullptr != m_pTarget)
		{
			ImGui::SameLine();

//			m_pTarget->Get_ObjectTag();
//
//			wstring                 strTemp = m_pTarget->Get_ObjectTag();
//
//			string strSelectedObj = GI->ToString(strTemp);
//
//			_char* szSelectedObj = { nullptr };
//			szSelectedObj = const_cast<_char*>(strSelectedObj.c_str());
//			ImGui::Text("%s", szSelectedObj);
		}

		ImGui::Text("\t");
		ImGui::Text("\t");

		ImGui::Separator();
		ImGui::Text("\t");

		//////////////
		// Position //
		//////////////

		_float2 vTempPosition = _float2(0.f, 0.f);
		//XMStoreFloat4(&vTemp, /*UI의 Position을 가지고 온다.(x, y만)*/);
		_float fPos[2] = { vTempPosition.x, vTempPosition.y };

		ImGui::Text("Position :");

		ImGui::DragFloat2("##UI_Position", fPos, 0.1f);
		vTempPosition.x = fPos[0];
		vTempPosition.y = fPos[1];
		// pTransform->Set_State(CTransform::STATE::STATE_POSITION, XMLoadFloat4(&vTargetPos));

		///////////
		// Scale //
		///////////

		_float2 vTempScale = _float2(0.f, 0.f); // Transform으로 Scale을 받아온다.
		_float fScale[2] = { vTempScale.x, vTempScale.y };

		ImGui::Text("Scale :");

		ImGui::DragFloat2("##UI_Scale", fScale, 0.1f);

		if (fScale[0] <= 0.0f)
			fScale[0] = 0.0001f;

		if (fScale[1] <= 0.0f)
			fScale[1] = 0.0001f;

		vTempPosition.x = fPos[0];
		vTempPosition.y = fPos[1];
		//pTransform->Set_Scale(DirectX::XMLoadFloat3(&vTargetScale));

		ImGui::Text("\t");
		ImGui::Separator();
		ImGui::Text("\t");

		//////////////
		// Rotation //
		//////////////

		ImGui::Text("Rotation Z : ");
		_float fAngle = 0.f;
		if (ImGui::SliderFloat("##AngleRange Slider", &fAngle, -1.00f, 1.00f))
		{
			// 이 값을 UI에 세팅해준다.
		}

		/////////////
		// Depth Z //
		/////////////

		ImGui::Text("Depth Z : ");
		_float fRange = 1.f;
		if (ImGui::SliderFloat("##DepthRange Slider", &fRange, 0.00f, 1.00f))
		{
			// 이 값을 UI에 세팅해준다.
		}

		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

	ImGui::Text("\t");
	ImGui::Separator();

	if (ImGui::Button(("S A V E : U I"), Buttonsize))
	{
		// Save 기능 구현
	}

	ImGui::SameLine();

	if (ImGui::Button(("L O A D : U I"), Buttonsize))
	{
		// Load 기능 구현
	}

	ImGui::End();
}



CTool_UI* CTool_UI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_UI* pInstance = new CTool_UI(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed To Create : CTool_UI");
		return nullptr;
	}
	return pInstance;
}

void CTool_UI::Free()
{
	__super::Free();

	Safe_Release(m_pTarget);
}
