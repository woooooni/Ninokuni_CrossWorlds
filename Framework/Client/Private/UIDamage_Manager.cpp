#include "stdafx.h"
#include "UIDamage_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"
#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Damage_Number.h"
#include "UI_Damage_General.h"
#include "UI_Damage_Critical.h"


IMPLEMENT_SINGLETON(CUIDamage_Manager)

CUIDamage_Manager::CUIDamage_Manager()
{

}

HRESULT CUIDamage_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	
	return S_OK;
}

void CUIDamage_Manager::Tick(_float fTimeDelta)
{

}

HRESULT CUIDamage_Manager::Ready_DamageNumberPrototypes()
{
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Blue"), // Water -> Weakness
		CUI_Damage_Number::Create(m_pDevice, m_pContext, CUI_Damage_Number::UI_DAMAGEFONT::BLUE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Red"), // Fire -> PlayerDamage
		CUI_Damage_Number::Create(m_pDevice, m_pContext, CUI_Damage_Number::UI_DAMAGEFONT::RED), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_WhiteGold"), // Light
		CUI_Damage_Number::Create(m_pDevice, m_pContext, CUI_Damage_Number::UI_DAMAGEFONT::WHITEGOLD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Green"), // Wood
		CUI_Damage_Number::Create(m_pDevice, m_pContext, CUI_Damage_Number::UI_DAMAGEFONT::GREEN), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Purple"), // Dark
		CUI_Damage_Number::Create(m_pDevice, m_pContext, CUI_Damage_Number::UI_DAMAGEFONT::PURPLE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Gold"),
		CUI_Damage_Number::Create(m_pDevice, m_pContext, CUI_Damage_Number::UI_DAMAGEFONT::GOLD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_GoldWithRed"), // Strong
		CUI_Damage_Number::Create(m_pDevice, m_pContext, CUI_Damage_Number::UI_DAMAGEFONT::GOLD_WITHRED), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_White"),
		CUI_Damage_Number::Create(m_pDevice, m_pContext, CUI_Damage_Number::UI_DAMAGEFONT::WHITE), LAYER_UI)))
		return E_FAIL;

	// Critical
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Critical"),
		CUI_Damage_Critical::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	// General -> 확인 필요
//	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_General"),
//		CUI_Damage_General::Create(m_pDevice, m_pContext, CUI_Damage_General::UI_DMG_FONTTYPE::DMG_GENERALATTACK), LAYER_UI)))
//		return E_FAIL;

	// Miss
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Miss"),
		CUI_Damage_General::Create(m_pDevice, m_pContext, CUI_Damage_General::UI_DMG_FONTTYPE::DMG_MISS), LAYER_UI)))
		return E_FAIL;

	// Dodge
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Dodge"),
		CUI_Damage_General::Create(m_pDevice, m_pContext, CUI_Damage_General::UI_DMG_FONTTYPE::DMG_DODGE), LAYER_UI)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIDamage_Manager::Create_PlayerDamageNumber(CTransform* pTransformCom, _int iDamage)
{
	CUI_Damage_Number::DAMAGE_DESC DamageDesc = {};
	ZeroMemory(&DamageDesc, sizeof(CUI_Damage_Number::DAMAGE_DESC));

	DamageDesc.pTargetTransform = pTransformCom;
	DamageDesc.iDamage = iDamage;
	_float2 vPlayerPosition = CUI_Manager::GetInstance()->Get_ProjectionPosition(pTransformCom);
	vPlayerPosition.y -= 200.f;
	DamageDesc.vTargetPosition = vPlayerPosition;
	DamageDesc.bIsPlayer = true;

	CGameObject* pNumber = nullptr;
	if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_DamageNumber_Red"), &DamageDesc, &pNumber)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIDamage_Manager::Create_MonsterDamageNumber(CTransform* pTransformCom, _bool bIsBoss, UI_DAMAGETYPE eType, _int iDamage)
{
	if (UI_DAMAGETYPE::DAMAGETYPE_END <= eType)
		return E_FAIL;

	if (999999 < iDamage)
		return E_FAIL;

	CGameObject* pNumber = nullptr;
	CUI_Damage_Number::DAMAGE_DESC DamageDesc = {};
	ZeroMemory(&DamageDesc, sizeof(CUI_Damage_Number::DAMAGE_DESC));

	DamageDesc.pTargetTransform = pTransformCom;
	DamageDesc.iDamage = iDamage;
	_float2 vRandomPosition = Designate_RandomPosition(CUI_Manager::GetInstance()->Get_ProjectionPosition(pTransformCom), bIsBoss);
	if (vRandomPosition.x == -9999.f)
		return E_FAIL;
	DamageDesc.vTargetPosition = vRandomPosition;
	DamageDesc.bIsBoss = bIsBoss;

	if (0 == iDamage)
	{
		if (FAILED(Create_Miss(pTransformCom, bIsBoss)))
			return E_FAIL;

		return S_OK;
	}

	switch (eType)
	{
	case UI_DAMAGETYPE::WEAKNESS:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_Blue"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;

	case UI_DAMAGETYPE::NONE:
		if (m_iMaxStandard < iDamage)
		{
			if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
				TEXT("Prototype_GameObject_UI_DamageNumber_WhiteGold"), &DamageDesc, &pNumber)))
				return E_FAIL;
			//+ Critical
			Create_Critical(UI_DAMAGETYPE::NONE, DamageDesc.vTargetPosition);
		}
		else
		{
			if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
				TEXT("Prototype_GameObject_UI_DamageNumber_White"), &DamageDesc, &pNumber)))
				return E_FAIL;
		}
		break;

	case UI_DAMAGETYPE::STRENGTH:
		if (m_iMaxStandard < iDamage)
		{
			if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
				TEXT("Prototype_GameObject_UI_DamageNumber_GoldWithRed"), &DamageDesc, &pNumber)))
				return E_FAIL;
			//+ Critical
			Create_Critical(UI_DAMAGETYPE::STRENGTH, DamageDesc.vTargetPosition);
		}
		else
		{
			if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
				TEXT("Prototype_GameObject_UI_DamageNumber_Gold"), &DamageDesc, &pNumber)))
				return E_FAIL;
		}
		break;
	}

	return S_OK;
}

