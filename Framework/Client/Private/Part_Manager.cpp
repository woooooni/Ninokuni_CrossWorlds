#include "stdafx.h"
#include "GameInstance.h"
#include "Part_Manager.h"
#include "Model.h"
#include "Animation.h"
#include "Utils.h"

IMPLEMENT_SINGLETON(CPart_Manager)

CPart_Manager::CPart_Manager()
{

}

HRESULT CPart_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (FAILED(Ready_PartModels()))
		return E_FAIL;

	return S_OK;
}

CModel* CPart_Manager::Get_PartModel(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType, const _uint iIndex)
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


vector<class CModel*>* CPart_Manager::Get_PartModels(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType)
{
	if (CHARACTER_TYPE::CHARACTER_END <= eCharacterType || PART_TYPE::PART_END < ePartType || eCharacterType < 0 || ePartType < 0)
		return nullptr;

	auto iter = m_PartModels[eCharacterType].find(ePartType);

	if (iter == m_PartModels[eCharacterType].end())
		return nullptr;

	return &(iter->second);
}
HRESULT CPart_Manager::Ready_PartModels()
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

void CPart_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
	{
		for (auto& iter : m_PartModels[i])
		{
			for (auto& pModel : iter.second)
				Safe_Release(pModel);

			iter.second.clear();
		}
		m_PartModels[i].clear();
	}
}



