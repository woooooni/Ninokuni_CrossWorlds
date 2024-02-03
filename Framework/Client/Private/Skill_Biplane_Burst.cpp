#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Biplane_Burst.h"
#include "Riding_Manager.h"
#include "VehicleFlying_Projectile.h"
#include "Vehicle_Flying_Biplane.h"
#include "Camera_Manager.h"
#include "Camera.h"

CSkill_Biplane_Burst::CSkill_Biplane_Burst()
{
	
}



HRESULT CSkill_Biplane_Burst::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 3.f;

	return S_OK;
}

void CSkill_Biplane_Burst::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSkill_Biplane_Burst::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Biplane_Burst::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		Shoot_BlackHole();
		return true;
	}
	else
	{
		return false;
	}
}


void CSkill_Biplane_Burst::Shoot_BlackHole()
{
	CVehicleFlying_Projectile::GRANDPRIX_PROJECTILE_DESC ProjectileDesc = {};
	ProjectileDesc.pOwner = CRiding_Manager::GetInstance()->Get_Character_Biplane();

	CTransform* pBiplaneTransform = CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component_Transform();
	if (nullptr == pBiplaneTransform)
	{
		MSG_BOX("pBiplaneTransform is Null. : CSkill_Biplane_Burst::Shoot_BlackHole");
		return;
	}

	CGameObject* pBlackHole = GI->Clone_GameObject(L"Prototype_GameObject_Biplane_BlackHole", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);
	if (nullptr == pBlackHole)
	{
		MSG_BOX("pBlackHole is Null. : CSkill_Biplane_Burst::Shoot_BlackHole");
		return;
	}

	CTransform* pBlackHoleTransform = pBlackHole->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pBlackHoleTransform)
	{
		MSG_BOX("Thunder Cloud Transform is Null. : CCSkill_Biplane_Burst::Shoot_BlackHole");
		return;
	}

	Vec3 vBlackHoleScale = pBlackHoleTransform->Get_Scale();
	pBlackHoleTransform->Set_WorldMatrix(pBiplaneTransform->Get_WorldMatrix());
	pBlackHoleTransform->Set_Scale(vBlackHoleScale);

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pBlackHole)))
	{
		MSG_BOX("Add GameObject Failed. : CCSkill_Biplane_Burst::Shoot_BlackHole");
		return;
	}

	
}


CSkill_Biplane_Burst* CSkill_Biplane_Burst::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Biplane_Burst* pInstance = new CSkill_Biplane_Burst;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Biplane_Burst");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Biplane_Burst::Free()
{
	__super::Free();
}
