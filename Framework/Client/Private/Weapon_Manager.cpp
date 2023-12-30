#include "stdafx.h"
#include "GameInstance.h"
#include "Weapon_Manager.h"
#include "Weapon.h"
#include "Utils.h"


IMPLEMENT_SINGLETON(CWeapon_Manager)

CWeapon_Manager::CWeapon_Manager()
{

}

HRESULT CWeapon_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// [TW] Loader에서 맨 마지막에 호출되어야 합니다.
	if (true == m_bReserved)
		return S_OK;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (FAILED(Loading_Weapon_Models(L"../Bin/Export/NonAnimModel/Weapon/Sword/")))
		return E_FAIL;

	if (FAILED(Loading_Weapon_Models(L"../Bin/Export/NonAnimModel/Weapon/Hammer/")))
		return E_FAIL;


	m_bReserved = true;


	return S_OK;
}

CModel* CWeapon_Manager::Get_WeaponModel(CHARACTER_TYPE eType, const wstring& strWeaponModelName)
{
	if (eType >= CHARACTER_TYPE::CHARACTER_END)
		return nullptr;

	for (auto& pWeapon : m_pWeaponModels[eType])
	{
		if (pWeapon->Get_Name().find(strWeaponModelName) != wstring::npos)
			return pWeapon;
	}

	return nullptr;
}








HRESULT CWeapon_Manager::Loading_Weapon_Models(const wstring& strFolderPath)
{
	for (auto& p : std::filesystem::directory_iterator(strFolderPath))
	{
		if (p.is_directory())
			Loading_Weapon_Models(p.path().wstring());


		if (strFolderPath.find(L"Dummy") != wstring::npos)
			continue;

		wstring strFilePath = CUtils::PathToWString(p.path().wstring());
		_tchar strFileName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strExt[MAX_PATH];

		_wsplitpath_s(strFilePath.c_str(), nullptr, 0, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		if (true == !lstrcmp(strExt, L".mesh"))
		{
			wstring strPrototypeName = L"Prototype_Component_Model_";
			strPrototypeName += strFileName;
			CModel* pModel = nullptr;
			if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, strPrototypeName, CModel::TYPE_NONANIM, strFolderName, strFileName, &pModel)))
				return E_FAIL;

			if (nullptr == pModel)
				return E_FAIL;

			wstring strDetermine = strFolderName;
			if (strDetermine.find(L"Sword") != wstring::npos)
			{
				CModel* pCloned = dynamic_cast<CModel*>(pModel->Clone(nullptr));
				m_pWeaponModels[CHARACTER_TYPE::SWORD_MAN].push_back(pCloned);
			}
			else if (strDetermine.find(L"Hammer") != wstring::npos)
			{
				CModel* pCloned = dynamic_cast<CModel*>(pModel->Clone(nullptr));
				m_pWeaponModels[CHARACTER_TYPE::DESTROYER].push_back(pCloned);
			}
			else if (strDetermine.find(L"Rifle") != wstring::npos)
			{
				CModel* pCloned = dynamic_cast<CModel*>(pModel->Clone(nullptr));
				m_pWeaponModels[CHARACTER_TYPE::ENGINEER].push_back(pCloned);
			}
			else
			{
				return E_FAIL;
			}
		}
	}
	return S_OK;

	return S_OK;
}


void CWeapon_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	if (true == m_bReserved)
	{
		for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
		{
			for (auto& pModel : m_pWeaponModels[i])
			{
				Safe_Release(pModel);
			}
			m_pWeaponModels[i].clear();
		}
	}

}



