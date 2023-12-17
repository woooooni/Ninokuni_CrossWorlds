#include "stdafx.h"
#include "GameInstance.h"
#include "Part_Manager.h"

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

CModel* CPart_Manager::Get_PartModel(CHARACTER_TYPE eCharacterType, PART_TYPE ePartType)
{
	return nullptr;
}



HRESULT CPart_Manager::Ready_PartModels()
{
	// TODO :: 모델 로드 및 이니셜라이즈.

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



