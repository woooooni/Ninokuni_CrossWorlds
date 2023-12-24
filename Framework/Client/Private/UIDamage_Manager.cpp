#include "stdafx.h"
#include "UIDamage_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"
#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"

#include "UI_Damage_Skill.h"
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
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Blue"), // Water
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::BLUE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Red"), // Fire
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::RED), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_WhiteGold"), // Light
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::WHITEGOLD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Green"), // Wood
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::GREEN), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Purple"), // Dark
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::PURPLE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Gold"),
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::GOLD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_GoldWithRed"),
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::BLUE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_White"),
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::WHITE), LAYER_UI)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIDamage_Manager::Create_SkillDamageNumber(CTransform* pTransformCom, _int iDamage, ELEMENTAL_TYPE eType)
{
	CGameObject* pNumber = nullptr;
	CUI_Damage_Skill::DAMAGE_DESC DamageDesc = {};
	ZeroMemory(&DamageDesc, sizeof(CUI_Damage_Skill::DAMAGE_DESC));

	DamageDesc.pTargetTransform = pTransformCom;
	DamageDesc.iDamage = iDamage;

	switch (eType)
	{
	case ELEMENTAL_TYPE::FIRE:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_Red"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;

	case ELEMENTAL_TYPE::WATER:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_Blue"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;

	case ELEMENTAL_TYPE::WOOD:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_Green"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;

	case ELEMENTAL_TYPE::LIGHT:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_WhiteGold"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;

	case ELEMENTAL_TYPE::DARK:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_Purple"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;
	}

	if (nullptr == pNumber)
		return E_FAIL;
	//Safe_AddRef(pNumber);

	//타겟의 Transform을 이용해 WorldMatrix를 가지고온다.
//	_float4x4 matTargetWorld = pTransformCom->Get_WorldFloat4x4();
//
//	_float4x4 matWorld;
//	matWorld = matTargetWorld;
//	_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
//	_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
//	matTargetWorld._42 += 2.5f;
//
//	_float4x4 matWindow;
//	XMStoreFloat4x4(&matWindow, XMLoadFloat4x4(&matWorld) * matView * matProj);
//
//	_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];
//
//	vWindowPos.x /= vWindowPos.z;
//	vWindowPos.y /= vWindowPos.z;
//	_float fX = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
//	_float fY = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);
//
//	CTransform* pNumTransform = dynamic_cast<CUI_Damage_Skill*>(pNumber)->Get_Component<CTransform>(L"Com_Transform");
//	if (nullptr == pTransformCom)
//		return E_FAIL;
//
//	pNumTransform->Set_State(CTransform::STATE_POSITION,
//		XMVectorSet(fX - g_iWinSizeX * 0.5f, -(fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
//
	//dynamic_cast<CUI_Damage_Skill*>(pNumber)->Set_Damage((_uint)iDamage);// DamageNumber를 넘긴다.

	return S_OK;
}

HRESULT CUIDamage_Manager::Create_CommonDamageNumber(CTransform* pTransformCom, _int iDamage)
{
	return S_OK;
}

HRESULT CUIDamage_Manager::Create_Critical(CTransform* pTransformCom)
{
	return S_OK;
}

void CUIDamage_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
