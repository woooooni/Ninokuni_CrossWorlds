#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Attack.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Enemy_Biplane_Bullet.h"

#include "Vehicle_Flying_EnemyBiplane.h"
#include "Pool.h"

CState_EnemyBiplane_Attack::CState_EnemyBiplane_Attack(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_Attack::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
    m_pEngineerPlane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle || nullptr == m_pEngineerPlane)
        return E_FAIL;

    m_pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

    return S_OK;
}

void CState_EnemyBiplane_Attack::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_EnemyBiplane_Attack::Tick_State(_float fTimeDelta)
{
    CTransform* pTargetTransform = m_pTarget->Get_CharacterTransformCom(); //플레이어의 트랜스폼
    if (nullptr == pTargetTransform)
        return;
	
	Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
	_float fDistance = vDir.Length();

	if (m_pEngineerPlane->Get_Attack_StartEnd_Distance().y < fDistance)
	{
		m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN);
		return;
	}

	
	Follow_Target(fTimeDelta);


    m_fAccShootTime += fTimeDelta;
    if (m_fAccShootTime >= m_fShootTime)
    {
        m_fAccShootTime = 0.f;
        Shoot();
    }
}

void CState_EnemyBiplane_Attack::Exit_State()
{
    
}

void CState_EnemyBiplane_Attack::Tick_NormalAttack0(_float fTimeDelta)
{

}

void CState_EnemyBiplane_Attack::Tick_NormalAttack1(_float fTimeDelta)
{
}

void CState_EnemyBiplane_Attack::Tick_NormalAttack2(_float fTimeDelta)
{
}

void CState_EnemyBiplane_Attack::Shoot()
{
	CEnemy_Biplane_Bullet::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
	ProjectileDesc.pOwner = m_pEngineerPlane;

	// Left Side Bullet

	CGameObject* pLeftBullet = CPool<CEnemy_Biplane_Bullet>::Get_Obj();

	if (nullptr == pLeftBullet)
		pLeftBullet = GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);

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

	if (nullptr != m_pEngineerPlane->Get_Target())
	{
		CTransform* pTargetTransform = m_pEngineerPlane->Get_Target()->Get_Component_Transform();
		if (nullptr != pTargetTransform)
		{
			Vec3 vDir = XMVector3Normalize(pTargetTransform->Get_Position() - pLeftTransform->Get_Position());
			pLeftTransform->Rotation_Look(vDir);
		}
	}

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pLeftBullet)))
		MSG_BOX("Generate Bullet Failed.");


	// Right Side Bullet
	CGameObject* pRightBullet = CPool<CEnemy_Biplane_Bullet>::Get_Obj();

	if (nullptr == pRightBullet)
		pRightBullet = GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Bullet", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc);

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

	if (nullptr != m_pEngineerPlane->Get_Target())
	{
		CTransform* pTargetTransform = m_pEngineerPlane->Get_Target()->Get_Component_Transform();
		if (nullptr != pTargetTransform)
		{
			Vec3 vDir = XMVector3Normalize(pTargetTransform->Get_Position() - pRightTransform->Get_Position());
			pRightTransform->Rotation_Look(vDir);
		}
	}

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pRightBullet)))
		MSG_BOX("Generate Bullet Failed.");
}

void CState_EnemyBiplane_Attack::Follow_Target(_float fTimeDelta)
{
	CTransform* pTargetTransform = m_pTarget->Get_CharacterTransformCom(); //플레이어의 트랜스폼
	if (nullptr == pTargetTransform)
		return;

	Vec3 vDir = pTargetTransform->Get_Position() - m_pTransformCom->Get_Position();
	_float fDistance = vDir.Length();

	Vec3 vDestLook = XMVector3Normalize(vDir);
	Vec3 vMyLook = XMVector3Normalize(m_pTransformCom->Get_Look());

	m_pTransformCom->Rotation_Look(XMVector3Normalize(Vec3::Lerp(vMyLook, vDestLook, fTimeDelta)));

	if (fDistance > 30.f)	
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMovingSpeed, fTimeDelta);
}


CState_EnemyBiplane_Attack* CState_EnemyBiplane_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Attack* pInstance = new CState_EnemyBiplane_Attack(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Attack::Free()
{
    __super::Free();
}
