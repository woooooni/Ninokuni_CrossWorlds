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

void CGame_Manager::Set_KuuTarget_Player()
{
	m_pKuu->Set_KuuTarget_Player();
}


void CGame_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);
	Safe_Release(m_pKuu);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}



