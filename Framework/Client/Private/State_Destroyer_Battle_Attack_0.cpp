#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Weapon.h"
#include "Trail.h"
#include "State_Destroyer_Battle_Attack_0.h"

CState_Destroyer_Battle_Attack_0::CState_Destroyer_Battle_Attack_0(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_Battle_Attack_0::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_Battle_Attack_0::Enter_State(void* pArg)
{
    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 5));
    CSound_Manager::GetInstance()->Play_Sound(L"Destroyer_V_Atk_Short_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_pCharacter->Look_For_Target();
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    m_bGenTrail = false;
}

void CState_Destroyer_Battle_Attack_0::Tick_State(_float fTimeDelta)
{
    if (m_pModelCom->Get_Progress() >= 0.3f && m_pModelCom->Get_Progress() <= 0.4f)    
        m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), 3.f, fTimeDelta);

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

    if (false == m_pModelCom->Is_Tween())
    {
        if (false == m_bGenTrail && m_pModelCom->Get_CurrAnimationFrame() >= 16.f)
        {
            m_bGenTrail = true;
            m_pCharacter->Get_Weapon()->Get_Trail()->SetUp_Position(Vec4(0.f, 0.f, 0.9f, 1.f), Vec4(0.f, 0.f, 0.3f, 1.f));
            m_pCharacter->Get_Weapon()->Generate_Trail(L"PS_FireWave_Ai013.png", L"", L"Distortion_1.png", Vec4(0.f, 0.f, 0.f, 0.f), 11);
        }
        if (true == m_bGenTrail && m_pModelCom->Get_CurrAnimationFrame() >= 22.f)
        {
            m_pCharacter->Get_Weapon()->Stop_Trail();
        }
    }

    __super::Attack_Input(fTimeDelta);
}

void CState_Destroyer_Battle_Attack_0::Exit_State()
{
    m_bGenTrail = false;
}


CState_Destroyer_Battle_Attack_0* CState_Destroyer_Battle_Attack_0::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_Battle_Attack_0* pInstance = new CState_Destroyer_Battle_Attack_0(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_Battle_Attack_0::Free()
{
    __super::Free();
}
