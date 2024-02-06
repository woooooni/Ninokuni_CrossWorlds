#include "stdafx.h"
#include "GameInstance.h"

#include "State_VehicleFlying_Run.h"
#include "Vehicle.h"
#include "Vehicle_Flying.h"
#include "UIMinigame_Manager.h"
#include "Camera_Follow.h"

#include "Character_Biplane_Bullet.h"
#include "Vehicle_Flying_Biplane.h"

#include "Pool.h"

CState_VehicleFlying_Run::CState_VehicleFlying_Run(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_VehicleFlying_Run::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
	m_pVehicle_Flying_Biplane = dynamic_cast<CVehicle_Flying_Biplane*>(m_pStateMachineCom->Get_Owner());
	m_pFollowCamera = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));

    if (nullptr == m_pVehicle || nullptr == m_pVehicle_Flying_Biplane || nullptr == m_pFollowCamera)
        return E_FAIL;

    return S_OK;
}

void CState_VehicleFlying_Run::Enter_State(void* pArg)
{
	m_iCurrAnimIndex = m_AnimIndices[0];
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);

	if (nullptr != m_pFollowCamera)
	{
		m_pFollowCamera->Set_DampingBackLimitRad(XMConvertToRadians(180.f));
		m_pFollowCamera->Set_CanInput(true);
		m_pFollowCamera->Set_MinMaxLimitY(0.4f, 2.5f);
	}
	

	CVehicle_Flying_Biplane* pBiplane = dynamic_cast<CVehicle_Flying_Biplane*>(m_pVehicle);
	if (nullptr != pBiplane)
	{
		pBiplane->Start_Trail(CVehicle_Flying_Biplane::BIPLANE_TRAIL::TAIL);
	}
}

void CState_VehicleFlying_Run::Tick_State(_float fTimeDelta)
{
	_bool bMove = false;

	

	// Skills
	if (KEY_TAP(KEY::NUM_1))
	{
		CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_MUCKCLOUD);
		CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_MUCKCLOUD);
		return;
	}
	if (KEY_TAP(KEY::NUM_2))
	{
		m_pStateMachineCom->Change_State(CVehicle::VEHICLE_RUSH);
		CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_RUSH);
		CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_RUSH);
		return;
	}
	if (KEY_TAP(KEY::NUM_3))
	{
		CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_GUIDED_MISSILE);
		CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_GUIDED_MISSILE);
		return;
	}
	if (KEY_TAP(KEY::R))
	{
		CSkill_Manager::GetInstance()->Use_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::FLYING_BURST);
		CUIMinigame_Manager::GetInstance()->Use_GrandprixSkill(SKILL_TYPE::FLYING_BURST);
		return;
	}

	if (KEY_HOLD(KEY::Q))
	{
		_vector vRight = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		_float fDot = XMVectorGetX(XMVector3Dot(vRight, vLook));

		m_pTransformCom->Rotation_Acc(vRight, acos(fDot) * fTimeDelta * -1.f);
	}

	if (KEY_HOLD(KEY::E))
	{
		_vector vRight = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_RIGHT));
		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		_float fDot = XMVectorGetX(XMVector3Dot(vRight, vLook));

		m_pTransformCom->Rotation_Acc(vRight, acos(fDot) * fTimeDelta);
	}

	if (KEY_HOLD(KEY::W))
	{
		bMove = true;

		if (false == m_pFollowCamera->Is_LockOn())
		{
			_matrix CamWorld = GI->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW);
			Vec3 vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
			Vec3 vCamLook = XMVector3Normalize(CamWorld.r[CTransform::STATE_LOOK]);
			vLook = XMVectorLerp(vLook, vCamLook, fTimeDelta);
			
			m_pTransformCom->Rotation_Look(vLook);
		}
		else
		{
			if (nullptr != m_pVehicle_Flying_Biplane->Get_Target())
			{
				CTransform* pTargetTransform = m_pVehicle_Flying_Biplane->Get_Target()->Get_Component_Transform();
				if (nullptr != pTargetTransform)
				{
					Vec3 vDir = XMVector3Normalize(pTargetTransform->Get_Position() - m_pTransformCom->Get_Position());
					Vec3 vLook = XMVector3Normalize(m_pTransformCom->Get_Look());

					vLook = XMVectorLerp(vLook, vDir, fTimeDelta);
					m_pTransformCom->Rotation_Look(vLook);
				}
			}
			else
			{
				_matrix CamWorld = GI->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW);
				Vec3 vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
				Vec3 vCamLook = XMVector3Normalize(CamWorld.r[CTransform::STATE_LOOK]);
				vLook = XMVectorLerp(vLook, vCamLook, fTimeDelta);

				m_pTransformCom->Rotation_Look(vLook);
			}
		}
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pVehicle->Get_Speed(), fTimeDelta);
		
	}


	if (KEY_HOLD(KEY::A))
	{
		m_pTransformCom->Rotation_Acc(XMVector3Normalize(m_pTransformCom->Get_Up()), -3.f * XMConvertToRadians(90.f) * fTimeDelta);
		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pVehicle->Get_Speed(), fTimeDelta);

		bMove = true;
	}


	if (KEY_HOLD(KEY::D))
	{
		m_pTransformCom->Rotation_Acc(XMVector3Normalize(m_pTransformCom->Get_Up()), 3.f * XMConvertToRadians(90.f) * fTimeDelta);
		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pVehicle->Get_Speed(), fTimeDelta);

		bMove = true;
	}

	// // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

	if (false == m_bShoot)
	{
		m_fTimeAcc += fTimeDelta;

		if (0.1f < m_fTimeAcc)
		{
			m_bShoot = true;
			m_fTimeAcc = 0.f;
		}
	}

	if (KEY_HOLD(KEY::LBTN))
	{
		if (g_eLoadCharacter == LOAD_CHARACTER_TYPE::ALL_CH)
		{
			if (true == m_bShoot)
				Shoot();
		}
	}

	if (KEY_HOLD(KEY::RBTN))
	{
		if (false == CUIMinigame_Manager::GetInstance()->Is_AimActive())
			CUIMinigame_Manager::GetInstance()->Set_GrandprixAimActive(true);
	}

	if (KEY_NONE(KEY::RBTN))
	{
		if (true == CUIMinigame_Manager::GetInstance()->Is_AimActive())
			CUIMinigame_Manager::GetInstance()->Set_GrandprixAimActive(false);
	}

	if (!bMove)
	{
		if (KEY_NONE(KEY::W) && KEY_NONE(KEY::A) && KEY_NONE(KEY::S) && KEY_NONE(KEY::D))
		{
			m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_IDLE);
			return;
		}
	}
}

