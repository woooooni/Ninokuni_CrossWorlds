#include "stdafx.h"
#include "GameInstance.h"

#include "State_EnemyBiplane_Skill_2.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Vehicle_Flying_EnemyBiplane.h"

#include "Pool.h"
#include "Enemy_Biplane_Feather.h"

CState_EnemyBiplane_Skill_2::CState_EnemyBiplane_Skill_2(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_EnemyBiplane_Skill_2::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
    m_pEngineerPlane = dynamic_cast<CVehicle_Flying_EnemyBiplane*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle || nullptr == m_pEngineerPlane)
        return E_FAIL;

    m_pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
    m_pFollowCamera = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));

    if (nullptr == m_pFollowCamera)
        return E_FAIL;

    return S_OK;
}

void CState_EnemyBiplane_Skill_2::Enter_State(void* pArg)
{
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CState_EnemyBiplane_Skill_2::Tick_State(_float fTimeDelta)
{
    if ((nullptr != m_pFollowCamera->Get_LookAtObj()) && (m_pFollowCamera->Get_LookAtObj()->Get_ObjectType() != OBJ_TYPE::OBJ_GRANDPRIX_CHARACTER_PROJECTILE))
    {
        m_pFollowCamera->Set_TargetObj(m_pTarget);
        m_pFollowCamera->Set_LookAtObj(m_pEngineerPlane);
    }

    // ±êÅÐ ¶³±¸±â.
    Shoot_Feathers();
    m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_ENGINEER_STAND);
}

void CState_EnemyBiplane_Skill_2::Exit_State()
{
    
}


void CState_EnemyBiplane_Skill_2::Shoot_Feathers()
{
    for (_int i = -10; i <= 10; ++i)
    {
        for (_int j = -10; j <= 10; ++j)
        {
            CEnemy_Biplane_Feather* pFeather = CPool<CEnemy_Biplane_Feather>::Get_Obj();

            CEnemy_Biplane_Feather::GRANDPRIX_PROJECTILE_DESC ProjectileDesc;
            ProjectileDesc.pOwner = m_pEngineerPlane;

            if (nullptr == pFeather)
                pFeather = dynamic_cast<CEnemy_Biplane_Feather*>(GI->Clone_GameObject(L"Prototype_GameObject_Enemy_Biplane_Feather", LAYER_TYPE::LAYER_CHARACTER, &ProjectileDesc));

            CTransform* pFeatherTransform = pFeather->Get_Component<CTransform>(L"Com_Transform");
            Vec3 vScale = pFeatherTransform->Get_Scale();

            pFeatherTransform->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
            pFeatherTransform->Set_Scale(vScale);

            Vec4 vOffsetPos = pFeatherTransform->Get_RelativeOffset(
                Vec4(i * 5.f, 10.f + GI->RandomInt(-2, 5), j * 10.f, 1.f));
            pFeatherTransform->Set_Position(Vec4(pFeatherTransform->Get_Position()) + vOffsetPos);

            if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER, pFeather)))
                MSG_BOX("Generate Feather Failed.");
        }
        
    }

    GI->Play_Sound(TEXT("UI_Fx_Quest_Reward_Item_1.mp3"), CHANNELID::SOUND_VOICE_MONSTER3,
        GI->Get_ChannelVolume(CHANNELID::SOUND_VOICE_MONSTER3));
}

CState_EnemyBiplane_Skill_2* CState_EnemyBiplane_Skill_2::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_EnemyBiplane_Skill_2* pInstance = new CState_EnemyBiplane_Skill_2(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_EnemyBiplane_Skill_2");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_EnemyBiplane_Skill_2::Free()
{
    __super::Free();
}
