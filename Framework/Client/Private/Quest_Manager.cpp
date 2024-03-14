#include "stdafx.h"
#include "Quest_Manager.h"

#include "MainQuest.h"
#include "SubQuest.h"

#include "GameInstance.h"
#include "UI_Manager.h"

#include <FileUtils.h>
#include <Utils.h>
#include "Light.h"

IMPLEMENT_SINGLETON(CQuest_Manager)

CQuest_Manager::CQuest_Manager()
{

}

HRESULT CQuest_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pMainQuest = CMainQuest::Create();
	m_pSubQuest = CSubQuest::Create();

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef<ID3D11Device*>(m_pDevice);
	Safe_AddRef<ID3D11DeviceContext*>(m_pContext);

	m_bIsReserve = true;

	return S_OK;
}

void CQuest_Manager::Tick(_float fTimeDelta)
{
	if (m_bIsRunning)
	{
		m_pMainQuest->Tick(fTimeDelta);
		m_pSubQuest->Tick(fTimeDelta);
	}

	/* 모든 퀘스트 다이얼로그에서 카메라 이벤트 연동후 삭제 해주면 됩니다. */
	{
		if (GI->Get_CurrentLevel() != LEVELID::LEVEL_TOOL && KEY_TAP(KEY::HOME))
		{
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
		}
	}
}

void CQuest_Manager::LateTick(_float fTimeDelta)
{
	if (m_bIsRunning)
	{
		m_pMainQuest->LateTick(fTimeDelta);
		m_pSubQuest->LateTick(fTimeDelta);
	}
}

void CQuest_Manager::Set_SubQuestRunning(CSubQuest::SUBQUEST_NAME eSQName, _bool bIsRunning)
{
	m_pSubQuest->Set_SubQuestRunning(eSQName, bIsRunning);
}

void CQuest_Manager::Ready_InvasionLight(const wstring& strLightFilePath)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strLightFilePath + L"/" + L"Evermore InvasionLight" + L".light";

	shared_ptr<CFileUtils> pFile = make_shared<CFileUtils>();
	pFile->Open(strMapFilePath, FileMode::Read);

	_uint iLightSize = 0;
	pFile->Read<_uint>(iLightSize);
	// 라이트 개수
	list<CLight*>* pLightlist = GI->Get_LightList();
	for (auto& pLight : *pLightlist)
		Safe_Release<CLight*>(pLight);

	pLightlist->clear();

	for (_uint i = 0; i < iLightSize; ++i)
	{
		LIGHTDESC LightDesc;
		::ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

		// Type
		_uint iLightType = 0;
		_uint iLightID = 0;

		pFile->Read<_uint>(iLightType);

		if (LIGHTDESC::TYPE_DIRECTIONAL == iLightType)
		{
			// ID
			pFile->Read<_uint>(iLightID);

			// State
			Vec3 vDiffuse, vAmbientLowerColor, vAmbientUpperColor, vDirection;
			pFile->Read<Vec3>(vDiffuse);
			pFile->Read<Vec3>(vAmbientLowerColor);
			pFile->Read<Vec3>(vAmbientUpperColor);
			pFile->Read<Vec3>(vDirection);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempColor = vDiffuse;
			LightDesc.vAmbientLowerColor = vAmbientLowerColor;
			LightDesc.vAmbientUpperColor = vAmbientUpperColor;
			LightDesc.vTempDirection = vDirection;
		}
		else if (LIGHTDESC::TYPE_POINT == iLightType)
		{
			pFile->Read<_uint>(iLightID);

			// State
			Vec3 vPos, vColor;
			_float fRange;
			_bool bNonCull;
			pFile->Read<Vec3>(vPos);
			pFile->Read<_float>(fRange);
			pFile->Read<Vec3>(vColor);
			pFile->Read	<_bool>(bNonCull);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempPosition = vPos;
			LightDesc.fTempRange = fRange;
			LightDesc.vTempColor = vColor;
			LightDesc.bNonCull = bNonCull;
		}
		else if (LIGHTDESC::TYPE_SPOT == iLightType)
		{
			pFile->Read<_uint>(iLightID);

			Vec3 vPos, vColor, vDirection;
			_float fTempRange, fOuterAngle, fInnerAngle;
			_bool bNonCull;
			pFile->Read<Vec3>(vPos);
			pFile->Read<Vec3>(vDirection);
			pFile->Read<Vec3>(vColor);
			pFile->Read<_float>(fTempRange);
			pFile->Read<_float>(fOuterAngle);
			pFile->Read<_float>(fInnerAngle);
			pFile->Read<_bool>(bNonCull);

			LightDesc.eType = static_cast<LIGHTDESC::TYPE>(iLightType);
			LightDesc.vTempPosition = vPos;
			LightDesc.vTempDirection = vDirection;
			LightDesc.vTempColor = vColor;
			LightDesc.fTempRange = fTempRange;
			LightDesc.fOuterAngle = fOuterAngle;
			LightDesc.fInnerAngle = fInnerAngle;
			LightDesc.bNonCull = bNonCull;
		}

		if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
			return;
	}

}

void CQuest_Manager::Free()
{
	Safe_Release(m_pMainQuest);
	Safe_Release(m_pSubQuest);

	Safe_Release<ID3D11Device*>(m_pDevice);
	Safe_Release<ID3D11DeviceContext*>(m_pContext);
}
