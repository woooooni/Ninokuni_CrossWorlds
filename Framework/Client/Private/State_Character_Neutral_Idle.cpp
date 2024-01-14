#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "State_Character_Neutral_Idle.h"
#include "UI_Manager.h"

#include "Game_Manager.h"
#include "Kuu.h"

CState_Character_Neutral_Idle::CState_Character_Neutral_Idle(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Neutral_Idle::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_Character_Neutral_Idle::Enter_State(void* pArg)
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(false);
    m_pCharacter->Disappear_Weapon();
    m_fAccIdleMotion = 0.f;
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);

    if(CGame_Manager::GetInstance()->Get_Kuu() != nullptr)
        CGame_Manager::GetInstance()->Get_Kuu()->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CKuu::NPC_IDLE);

    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(false);
}

void CState_Character_Neutral_Idle::Tick_State(_float fTimeDelta)
{
    m_fAccIdleMotion += fTimeDelta;
    if (m_fAccIdleMotion >= m_fIdleMotionTime)
    {
        m_fAccIdleMotion = 0.f;
        m_iCurrAnimIndex = m_AnimIndices[GI->RandomInt(1, 2)];

        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    }

    if (m_iCurrAnimIndex != m_AnimIndices[0] && false == m_pModelCom->Is_Tween() && m_pModelCom->Is_Finish())
    {
        m_fAccIdleMotion = 0.f;
        m_iCurrAnimIndex = m_AnimIndices[0];
        m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    }

     __super::Neutral_Idle_Input(fTimeDelta);
     __super::Skill_Input(fTimeDelta);
}

void CState_Character_Neutral_Idle::Exit_State()
{
    CUI_Manager::GetInstance()->Hide_MouseCursor(true);
    m_fAccIdleMotion = 0.f;
    m_iCurrAnimIndex = 0;

    CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
    if (nullptr != pFollowCam)
        pFollowCam->Set_CanInput(true);
}



CState_Character_Neutral_Idle* CState_Character_Neutral_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Neutral_Idle* pInstance = new CState_Character_Neutral_Idle(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Neutral_Idle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Neutral_Idle::Free()
{
    __super::Free();
}
