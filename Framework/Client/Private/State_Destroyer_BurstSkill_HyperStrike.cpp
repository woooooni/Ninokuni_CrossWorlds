#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_BurstSkill_HyperStrike.h"

#include "Effect_Manager.h"
#include "Character_Projectile.h"

CState_Destroyer_BurstSkill_HyperStrike::CState_Destroyer_BurstSkill_HyperStrike(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_BurstSkill_HyperStrike::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_BurstSkill_HyperStrike::Enter_State(void* pArg)
{
    m_pCharacter->Set_Infinite(999.f, true);
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pCharacter->Look_For_Target();

    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
    CSound_Manager::GetInstance()->Play_Sound(L"Destroyer_V_Atk_Cast_Long_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
    

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Destroyer_Skill_HyperStrike"), pTransformCom->Get_WorldMatrix(), m_pCharacter);

    Start_Action();
    m_bStopAction = false;

    GI->Set_Slow(TIMER_TYPE::GAME_PLAY, 2.f, 0.5f, true);
}

void CState_Destroyer_BurstSkill_HyperStrike::Tick_State(_float fTimeDelta)
{

    if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_CurrAnimationFrame() >= 26)
    {
        if (false == m_bStopAction)
        {
            GI->Set_TimeScale(TIMER_TYPE::GAME_PLAY, 1.f);
            Stop_Action();
            m_bStopAction = true;
        }
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Destroyer_BurstSkill_HyperStrike::Exit_State()
{
    m_pCharacter->Set_Infinite(0.f, false);
    m_bStopAction = false;
}

void CState_Destroyer_BurstSkill_HyperStrike::Start_Action()
{
    CCamera_Action* pActionCamera = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));

    if (nullptr != pActionCamera)
    {
        if (FAILED(pActionCamera->Start_Action_DestroyerBurst(m_pTransformCom)))
        {
            MSG_BOX("Start_Action_DestroyerBurst Failed. : CState_Destroyer_BurstSkill_HyperStrike::Start_Action");
            return;
        }
    }
}

void CState_Destroyer_BurstSkill_HyperStrike::Stop_Action()
{
    CCamera_Action* pActionCamera = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));

    if (nullptr != pActionCamera)
    {
        if (FAILED(pActionCamera->Stop_ActionDestroyer_Burst()))
        {
            MSG_BOX("Stop_ActionDestroyer_Burst Failed. : CState_Destroyer_BurstSkill_HyperStrike::Stop_Action");
            return;
        }
    }
}

CState_Destroyer_BurstSkill_HyperStrike* CState_Destroyer_BurstSkill_HyperStrike::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_BurstSkill_HyperStrike* pInstance = new CState_Destroyer_BurstSkill_HyperStrike(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_BurstSkill_HyperStrike");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_BurstSkill_HyperStrike::Free()
{
    __super::Free();
}
