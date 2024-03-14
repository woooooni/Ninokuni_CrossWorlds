#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Battle_Attack_1.h"
#include "Weapon.h"

CState_SwordMan_Battle_Attack_1::CState_SwordMan_Battle_Attack_1(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Battle_Attack_1::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Battle_Attack_1::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pCharacter->Look_For_Target();
    m_pModelCom->Set_Animation(m_AnimIndices[0], MIN_TWEEN_DURATION);

    m_MotionTrailDesc.fAlphaSpeed = 1.f;
    m_MotionTrailDesc.fBlurPower = 0.f;
    m_MotionTrailDesc.vRimColor = { 1.f, 1.f, 1.f, 0.2f };
    m_MotionTrailDesc.vBloomPower = { 1.f, 1.f, 1.f };
    m_MotionTrailDesc.fAccMotionTrail = 0.f;
    m_MotionTrailDesc.fMotionTrailTime = 0.1f;

    for (_uint i = 0; i < 2; ++i)
        m_bGenMotionTrail[i] = false;

    m_bGenTrail = false;
    m_iGenTrailCount = 0;
}

void CState_SwordMan_Battle_Attack_1::Tick_State(_float fTimeDelta)
{
    
    /*if (false == m_bGenTrail && false == m_pModelCom->Is_Tween())
    {
        if (m_iGenTrailCount < 4)
        {
            if (m_pModelCom->Get_CurrAnimationFrame() >= 6.f || m_pModelCom->Get_CurrAnimationFrame() >= 15.f)
            {
                m_iGenTrailCount++;
                m_bGenTrail = true;
                m_pCharacter->Get_Weapon()->Generate_Trail(L"", L"", L"Distortion_1.png", Vec4(1.f, 0.6f, 0.f, 1.f), 11);
            }
        }
        
    }

    else if (true == m_bGenTrail && false == m_pModelCom->Is_Tween())
    {
        if (m_iGenTrailCount < 4)
        {
            if (m_pModelCom->Get_CurrAnimationFrame() >= 12.f || m_pModelCom->Get_CurrAnimationFrame() >= 25.f)
            {
                m_iGenTrailCount++;
                m_bGenTrail = false;
                m_pCharacter->Get_Weapon()->Stop_Trail();
            }
        }
        
    }*/
        
       

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

    __super::Attack_Input(fTimeDelta);
}

void CState_SwordMan_Battle_Attack_1::Exit_State()
{
    for (_uint i = 0; i < 2; ++i)
        m_bGenMotionTrail[i] = false;

    m_iGenTrailCount = 0;
    m_pCharacter->Stop_MotionTrail();
    m_pCharacter->Get_Weapon()->Stop_Trail();
}


CState_SwordMan_Battle_Attack_1* CState_SwordMan_Battle_Attack_1::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Battle_Attack_1* pInstance = new CState_SwordMan_Battle_Attack_1(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Battle_Attack_1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Battle_Attack_1::Free()
{
    __super::Free();
}
