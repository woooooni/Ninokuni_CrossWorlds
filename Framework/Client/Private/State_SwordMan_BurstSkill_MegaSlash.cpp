#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera.h"
#include "Camera_Manager.h"
#include "State_SwordMan_BurstSkill_MegaSlash.h"

#include "Effect_Manager.h"
#include "Camera_Action.h"

CState_SwordMan_BurstSkill_MegaSlash::CState_SwordMan_BurstSkill_MegaSlash(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_BurstSkill_MegaSlash::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_BurstSkill_MegaSlash::Enter_State(void* pArg)
{
    m_pCharacter->Set_Infinite(999.f, true);
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pCharacter->Look_For_Target();

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_SwordMan_Skill_MegaSlash"), pTransformCom->Get_WorldMatrix(), m_pCharacter);

    Start_Action();

    m_bSlow = false;
    m_bStopAction = false;
    m_fAccRadialBlur = 16.f;

    m_vScreenSpeed = { 0.f, 0.f };
}

void CState_SwordMan_BurstSkill_MegaSlash::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_CurrAnimationFrame() >= 46)
    {
        if (false == m_bSlow)
        {
            GI->Set_Slow(TIMER_TYPE::GAME_PLAY, 0.5f, 0.1f, true);
            // m_pCharacter->Get_RendererCom()->Set_ScreenEffect(CRenderer::SCREEN_EFFECT::SWORDMAN_SPLIT);
            m_pCharacter->Get_RendererCom()->Set_RadialBlur(true);
            m_bSlow = true;
        }
        m_fAccRadialBlur -= 30.f * fTimeDelta;
        m_pCharacter->Get_RendererCom()->Set_RadialBlur(true, m_fAccRadialBlur);
    }

    if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_CurrAnimationFrame() >= 40)
    {
        if (false == m_bStopAction)
        {
            Stop_Action();
            m_bStopAction = true;
        }
    }

    

    //if (m_pCharacter->Get_RendererCom()->Get_Current_ScreenEffect() == CRenderer::SCREEN_EFFECT::SWORDMAN_SPLIT)
    //{
    //    m_vScreenSpeed.x += 50.f * fTimeDelta;
    //    m_vScreenSpeed.y += 100.f * fTimeDelta;
    //    m_pCharacter->Get_RendererCom()->Add_ScreenEffectAcc(Vec2((-0.5f -m_vScreenSpeed.x) * fTimeDelta, (-0.7f - m_vScreenSpeed.y) * fTimeDelta));
    //}
        

    if (GI->Get_TimeScale(TIMER_TYPE::GAME_PLAY) >= 1.f)
    {
        m_pCharacter->Get_RendererCom()->Set_ScreenEffect(CRenderer::SCREEN_EFFECT::SCREENEFFECT_END);
        m_pCharacter->Get_RendererCom()->Set_RadialBlur(false);
    }
        
        

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

}

void CState_SwordMan_BurstSkill_MegaSlash::Exit_State()
{
    m_pCharacter->Set_Infinite(0.f, false);

    if(!CCamera_Manager::GetInstance()->Get_CurCamera()->Is_Lock_Fov())
        CCamera_Manager::GetInstance()->Get_CurCamera()->Set_Fov(Cam_Fov_Follow_Default);

    m_pCharacter->Get_RendererCom()->Set_RadialBlur(false);
    m_bSlow = false;
    m_bStopAction = false;
    m_fAccRadialBlur = 16.f;
}
void CState_SwordMan_BurstSkill_MegaSlash::Start_Action()
{
    CCamera_Action* pActionCamera = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));

    if (nullptr != pActionCamera)
    {
        if (FAILED(pActionCamera->Start_Action_SwordManBurst(m_pTransformCom)))
        {
            MSG_BOX("Start_Action_SwordManBurst Failed. : CState_SwordMan_BurstSkill_MegaSlash::Start_Action");
            return;
        }
    }


}
void CState_SwordMan_BurstSkill_MegaSlash::Stop_Action()
{
    CCamera_Action* pActionCamera = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));

    if (nullptr != pActionCamera)
    {
        if (FAILED(pActionCamera->Stop_ActionSwordMan_Burst()))
        {
            MSG_BOX("Start_Action_SwordManBurst Failed. : CState_SwordMan_BurstSkill_MegaSlash::Start_Action");
            return;
        }
    }
}
CState_SwordMan_BurstSkill_MegaSlash* CState_SwordMan_BurstSkill_MegaSlash::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_BurstSkill_MegaSlash* pInstance = new CState_SwordMan_BurstSkill_MegaSlash(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_BurstSkill_MegaSlash");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_BurstSkill_MegaSlash::Free()
{
    __super::Free();
}
