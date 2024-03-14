#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Biplane_1.h"
#include "Riding_Manager.h"
#include "VehicleFlying_Projectile.h"
#include "Vehicle_Flying_Biplane.h"
#include "Camera_Manager.h"
#include "Camera.h"

CSkill_Biplane_1::CSkill_Biplane_1()
{
	
}



HRESULT CSkill_Biplane_1::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 3.f;

	return S_OK;
}

void CSkill_Biplane_1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSkill_Biplane_1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Biplane_1::Use_Skill()
{
	if (true == __super::Use_Skill())
	{
		Generate_MuckCloud();
		GI->Play_Sound(TEXT("amb_prop_st12_vfx_step_thunder_distance_01.wav"), CHANNELID::SOUND_BOSS,
			GI->Get_ChannelVolume(CHANNELID::SOUND_BOSS));
		return true;
	}
	else
	{
		GI->Play_Sound(TEXT("npc_np0900_tr_combo_fail_01.wav"), CHANNELID::SOUND_BOSS, 0.3f);
		return false;
	}
		
}

void CSkill_Biplane_1::Generate_MuckCloud()
{
	list<CGameObject*>& Targets = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER);

	for (auto& pTarget : Targets)
	{
		if (OBJ_TYPE::OBJ_GRANDPRIX_ENEMY != pTarget->Get_ObjectType())
			continue;

		CTransform* pNpcTransform = pTarget->Get_Component<CTransform>(L"Com_Transform");

		if (nullptr == pNpcTransform)
		{
			MSG_BOX("Npc Transform is Null. : CCSkill_Biplane_1::Generate_MuckCloud");
			continue;
		}

		CVehicleFlying_Projectile::GRANDPRIX_PROJECTILE_DESC ProjectileDesc = {};
		ProjectileDesc.pOwner = CRiding_Manager::GetInstance()->Get_Character_Biplane();

		CGameObject* pThunderCloud = GI->Clone_GameObject(L"Prototype_GameObject_Biplane_ThunderCloud", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);

		if (nullptr == pThunderCloud)
		{
			MSG_BOX("Thunder Cloud is Null. : CSkill_Biplane_1::Generate_MuckCloud");
			return;
		}

		CTransform* pThunderCloudTransform = pThunderCloud->Get_Component<CTransform>(L"Com_Transform");
		if (nullptr == pThunderCloudTransform)
		{
			MSG_BOX("Thunder Cloud Transform is Null. : CCSkill_Biplane_1::Generate_MuckCloud");
			return;
		}

		pThunderCloudTransform->Set_State(CTransform::STATE_POSITION, pNpcTransform->Get_Position());
		
		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pThunderCloud)))
		{
			MSG_BOX("Add GameObject Failed. : CCSkill_Biplane_1::Generate_MuckCloud");
			return;
		}
	}
}


CSkill_Biplane_1* CSkill_Biplane_1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Biplane_1* pInstance = new CSkill_Biplane_1;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Biplane_1");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Biplane_1::Free()
{
	__super::Free();
}
