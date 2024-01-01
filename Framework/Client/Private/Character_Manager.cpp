#include "stdafx.h"
#include "GameInstance.h"
#include "Character_Manager.h"
#include "Utils.h"

#include "Character_SwordMan.h"
#include "Character_Engineer.h"
#include "Character_Destroyer.h"
#include "Engineer_Bullet.h"

IMPLEMENT_SINGLETON(CCharacter_Manager)

CCharacter_Manager::CCharacter_Manager()
{

}

HRESULT CCharacter_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// [TW] Loader에서 맨 마지막에 호출되어야 합니다.
	if (true == m_bReserved)
		return S_OK;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
		m_pCharacters[i] = nullptr;

	if (FAILED(Ready_Character_Models()))
		return E_FAIL;

	if (FAILED(Ready_Characters()))
		return E_FAIL;

	m_bReserved = true;


	return S_OK;
}



CModel* CCharacter_Manager::Get_PartModel(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType, const wstring& strPartTag)
{
	if (CHARACTER_TYPE::CHARACTER_END <= eCharacterType || PART_TYPE::PART_END < ePartType || eCharacterType < 0 || ePartType < 0)
		return nullptr;

	auto iter = m_PartModels[eCharacterType].find(ePartType);

	if (iter == m_PartModels[eCharacterType].end())
		return nullptr;

	for (auto& pModel : iter->second)
	{
		if (wstring::npos != pModel->Get_Name().find(strPartTag))
			return pModel;
	}

	return nullptr;
}

CModel* CCharacter_Manager::Get_PartModel(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType, _uint iIndex)
{
	if (CHARACTER_TYPE::CHARACTER_END <= eCharacterType || PART_TYPE::PART_END < ePartType || eCharacterType < 0 || ePartType < 0)
		return nullptr;

	auto iter = m_PartModels[eCharacterType].find(ePartType);

	if (iter == m_PartModels[eCharacterType].end())
		return nullptr;

	if (iter->second.size() <= iIndex)
		return nullptr;

	return iter->second[iIndex];
}


vector<class CModel*>* CCharacter_Manager::Get_PartModels(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType)
{
	if (CHARACTER_TYPE::CHARACTER_END <= eCharacterType || PART_TYPE::PART_END < ePartType || eCharacterType < 0 || ePartType < 0)
		return nullptr;

	auto iter = m_PartModels[eCharacterType].find(ePartType);

	if (iter == m_PartModels[eCharacterType].end())
		return nullptr;

	return &(iter->second);
}

HRESULT CCharacter_Manager::Ready_Characters()
{
#pragma region SwordMan
	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_Character_SwordMan", CCharacter_SwordMan::Create(m_pDevice, m_pContext, L"SwordMan"), LAYER_CHARACTER, true)))
		return E_FAIL;

	CCharacter* pCharacterSwordMan = dynamic_cast<CCharacter*>(GI->Find_Prototype_GameObject(LAYER_CHARACTER, L"Prototype_GameObject_Character_SwordMan"));
	if (nullptr == pCharacterSwordMan)
		return E_FAIL;

	m_pCharacters[CHARACTER_TYPE::SWORD_MAN] = dynamic_cast<CCharacter*>(pCharacterSwordMan->Clone(nullptr));
	if (nullptr == m_pCharacters[CHARACTER_TYPE::SWORD_MAN])
		return E_FAIL;
#pragma endregion

#pragma region Engineer
	/* Engineer */
	/*if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_Character_Engineer", CCharacter_Engineer::Create(m_pDevice, m_pContext, L"Engineer"), LAYER_CHARACTER, true)))
		return E_FAIL;

	CCharacter* pCharacterEngineer = dynamic_cast<CCharacter*>(GI->Find_Prototype_GameObject(LAYER_CHARACTER, L"Prototype_GameObject_Character_Engineer"));
	if (nullptr == pCharacterEngineer)
		return E_FAIL;

	m_pCharacters[CHARACTER_TYPE::ENGINEER] = dynamic_cast<CCharacter*>(pCharacterEngineer->Clone(nullptr));
	if (nullptr == m_pCharacters[CHARACTER_TYPE::ENGINEER])
		return E_FAIL;*/
#pragma endregion

#pragma region Destroyer
	//if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_Character_Destroyer", CCharacter_Destroyer::Create(m_pDevice, m_pContext, L"Destroyer"), LAYER_CHARACTER, true)))
	//	return E_FAIL;

	//CCharacter* pCharacterDestroyer = dynamic_cast<CCharacter*>(GI->Find_Prototype_GameObject(LAYER_CHARACTER, L"Prototype_GameObject_Character_Destroyer"));
	//if (nullptr == pCharacterDestroyer)
	//	return E_FAIL;

	//m_pCharacters[CHARACTER_TYPE::DESTROYER] = dynamic_cast<CCharacter*>(pCharacterDestroyer->Clone(nullptr));
	//if (nullptr == m_pCharacters[CHARACTER_TYPE::DESTROYER])
	//	return E_FAIL;
#pragma endregion

	return S_OK;
}



// 파츠 모델들을 Clone하여 가지고 있는다.
HRESULT CCharacter_Manager::Ready_Character_Models()
{
	const map<const wstring, CComponent*> PrototypeModels = GI->Find_Prototype_Components(LEVEL_PARTS);

	for (auto& iter : PrototypeModels)
	{
		wstring strPrototypeTag = iter.second->Get_PrototypeTag();

		PART_TYPE ePartType;
		CHARACTER_TYPE eCharacterType;

		if (strPrototypeTag.find(L"Head") != wstring::npos)
		{
			ePartType = PART_TYPE::HEAD;
		}

		else if (strPrototypeTag.find(L"Hair") != wstring::npos)
		{
			ePartType = PART_TYPE::HAIR;
		}

		else if (strPrototypeTag.find(L"Face") != wstring::npos)
		{
			ePartType = PART_TYPE::FACE;
		}
		else if (strPrototypeTag.find(L"Body") != wstring::npos)
		{
			ePartType = PART_TYPE::BODY;
		}
		else
		{
			return E_FAIL;
		}



		if (strPrototypeTag.find(L"SwordMan") != wstring::npos)
		{
			eCharacterType = CHARACTER_TYPE::SWORD_MAN;
		}
		else if (strPrototypeTag.find(L"Destroyer") != wstring::npos)
		{
			eCharacterType = CHARACTER_TYPE::DESTROYER;
		}

		else if (strPrototypeTag.find(L"Engineer") != wstring::npos)
		{
			eCharacterType = CHARACTER_TYPE::ENGINEER;
		}
		else
		{
			return E_FAIL;
		}


		CModel* pPartModel = dynamic_cast<CModel*>(iter.second->Clone(nullptr));
		if (nullptr == pPartModel)
			return E_FAIL;

		auto iterPartModel = m_PartModels[eCharacterType].find(ePartType);
		if (m_PartModels[eCharacterType].end() == iterPartModel)
		{
			vector<CModel*> Temp;
			m_PartModels[eCharacterType].emplace(ePartType, Temp);
			iterPartModel = m_PartModels[eCharacterType].find(ePartType);
			iterPartModel->second.reserve(10);
		}

		iterPartModel->second.push_back(pPartModel);
	}

	return S_OK;
}

void CCharacter_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	if (true == m_bReserved)
	{
		for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
		{
			Safe_Release(m_pCharacters[i]);

			for (auto& iter : m_PartModels[i])
			{
				for (auto& pModel : iter.second)
					Safe_Release(pModel);

				iter.second.clear();
			}
			m_PartModels[i].clear();
		}
	}

}



