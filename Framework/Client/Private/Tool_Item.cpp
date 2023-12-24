#include "stdafx.h"
#include "Tool_Item.h"
#include "imgui.h"
#include "Utils.h"

#include "GameInstance.h"

CTool_Item::CTool_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool(pDevice, pContext)
{
}

HRESULT CTool_Item::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_Item::Tick(_float fTimeDelta)
{
	ImGui::Begin("Item_Tool");

	Show_Button();
	Show_Desc();

	ImGui::End();
}

void CTool_Item::Show_Button()
{
	ImGui::Begin("Type");

	ImGui::Checkbox("Show_WeaponDesc", &m_bShowWeapon);
	ImGui::Checkbox("Show_ArmorDesc", &m_bShowArmor);

	ImGui::End();
}

void CTool_Item::Show_Desc()
{
	if (m_bShowWeapon)
		Show_Weapon();
	else if (m_bShowArmor)
		Show_Armor();
}

void CTool_Item::Show_Weapon()
{
	const char* enumStrings[CGameItem_Weapon::WEAPON_END] = { "WEAPON_SWORD", "WEAPON_GAUNTLET", "WEAPON_HAMMER", "WEAPON_RIFLE" };

	m_tWeaponDesc.eMainCategory = CGameItem_Weapon::CATEGORY_WEAPON;

	ImGui::Text("Name");
	ImGui::SameLine();
	ImGui::InputText("##Name", strName, MAX_PATH);

	ImGui::Text("Code");
	ImGui::SameLine();
	ImGui::InputInt("##Code", &m_tWeaponDesc.eCode);

	ImGui::Text("WeaponCategory");
	ImGui::Combo("##WeaponCategory", &m_tWeaponDesc.eWeaponCategory, enumStrings, CGameItem_Weapon::WEAPON_END);

	ImGui::Text("Atk");
	ImGui::SameLine();
	ImGui::InputFloat("##Atk", &m_tWeaponDesc.fAtk);

	ImGui::Text("Content");
	ImGui::InputText("##Content", strContent, MAX_PATH);

	ImGui::Text("FilePath");
	ImGui::InputText("##Path", szFilePath, MAX_PATH);

	if (ImGui::Button("Export"))
	{
		m_tWeaponDesc.strName = strName;
		m_tWeaponDesc.strContent = strContent;

		Json WeaponDesc;
		WeaponDesc["Name"] = m_tWeaponDesc.strName;
		WeaponDesc["Code"] = m_tWeaponDesc.eCode;
		WeaponDesc["MainCategory"] = m_tWeaponDesc.eMainCategory;
		WeaponDesc["WeaponCategory"] = m_tWeaponDesc.eWeaponCategory;
		WeaponDesc["Atk"] = m_tWeaponDesc.fAtk;
		WeaponDesc["Content"] = m_tWeaponDesc.strContent;

		if (FAILED(GI->Json_Save(CUtils::ToWString(szFilePath), WeaponDesc)))
		{
			MSG_BOX("Failed Export.");
		}
		else
		{
			MSG_BOX("Success Export.");
		}
	}
}

void CTool_Item::Show_Armor()
{
}

CTool_Item* CTool_Item::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Item* pInstance = new CTool_Item(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CTool_Item");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Item::Free()
{
	__super::Free();
}
