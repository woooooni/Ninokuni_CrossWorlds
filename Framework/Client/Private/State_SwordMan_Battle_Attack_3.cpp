#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Battle_Attack_3.h"
#include "Utils.h"

CState_SwordMan_Battle_Attack_3::CState_SwordMan_Battle_Attack_3(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Battle_Attack_3::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Battle_Attack_3::Enter_State(void* pArg)
{
    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 5));
    CSound_Manager::GetInstance()->Play_Sound(L"SwordsMan_V_Atk_Long_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Battle_Attack_3::Tick_State(_float fTimeDelta)
{
    if (m_pModelCom->Get_Progress() >= 0.2f && m_pModelCom->Get_Progress() <= 0.3f)
        m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), 4.f, fTimeDelta);

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_SwordMan_Battle_Attack_3::Exit_State()
{
    
}


CState_SwordMan_Battle_Attack_3* CState_SwordMan_Battle_Attack_3::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Battle_Attack_3* pInstance = new CState_SwordMan_Battle_Attack_3(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Battle_Attack_3");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Battle_Attack_3::Free()
{
    __super::Free();
}
