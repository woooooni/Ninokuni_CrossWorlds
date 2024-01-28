#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"

#include "State_VehicleFlying_Stand.h"
#include "Vehicle.h"

#include "UIMinigame_Manager.h"

CState_VehicleFlying_Stand::CState_VehicleFlying_Stand(CStateMachine* pMachine)
    : CState_Vehicle(pMachine)
{
}

HRESULT CState_VehicleFlying_Stand::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pVehicle)
        return E_FAIL;
    
    return S_OK;
}

void CState_VehicleFlying_Stand::Enter_State(void* pArg)
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(false);
    
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(false);
}

void CState_VehicleFlying_Stand::Tick_State(_float fTimeDelta)
{
    _bool bMove = false;

    if (true == CUIMinigame_Manager::GetInstance()->Is_BiplaneFlying())
    {
        if (KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
        {
            m_pStateMachineCom->Change_State(CVehicle::VEHICLE_STATE::VEHICLE_RUN);
            return;
        }
    }
    else
    {
        if (KEY_TAP(KEY::SPACE))
        {
            bMove = true;

            _matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
            _vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
            _vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

            vRight = XMVector3Normalize(vRight);
            vCamLook = XMVector3Normalize(vCamLook);

            _float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 10.f * fTimeDelta;

            m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
            m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_pVehicle->Get_Speed(), fTimeDelta);
        }
    }

    if (true == GI->Mouse_Down(DIMK_WHEEL))
    {
        CUI_Manager::GetInstance()->Hide_MouseCursor(true);
        CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
        if (nullptr != pFollowCam)
            pFollowCam->Set_CanInput(true);
    }

    if (true == GI->Mouse_Up(DIMK_WHEEL))
    {
        CUI_Manager::GetInstance()->Hide_MouseCursor(false);
        CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
        if (nullptr != pFollowCam)
            pFollowCam->Set_CanInput(false);
    }

    if (KEY_HOLD(KEY::Q))
    {
        bMove = true;

        _matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
        _vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
        _vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

        vRight = XMVector3Normalize(vRight);
        vCamLook = XMVector3Normalize(vCamLook);

        _float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * 10.f * fTimeDelta;

        m_pTransformCom->Rotation_Acc(vRight, fRadian);
        m_pTransformCom->Move(vCamLook, m_pVehicle->Get_Speed(), fTimeDelta);
    }
}

void CState_VehicleFlying_Stand::Exit_State()
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(true);
    
    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(true);
}



CState_VehicleFlying_Stand* CState_VehicleFlying_Stand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_VehicleFlying_Stand* pInstance = new CState_VehicleFlying_Stand(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_VehicleFlying_Stand");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_VehicleFlying_Stand::Free()
{
    __super::Free();
}