HRESULT CUIDamage_Manager::Create_Critical(UI_DAMAGETYPE eType, _float2 vPosition)
{
	if (UI_DAMAGETYPE::DAMAGETYPE_END <= eType)
		return E_FAIL;

	CUI_Damage_Critical::UI_CRITICALFONT eCriticalColor = CUI_Damage_Critical::UI_CRITICALFONT::CRITICALFONT_END;

	switch (eType)
	{
	case UI_DAMAGETYPE::WEAKNESS:
		eCriticalColor = CUI_Damage_Critical::UI_CRITICALFONT::CRITICAL_BLUE;
		break;

	case UI_DAMAGETYPE::NONE:
		eCriticalColor = CUI_Damage_Critical::UI_CRITICALFONT::CRITICAL_YELLOW;
		break;

	case UI_DAMAGETYPE::STRENGTH:
		eCriticalColor = CUI_Damage_Critical::UI_CRITICALFONT::CRITICAL_REDBLUR;
		break;

	}

	CGameObject* pCritical = nullptr;
	CUI_Damage_Critical::CRITICAL_DESC CriticalDesc = {};
	ZeroMemory(&CriticalDesc, sizeof(CUI_Damage_Critical::CRITICAL_DESC));

	CriticalDesc.vPosition = vPosition;
	CriticalDesc.eType = eCriticalColor;

	if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_DamageNumber_Critical"), &CriticalDesc, &pCritical)))
		return E_FAIL;

	return S_OK;
}

_float2 CUIDamage_Manager::Designate_RandomPosition(_float2 vTargetPosition, _bool bIsBoss)
{
	if (0.f > vTargetPosition.x || 1600.f < vTargetPosition.x ||
		0.f > vTargetPosition.y || 900.f < vTargetPosition.y)
		return _float2(-9999.f, -9999.f);

	_float2 vResultPosition;
	_float2 fRandomOffset = _float2(0.f, 0.f);
	if (bIsBoss)
	{
		fRandomOffset = _float2(GI->RandomFloat(-200.f, 200.f), GI->RandomFloat(-400.f, 0.f));
	}
	else
	{
		fRandomOffset = _float2(GI->RandomFloat(-100.f, 100.f), GI->RandomFloat(-200.f, 0.f));
	}

	vTargetPosition.x += fRandomOffset.x;
	vTargetPosition.y += fRandomOffset.y;

	vResultPosition.x = vTargetPosition.x;
	vResultPosition.y = vTargetPosition.y;

	return vResultPosition;
}

