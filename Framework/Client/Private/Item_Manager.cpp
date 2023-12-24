#include "stdafx.h"
#include "Item_Manager.h"

#include "Utils.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CItem_Manager)

CItem_Manager::CItem_Manager()
{
}

//void CItem_Manager::Add_ItemDesc(CGameItem::ITEMCATEGORY_MAIN eCategory, void* pDesc)
//{
//	// 무기
//	if (eCategory == CGameItem::CATEGORY_WEAPON)
//	{
//		CGameItem_Weapon::ITEMDESC_WEAPON tItemDesc = *(CGameItem_Weapon::ITEMDESC_WEAPON*)pDesc;
//		m_mapItemDesc.emplace(tItemDesc.eCode, tItemDesc);
//	}
//	// 방어구 
//	if (eCategory == CGameItem::CATEGORY_ARMOR)
//	{
//	}
//	// 악세서리
//	if (eCategory == CGameItem::CATEGORY_ACC)
//	{
//	}
//	// 포션 
//	if (eCategory == CGameItem::CATEGORY_POTION)
//	{
//	}
//	// 기타 
//	if (eCategory == CGameItem::CATEGORY_ETC)
//	{
//	}
//
//}

HRESULT CItem_Manager::Reserve_Manager(const wstring& strPatriclePath)
{
	if (FAILED(Add_ItemDesc(strPatriclePath)))
		return E_FAIL;

	return S_OK;
}

void CItem_Manager::Tick(_float fTimeDelta)
{
}

HRESULT CItem_Manager::Add_ItemDesc(const wstring& strPatriclePath)
{
	for (auto& p : std::filesystem::directory_iterator(strPatriclePath))
	{
		if (p.is_directory())
		{
			Add_ItemDesc(p.path().wstring());
		}

		wstring strFilePath = CUtils::PathToWString(p.path().wstring());

		_tchar strFileName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strExt[MAX_PATH];

		_wsplitpath_s(strFilePath.c_str(), nullptr, 0, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		if (0 == lstrcmp(TEXT(".json"), strExt))
		{
			if (strFilePath.find(L"Weapon") != wstring::npos)
			{
				CGameItem_Weapon::ITEMDESC_WEAPON tItemDesc;
				Json Load = GI->Json_Load(strFilePath);

				tItemDesc.eCode = Load["Code"];
				tItemDesc.eMainCategory = Load["MainCategory"];
				tItemDesc.eWeaponCategory = Load["WeaponCategory"];
				tItemDesc.strName = CUtils::ToWString(Load["Name"]);
				tItemDesc.strContent = CUtils::ToWString(Load["Content"]);
				tItemDesc.fAtk = Load["Atk"];

				m_mapWeaponItemDesc.emplace(tItemDesc.eCode, tItemDesc);
			}
		}
	}

	return S_OK;
}

void CItem_Manager::Free()
{
}
