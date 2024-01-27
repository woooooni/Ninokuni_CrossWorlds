#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_Battle_Attack_2.h"

CState_Destroyer_Battle_Attack_2::CState_Destroyer_Battle_Attack_2(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_Battle_Attack_2::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_Battle_Attack_2::Enter_State(void* pArg)
{
    m_pCharacter->Look_For_Target();
    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
    CSound_Manager::GetInstance()->Play_Sound(L"Destroyer_V_Atk_Medium_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0], MIN_TWEEN_DURATION);
}

void CState_Destroyer_Battle_Attack_2::Tick_State(_float fTimeDelta)
{
    if (m_pModelCom->Get_Progress() >= 0.2f && m_pModelCom->Get_Progress() <= 0.4f)
        m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), 5.f, fTimeDelta);


    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

    __super::Attack_Input(fTimeDelta);
}

void CState_Destroyer_Battle_Attack_2::Exit_State()
{
    m_pCharacter->Get_RendererCom()->Set_RadialBlur(false);
}


CState_Destroyer_Battle_Attack_2* CState_Destroyer_Battle_Attack_2::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_Battle_Attack_2* pInstance = new CState_Destroyer_Battle_Attack_2(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_Battle_Attack_2");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_Battle_Attack_2::Free()
{
    __super::Free();
}
