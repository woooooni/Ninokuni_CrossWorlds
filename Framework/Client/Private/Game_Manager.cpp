#include "stdafx.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "Utils.h"
#include "Player.h"
#include "Kuu.h"
#include "Skill_Manager.h"

IMPLEMENT_SINGLETON(CGame_Manager)

CGame_Manager::CGame_Manager()
{

}

HRESULT CGame_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (true == m_bReserved)
		return S_OK;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	m_pPlayer = CPlayer::Create();
	m_pKuu = dynamic_cast<CKuu*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Kuu"), LAYER_NPC));

	if (nullptr == m_pKuu)
		return E_FAIL;

	if (nullptr == m_pPlayer && GI->Get_CurrentLevel() != LEVELID::LEVEL_LOADING)
		return E_FAIL;

	m_bReserved = true;


	return S_OK;
}

void CGame_Manager::Tick(_float fTimeDelta)
{
	if (nullptr != m_pPlayer && GI->Get_CurrentLevel() != LEVELID::LEVEL_LOADING)
	{
		m_pPlayer->Tick(fTimeDelta);
	}

	CSkill_Manager::GetInstance()->Tick(fTimeDelta);

	if (nullptr != m_pKuu && GI->Get_CurrentLevel() != LEVELID::LEVEL_LOADING)
	{
		m_pKuu->Tick(fTimeDelta);
	}

	if(m_bRadialBlurIn || m_bRadialBlurOut)
		Tick_RadialBlur(fTimeDelta);
}

void CGame_Manager::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pPlayer && GI->Get_CurrentLevel() != LEVELID::LEVEL_LOADING)
	{
		m_pPlayer->LateTick(fTimeDelta);
	}

	CSkill_Manager::GetInstance()->Tick(fTimeDelta);

	if (nullptr != m_pKuu && GI->Get_CurrentLevel() != LEVELID::LEVEL_LOADING)
	{
		m_pKuu->LateTick(fTimeDelta);
	}
}

CRenderer* CGame_Manager::Get_Renderer()
{
	if (nullptr == m_pPlayer)
		return nullptr;

	CCharacter* pCharacter = m_pPlayer->Get_Character();
	if (nullptr == pCharacter)
		return nullptr;

	return pCharacter->Get_RendererCom();
}

void CGame_Manager::Set_RadialBlur(_bool bSet, _float fQuality, _float fRadialBlurPower)
{
	if (nullptr == m_pPlayer)
		return;

	CCharacter* pCharacter = m_pPlayer->Get_Character();
	if (nullptr == pCharacter)
		return;

	CRenderer* pRenderer = pCharacter->Get_RendererCom();
	if (nullptr == pRenderer)
		return;

	pRenderer->Set_RadialBlur(bSet, fQuality, fRadialBlurPower);
}

void CGame_Manager::Lerp_RadialBlur(_bool bRadialBlurInOut, _bool bWaitFinish, _float fStartPower, _float fEndPower, _float fSpeed, _float fQuality)
{
	if (bRadialBlurInOut)
	{
		m_bRadialBlurIn = true;
		m_bRadialBlurOut = false;
	}
	else
	{
		m_bRadialBlurOut = true;
		m_bRadialBlurIn = false;
	}

	m_bWaitFinish = bWaitFinish;
	m_fAccRadialBlurPower = fStartPower;
	m_fRadialBlurEnd      = fEndPower;
	m_fAccRadialBlurSpeed = fSpeed;
	m_fRadialBlurQuality  = fQuality;
}

void CGame_Manager::Set_KuuTarget_Player()
{
	m_pKuu->Set_KuuTarget_Player();
}

void CGame_Manager::Tick_RadialBlur(_float fTimeDelta)
{
	if (true == m_bRadialBlurIn) // 레디얼 블러 활성화중 / 0.f, -0.1f
	{
		if (m_fAccRadialBlurPower > m_fRadialBlurEnd)
		{
			m_fAccRadialBlurPower -= m_fAccRadialBlurSpeed * fTimeDelta;
			if (m_fAccRadialBlurPower < m_fRadialBlurEnd)
				m_fAccRadialBlurPower = m_fRadialBlurEnd;
			Set_RadialBlur(true, m_fRadialBlurQuality, m_fAccRadialBlurPower);
		}
		else if (false == m_bWaitFinish)
		{
			m_bRadialBlurIn = false;
			Set_RadialBlur(false);
		}
	}
	else if (true == m_bRadialBlurOut) // 레디얼 블러 비활성화중 / -0.1f, 0.f
	{
		if (m_fAccRadialBlurPower < m_fRadialBlurEnd)
		{
			m_fAccRadialBlurPower += m_fAccRadialBlurSpeed * fTimeDelta;
			if (m_fAccRadialBlurPower > m_fRadialBlurEnd)
				m_fAccRadialBlurPower = m_fRadialBlurEnd;
			Set_RadialBlur(true, m_fRadialBlurQuality, m_fAccRadialBlurPower);
		}
		else if (false == m_bWaitFinish) //- 0.1 까지 갔다가 끄면 어찌되남?
		{
			m_bRadialBlurOut = false;
			Set_RadialBlur(false);
		}
	}
}


void CGame_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);
	Safe_Release(m_pKuu);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}



