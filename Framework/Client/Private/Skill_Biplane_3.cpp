#include "stdafx.h"
#include "GameInstance.h"
#include "..\Public\Skill_Biplane_3.h"
#include "Biplane_GuidedMissile.h"
#include "Riding_Manager.h"
#include "Vehicle_Flying_Biplane.h"
#include "Character_Manager.h"
#include "Camera_Manager.h"
#include "Camera.h"

CSkill_Biplane_3::CSkill_Biplane_3()
{
	
}



HRESULT CSkill_Biplane_3::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	if (FAILED(__super::Initialize(pDevice, pContext, pOwner)))
		return E_FAIL;

	m_fCoolTime = 3.f;

	return S_OK;
}

void CSkill_Biplane_3::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSkill_Biplane_3::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

_bool CSkill_Biplane_3::Use_Skill()
{
	if (nullptr == CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Target())
		return false;

	if (true == __super::Use_Skill())
	{
		Shoot_GuidedMissile();
		return true;
	}
	else
	{
		GI->Play_Sound(TEXT("npc_np0900_tr_combo_fail_01.wav"), CHANNELID::SOUND_BOSS,
			0.3f);
		return false;
	}
}

void CSkill_Biplane_3::Shoot_GuidedMissile()
{
	for (_int i = -2; i <= 2; ++i)
	{
		CVehicleFlying_Projectile::GRANDPRIX_PROJECTILE_DESC ProjectileDesc = {};
		ProjectileDesc.pOwner = CRiding_Manager::GetInstance()->Get_Character_Biplane();

		CGameObject* pMissile = GI->Clone_GameObject(L"Prototype_GameObject_Biplane_GuidedMissile", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);

		if (nullptr == pMissile)
		{
			MSG_BOX("Missile is Null. : CSkill_Biplane_3::Shoot_GuidedMissile");
			return;
		}

		CTransform* pBiplaneTransform = CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component<CTransform>(L"Com_Transform");
		if (nullptr == pMissile)
		{
			MSG_BOX("Biplane Transform is Null. : CSkill_Biplane_3::Shoot_GuidedMissile");
			return;
		}

		CModel* pBiplaneModel = CRiding_Manager::GetInstance()->Get_Character_Biplane()->Get_Component<CModel>(L"Com_Model");
		if (nullptr == pMissile)
		{
			MSG_BOX("Biplane Model is Null. : CSkill_Biplane_3::Shoot_GuidedMissile");
			return;
		}

		CTransform* pMissileTransform = pMissile->Get_Component<CTransform>(L"Com_Transform");
		if (nullptr == pMissile)
		{
			MSG_BOX("Missile Transform is Null. : CSkill_Biplane_3::Shoot_GuidedMissile");
			return;
		}

		Vec3 vMissileScale = pMissileTransform->Get_Scale();
		_matrix MatWolrd = pBiplaneTransform->Get_WorldMatrix();

		MatWolrd.r[CTransform::STATE_RIGHT] = XMVector3Normalize(MatWolrd.r[CTransform::STATE_RIGHT]) * vMissileScale.x;
		MatWolrd.r[CTransform::STATE_UP] = XMVector3Normalize(MatWolrd.r[CTransform::STATE_UP]) * vMissileScale.y;
		MatWolrd.r[CTransform::STATE_LOOK] = XMVector3Normalize(MatWolrd.r[CTransform::STATE_LOOK]) * vMissileScale.z;
		MatWolrd.r[CTransform::STATE_POSITION] = XMVectorSetW((pBiplaneModel->Get_SocketLocalMatrix(5) * pBiplaneTransform->Get_WorldMatrix()).Translation(), 1.f);

		pMissileTransform->Set_WorldMatrix(MatWolrd);
		pMissileTransform->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f * i));
		
		GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pMissile);

		if (i == 0)
		{
			CVehicleFlying_Projectile* pCamTargetMissie = dynamic_cast<CVehicleFlying_Projectile*>(pMissile);
			if (nullptr != pCamTargetMissie)
			{
				pCamTargetMissie->Set_CameraTarget(true);
				CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Change_TargetObj(pMissile, 0.01f, LERP_MODE::SMOOTH_STEP);
				CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Change_LookAtObj(pMissile, 0.01f, LERP_MODE::SMOOTH_STEP);
			}
		}
	}

	GI->Play_Sound(TEXT("sp_mng_st12_last_atk_blow_partner_01_2.wav"), CHANNELID::SOUND_BOSS,
		GI->Get_ChannelVolume(CHANNELID::SOUND_BOSS));
}


CSkill_Biplane_3* CSkill_Biplane_3::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCharacter* pOwner)
{
	CSkill_Biplane_3* pInstance = new CSkill_Biplane_3;
	if (FAILED(pInstance->Initialize(pDevice, pContext, pOwner)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed : CSkill_Biplane_3");
		return nullptr;
	}

	return pInstance;
}

void CSkill_Biplane_3::Free()
{
	__super::Free();
}
