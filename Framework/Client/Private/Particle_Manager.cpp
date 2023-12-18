#include "stdafx.h"
#include "Particle_Manager.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "Transform.h"
#include "GameObject.h"
#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"
#include "Particle.h"
#include "Effects_MouseClick.h"

IMPLEMENT_SINGLETON(CParticle_Manager)

CParticle_Manager::CParticle_Manager()
{

}

HRESULT CParticle_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strParticlePath)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	
	if (FAILED(Ready_Proto_Particles(strParticlePath)))
		return E_FAIL;

	if (FAILED(Ready_Proto_Particles()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Manager::Tick(_float fTimeDelta)
{

}

HRESULT CParticle_Manager::Generate_Particle(const wstring& strParticleName, _vector vPosition)
{
	CGameObject* pGameObject = GI->Clone_GameObject(L"Prototype_" + strParticleName, LAYER_TYPE::LAYER_EFFECT, &vPosition);
	if (nullptr == pGameObject)
		return E_FAIL;

	_uint iLevelIndex = GI->Get_CurrentLevel();
	if (FAILED(GI->Add_GameObject(iLevelIndex, LAYER_TYPE::LAYER_EFFECT, pGameObject)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Manager::Ready_Proto_Particles(const wstring& strParticlePath)
{
	for (auto& p : std::filesystem::directory_iterator(strParticlePath))
	{
		if (p.is_directory())
			Ready_Proto_Particles(p.path());

		wstring strFullPath = CUtils::PathToWString(p.path().wstring());

		_tchar strFileName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strExt[MAX_PATH];

		_wsplitpath_s(strFullPath.c_str(), nullptr, 0, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		if (0 == lstrcmp(TEXT(".Particle"), strExt))
		{
			shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
			File->Open(strFullPath, FileMode::Read);

			size_t length = _tcslen(strFileName);
			wchar_t* wcharFileName = new wchar_t[length + 1];
			wcscpy_s(wcharFileName, length + 1, strFileName);
			wcharFileName[length] = L'\0';

			wstring strParticleName(wcharFileName);

			if (FAILED(GI->Add_Prototype(L"Prototype_" + strParticleName,
				CParticle::Create(m_pDevice, m_pContext, strParticleName, nullptr, strFullPath), LAYER_TYPE::LAYER_EFFECT)))
				return E_FAIL;

			Safe_Delete(wcharFileName);
		}
	}

	return S_OK;
}

HRESULT CParticle_Manager::Ready_Proto_Particles()
{
	// 마우스 클릭 파티클
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_Particles_MouseClick"),
		CEffects_MouseClick::Create(m_pDevice, m_pContext, TEXT("Particles_MouseClick")), LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	return S_OK;
}

void CParticle_Manager::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
