#include "stdafx.h"
#include "Stellia_Crystal_Controller.h"

#include "GameInstance.h"

#include "Stellia.h"
#include "Stellia_Crystal.h"
#include "Stellia_Crystal_Destructible.h"

CStellia_Crystal_Controller::CStellia_Crystal_Controller()
{
	m_fRespawnTime = 3.f;
}

void CStellia_Crystal_Controller::Tick(const _float fTimeDelta)
{
	if (nullptr == m_pStellia)
		return;

	if(m_pStellia->Get_Bools(CStellia::BOSS_BOOLTYPE::BOSSBOOL_RAGE2))
	{
		if (m_pStellia->Get_CrystalBingoCount() >= 2)
		{
			m_bIsRespawn = true;
			Clear_Crystals();
			m_pStellia->Clear_CrystalBingoCount();
		}

		if (m_bIsRespawn)
		{
			if (m_pStellia->Get_CrystalSuccessCount() < 3 && m_pStellia->Get_CrystalFailCount() < 3)
			{
				m_fTime += fTimeDelta;
				if (m_fTime >= m_fRespawnTime)
				{
					m_fTime = m_fRespawnTime - m_fTime;
					m_bIsRespawn = false;
					Create_Crystals(6, GI->RandomFloat(10.f, 25.f), m_pStellia);
				}
			}
		}

		if (m_pStellia->Get_CrystalSuccessCount() == 3)
		{
			if (m_pOriginalCrystal != nullptr)
				Safe_Release(m_pOriginalCrystal);

			for (auto& pCrystal : m_pCrystals)
			{
				pCrystal->Set_Dead(true);
			}

			m_pCrystals.clear();
			m_pCrystals.shrink_to_fit();

			m_pStellia->Set_Bools(CStellia::BOSS_BOOLTYPE::BOSSBOOL_RAGE2, false);
			m_pStellia->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CStellia::STELLIA_COUNTERSTART);
		}
		else if (m_pStellia->Get_CrystalFailCount() == 3)
		{
			if (m_pOriginalCrystal != nullptr)
				Safe_Release(m_pOriginalCrystal);

			for (auto& pCrystal : m_pCrystals)
			{
				pCrystal->Set_Dead(true);
			}

			m_pCrystals.clear();
			m_pCrystals.shrink_to_fit();

			m_pStellia->Set_Bools(CStellia::BOSS_BOOLTYPE::BOSSBOOL_RAGE2, false);
			m_pStellia->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CStellia::STELLIA_RAGE2FINISH);
		}
	}
}

HRESULT CStellia_Crystal_Controller::Create_Crystals(const _int& iNum, const _float& fRadius, CStellia* pStellia)
{
	if (pStellia == nullptr)
		return E_FAIL;

	m_iOriginalType = GI->RandomInt(CRYSTAL_AURA, CRYSTAL_GOLD);
	m_pStellia = pStellia;
	m_vOriginPos = m_pStellia->Get_OriginPos();

	/* 沥翠 农府胶呕 积己 */
	CGameObject* pGameObject = nullptr;
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Stellia_Crystal"), &m_vOriginPos, &pGameObject);
	if (nullptr != pGameObject)
	{
		CStellia_Crystal* pCrystal = dynamic_cast<CStellia_Crystal*>(pGameObject);
		if (nullptr != pCrystal)
			pCrystal->Set_CrystalType(m_iOriginalType);
	}

	/* 鞘靛 农府胶呕 积己 */
	if (nullptr == m_pStellia)
		return E_FAIL;

	for (size_t i = 0; i < iNum; i++)
	{
		const float fAngle = 360.f / (_float)iNum * i;

		Vec4 vPos = {};
		{
			vPos.x = m_vOriginPos.x + fRadius * cos(XMConvertToRadians(fAngle));
			vPos.y = m_vOriginPos.y;
			vPos.z = m_vOriginPos.z + fRadius * sin(XMConvertToRadians(fAngle));
		}

		pGameObject = nullptr;
		GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Stellia_Crystal_Destructible"), &vPos, &pGameObject);

		if (nullptr != pGameObject)
		{
			CStellia_Crystal_Destructible* pCrystal = dynamic_cast<CStellia_Crystal_Destructible*>(pGameObject);
			if (nullptr != pCrystal)
			{
				/* Set Data */
				{
					CStellia_Crystal_Destructible::STELLIA_CRYSTAL_DESC tCrystalDesc;
					tCrystalDesc.pStellia = m_pStellia;
					tCrystalDesc.iBingoType = m_iOriginalType;
					if(i < 2)
						tCrystalDesc.iSelfType = CRYSTAL_AURA;
					else if(i >= 2 && i < 4)
						tCrystalDesc.iSelfType = CRYSTAL_SKY;
					else
						tCrystalDesc.iSelfType = CRYSTAL_GOLD;

					pCrystal->Set_CrystalData(&tCrystalDesc);
				}

				m_pCrystals.push_back(pCrystal);
			}
		}
	}

	return S_OK;
}

HRESULT CStellia_Crystal_Controller::Clear_Crystals()
{
	for (auto& pCrystal : m_pCrystals)
	{
		pCrystal->Set_Dead(true);
	}

	m_pCrystals.clear();
	m_pCrystals.shrink_to_fit();

	return S_OK;
}