HRESULT CUIDamage_Manager::Create_Miss(CTransform* pTransformCom, _bool bIsBoss)
{
	CGameObject* pMiss = nullptr;

	CUI_Damage_General::GENERAL_DESC MissDesc = {};
	ZeroMemory(&MissDesc, sizeof(CUI_Damage_General::GENERAL_DESC));

	_float2 vRandomPosition = Designate_RandomPosition(CUI_Manager::GetInstance()->Get_ProjectionPosition(pTransformCom), bIsBoss);
	if (vRandomPosition.x == -9999.f)
		return E_FAIL;
	MissDesc.vPosition = vRandomPosition;

	_int iType = GI->RandomInt(0, 1);
	if (iType)
	{
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_Miss"), &MissDesc, &pMiss)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_Dodge"), &MissDesc, &pMiss)))
			return E_FAIL;
	}

	return S_OK;
}

void CUIDamage_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

/*

_int CUIDamage_Manager::Calculate_DamageColor(ELEMENTAL_TYPE eMonsterType, ELEMENTAL_TYPE eWeaponType, _int iDamage)
{
	if (ELEMENTAL_TYPE::BASIC <= eMonsterType || ELEMENTAL_TYPE::BASIC <= eWeaponType)
		return -1;

	if (999999 < iDamage)
	{
		iDamage = 999999;
		// 혹은 return -1;
	}

	UI_DAMAGEFONTCOLOR eColor = UI_DAMAGEFONTCOLOR::FONT_DAMAGECOLOR_END;

	switch (eMonsterType)
	{
	case ELEMENTAL_TYPE::FIRE: // 불 몬스터
		if (ELEMENTAL_TYPE::WATER == eWeaponType) // 물 무기 -> Strong
		{
			if (m_iMaxStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD; // 기본 공격
		}
		else if (ELEMENTAL_TYPE::WOOD == eWeaponType) // 나무 무기 -> Weakness
		{
			eColor = UI_DAMAGEFONTCOLOR::FONT_BLUE;
		}
		else
		{
			if (m_iStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE; // 기본 공격
		}
		break;

	case ELEMENTAL_TYPE::WATER: // 물 몬스터
		if (ELEMENTAL_TYPE::WOOD == eWeaponType) // 나무 무기 -> Strong
		{
			if (m_iMaxStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD; // 기본 공격
		}
		else if (ELEMENTAL_TYPE::FIRE == eWeaponType) // 불 무기 -> Weakness
		{
			eColor = UI_DAMAGEFONTCOLOR::FONT_BLUE;
		}
		else
		{
			if (m_iStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE; // 기본 공격
		}
		break;

	case ELEMENTAL_TYPE::WOOD: // 나무 몬스터
		if (ELEMENTAL_TYPE::FIRE == eWeaponType) // 불 무기 -> Strong
		{
			if (m_iMaxStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD; // 기본 공격
		}
		else if (ELEMENTAL_TYPE::WATER == eWeaponType) // 물 무기 -> Weakness
		{
			eColor = UI_DAMAGEFONTCOLOR::FONT_BLUE;
		}
		else
		{
			if (m_iStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE; // 기본 공격
		}
		break;

	case ELEMENTAL_TYPE::LIGHT: // 빛 몬스터
		if (ELEMENTAL_TYPE::DARK == eWeaponType) // 어둠 무기 -> Strong
		{
			if (m_iMaxStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD; // 기본 공격
		}
		else
		{
			if (m_iStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE; // 기본 공격
		}
		break;

	case ELEMENTAL_TYPE::DARK: // 어둠 몬스터
		if (ELEMENTAL_TYPE::LIGHT == eWeaponType) // 빛 무기 -> Strong
		{
			if (m_iMaxStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD; // 기본 공격
		}
		else
		{
			if (m_iStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE; // 기본 공격
		}
		break;
	}

	return _int(eColor);
}

*/
