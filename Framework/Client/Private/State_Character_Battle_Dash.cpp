#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Animation.h"
#include "State_Character_Battle_Dash.h"
#include "Motion_Trail.h"

CState_Character_Battle_Dash::CState_Character_Battle_Dash(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Character_Battle_Dash::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    
    return S_OK;
}

void CState_Character_Battle_Dash::Enter_State(void* pArg)
{
    // ���콺 �������� ������
    CHARACTER_TYPE eCharacterType = m_pCharacter->Get_CharacterType();
    wstring strSoundKey = L"";
    switch (eCharacterType)
    {
    case CHARACTER_TYPE::SWORD_MAN:
        strSoundKey = L"Swordsman_V_Atk_Cast_" + to_wstring(GI->RandomInt(1, 3));
        strSoundKey += L".mp3";
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    case CHARACTER_TYPE::ENGINEER:
        strSoundKey = L"Engineer_V_Atk_Cast_" + to_wstring(GI->RandomInt(1, 3));
        strSoundKey += L".mp3";
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    case CHARACTER_TYPE::DESTROYER:
        strSoundKey = L"Destroyer_V_Atk_Cast_" + to_wstring(GI->RandomInt(1, 3));
        strSoundKey += L".mp3";
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    }

    m_bFirstRootConvert = true;
    m_pCharacter->Appear_Weapon();
    m_pCharacter->Set_SuperArmor(true, 999.f);
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Character_Battle_Dash::Tick_State(_float fTimeDelta)
{
    if (m_pModelCom->Get_Progress() <= 0.5f)
    {
        m_pTransformCom->Move(m_pTransformCom->Get_Look(), 15.f, fTimeDelta);
    }
    
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Character_Battle_Dash::Exit_State()
{
    m_iCurrAnimIndex = 0;
    m_bFirstRootConvert = true;
    m_pCharacter->Set_SuperArmor(false, 0.f);
}




CState_Character_Battle_Dash* CState_Character_Battle_Dash::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Character_Battle_Dash* pInstance = new CState_Character_Battle_Dash(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Character_Battle_Dash");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Character_Battle_Dash::Free()
{
    __super::Free();
}
