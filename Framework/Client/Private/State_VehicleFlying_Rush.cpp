#include "stdafx.h"
#include "GameInstance.h"

#include "State_VehicleFlying_Rush.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"
#include "Camera_Follow.h"

#include "Vehicle.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

#include "Vehicle_Flying_Biplane.h"

CState_VehicleFlying_Rush::CState_VehicleFlying_Rush(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_VehicleFlying_Rush::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;

    m_fAccSpeed = 10.f;

    return S_OK;
}

void CState_VehicleFlying_Rush::Enter_State(void* pArg)
{
    m_fAccSpeed = 30.f;
    m_fAccRadialBlurPower = 0.1f;
    CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Lerp_Fov(XMConvertToRadians(80.f), 0.5f);

    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    CVehicle_Flying_Biplane* pFlyingBiplane = dynamic_cast<CVehicle_Flying_Biplane*>(m_pVehicle);
    if (nullptr != pFlyingBiplane)
    {
        CUIMinigame_Manager::GetInstance()->OnOff_RushVignette(true);
        pFlyingBiplane->Generate_Trail(L"", L"T_e_Skl_In_Slash_Line003.png", L"", Vec4(1.f, 1.f, 1.f, 0.5f), 22);
        CGameObject* pRider = pFlyingBiplane->Get_Rider();
        if (nullptr != pRider)
        {
            pRider->Get_Component_StateMachine()->Change_State(CCharacter::STATE::FLYING_STAND);
        }
    }

    GI->Stop_Sound(CHANNELID::SOUND_UI2);
    GI->Play_Sound(TEXT("Grandprix_Rush.wav"), CHANNELID::SOUND_UI2, GI->Get_ChannelVolume(CHANNELID::SOUND_UI2));
}

void CState_VehicleFlying_Rush::Tick_State(_float fTimeDelta)
{
    m_fAccRadialBlurPower -= 0.05f * fTimeDelta;
    m_fAccSpeed -= 10.f * fTimeDelta;

    if (0.f >= m_fAccSpeed)
    {
        m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN);
        return;
    }
     
    m_fAccRadialBlurPower = max(0.f, m_fAccRadialBlurPower);
    CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_RadialBlur(true, 16.f, m_fAccRadialBlurPower);
    // m_pTransformCom->Rotation_Acc(XMVector3Normalize(m_pTransformCom->Get_Look()), -5.f * XMConvertToRadians(180.f) * fTimeDelta);
    m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pVehicle->Get_Speed() + m_fAccSpeed, fTimeDelta);
}

void CState_VehicleFlying_Rush::Exit_State()
{
    m_fAccSpeed = 10.f;
    m_fAccRadialBlurPower = 0.1f;
    CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Lerp_Fov(Cam_Fov_Follow_Default, 0.5f);
    CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_RadialBlur(false, 16.f);

    CVehicle_Flying_Biplane* pFlyingBiplane = dynamic_cast<CVehicle_Flying_Biplane*>(m_pVehicle);
    if (nullptr != pFlyingBiplane)
        pFlyingBiplane->Stop_Trail();

    CUIMinigame_Manager::GetInstance()->OnOff_RushVignette(false);
}

CState_VehicleFlying_Rush* CState_VehicleFlying_Rush::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_VehicleFlying_Rush* pInstance = new CState_VehicleFlying_Rush(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_VehicleFlying_Rush");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_VehicleFlying_Rush::Free()
{
    __super::Free();
}