void CState_VehicleFlying_Run::Exit_State()
{

}

void CState_VehicleFlying_Run::Shoot()
{
	CCharacter_Biplane_Bullet::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
	ProjectileDesc.pOwner = dynamic_cast<CVehicle_Flying*>(m_pVehicle);

	// Left Side Bullet
	
	CGameObject* pLeftBullet = CPool<CCharacter_Biplane_Bullet>::Get_Obj();

	if (nullptr == pLeftBullet)	
		pLeftBullet = GI->Clone_GameObject(L"Prototype_GameObject_Character_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);
		
	if (nullptr == pLeftBullet)
		return;

	CTransform* pLeftTransform = pLeftBullet->Get_Component<CTransform>(L"Com_Transform");
	Vec3 vLeftScale = pLeftTransform->Get_Scale();

	pLeftTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	pLeftTransform->Set_Scale(vLeftScale);

	_matrix MatLeft = m_pModelCom->Get_SocketLocalMatrix(0) * m_pTransformCom->Get_WorldMatrix();
	Vec4 vLeftStartPos = MatLeft.r[CTransform::STATE_POSITION];

	//vLeftStartPos += XMVector3Normalize(pLeftTransform->Get_Look()) * 0.5f;
	vLeftStartPos += XMVector3Normalize(m_pVehicle->Get_Component<CTransform>(L"Com_Transform")->Get_Look()) * 0.5f;
	pLeftTransform->Set_State(CTransform::STATE_POSITION, vLeftStartPos);

	if (nullptr != m_pVehicle_Flying_Biplane->Get_Target())
	{
		CTransform* pTargetTransform = m_pVehicle_Flying_Biplane->Get_Target()->Get_Component_Transform();
		if (nullptr != pTargetTransform)
		{
			Vec3 vDir = XMVector3Normalize((pTargetTransform->Get_Position() + XMVectorSet(0.f, 0.5f, 0.f, 0.f)) - pLeftTransform->Get_Position());
			pLeftTransform->Rotation_Look(vDir);
		}
	}

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pLeftBullet)))
		MSG_BOX("Generate Bullet Failed.");


	// Right Side Bullet

	CGameObject* pRightBullet = GI->Clone_GameObject(L"Prototype_GameObject_Character_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);

	if (nullptr == pRightBullet)
		pRightBullet = GI->Clone_GameObject(L"Prototype_GameObject_Character_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);
	
	if (nullptr == pRightBullet)
		return;

	CTransform* pRightTransform = pRightBullet->Get_Component<CTransform>(L"Com_Transform");
	Vec3 vRightScale = pRightTransform->Get_Scale();
	pRightTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
	pRightTransform->Set_Scale(vRightScale);

	_matrix MatRight = m_pModelCom->Get_SocketLocalMatrix(1) * m_pTransformCom->Get_WorldMatrix();
	Vec4 vRightStartPos = MatRight.r[CTransform::STATE_POSITION];
	vRightStartPos += XMVector3Normalize(m_pVehicle->Get_Component<CTransform>(L"Com_Transform")->Get_Look()) * 0.5f;
	pRightTransform->Set_State(CTransform::STATE_POSITION, vRightStartPos);

	if (nullptr != m_pVehicle_Flying_Biplane->Get_Target())
	{
		CTransform* pTargetTransform = m_pVehicle_Flying_Biplane->Get_Target()->Get_Component_Transform();
		if (nullptr != pTargetTransform)
		{
			Vec3 vDir = XMVector3Normalize((pTargetTransform->Get_Position() + XMVectorSet(0.f, 0.5f, 0.f, 0.f)) - pRightTransform->Get_Position());
			pRightTransform->Rotation_Look(vDir);
		}
	}

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, pRightBullet)))
		MSG_BOX("Generate Bullet Failed.");

	m_bShoot = false;
}

CState_VehicleFlying_Run* CState_VehicleFlying_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_VehicleFlying_Run* pInstance = new CState_VehicleFlying_Run(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_VehicleFlying_Run");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_VehicleFlying_Run::Free()
{
    __super::Free();
}
