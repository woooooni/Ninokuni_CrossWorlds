#include "stdafx.h"
#include "GameInstance.h"
#include "Part_Manager.h"
#include "Model.h"

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

CModel* CPart_Manager::Get_PartModel(CHARACTER_TYPE eCharacterType, PART_TYPE ePartType, const _uint iIndex)
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


vector<class CModel*>* CPart_Manager::Get_PartModels(CHARACTER_TYPE eCharacterType, PART_TYPE ePartType)
{
	if (CHARACTER_TYPE::CHARACTER_END <= eCharacterType || PART_TYPE::PART_END < ePartType || eCharacterType < 0 || ePartType < 0)
		return nullptr;

	auto iter = m_PartModels[eCharacterType].find(ePartType);

	if (iter == m_PartModels[eCharacterType].end())
		return nullptr;

	return &(iter->second);
}

HRESULT CPart_Manager::Apply_PlayerAnimation(const CHARACTER_TYPE& eCharacterType)
{
	return E_NOTIMPL;
}

HRESULT CPart_Manager::Save_Parts(const CHARACTER_TYPE& eCharacterType)
{
	return E_NOTIMPL;
}



HRESULT CPart_Manager::Ready_PartModels()
{
	/* 로더에서 원형 생성하고 여기서 클론 */

	/* SwordMan*/
	{
		/* Body */
		{	
			vector<CModel*> pSwordMan_Bodys;

			/* 01 */
			{
				CModel* pModel = dynamic_cast<CModel*>(GI->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Body_01"));
				if (nullptr == pModel)
				{
					MSG_BOX("Failed CPart_Manager::Ready_PartModels()");
					return E_FAIL;
				}
				pSwordMan_Bodys.push_back(pModel);
			}



			m_PartModels[CHARACTER_TYPE::SWORD_MAN].emplace(PART_TYPE::BODY, pSwordMan_Bodys);
		}

		/* Face */
		{			
			vector<CModel*> pSwordMan_Faces;

			/* 01 */
			CModel* pModel = dynamic_cast<CModel*>(GI->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Face_01"));
			if (nullptr == pModel)
			{
				MSG_BOX("Failed CPart_Manager::Ready_PartModels()");
				return E_FAIL;
			}
			pSwordMan_Faces.push_back(pModel);





			m_PartModels[CHARACTER_TYPE::SWORD_MAN].emplace(PART_TYPE::FACE, pSwordMan_Faces);
		}

		/* Hair */
		{
			vector<CModel*> pSwordMan_Hairs;

			/* 01 */
			CModel* pModel = dynamic_cast<CModel*>(GI->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Hair_01"));
			if (nullptr == pModel)
			{
				MSG_BOX("Failed CPart_Manager::Ready_PartModels()");
				return E_FAIL;
			}
			pSwordMan_Hairs.push_back(pModel);


			m_PartModels[CHARACTER_TYPE::SWORD_MAN].emplace(PART_TYPE::HAIR, pSwordMan_Hairs);
		}
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



