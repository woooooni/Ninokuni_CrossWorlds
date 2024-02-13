#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_TagIn.h"

CState_Destroyer_TagIn::CState_Destroyer_TagIn(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_TagIn::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_TagIn::Enter_State(void* pArg)
{
    /*CHARACTER_TYPE eCharacterType = m_pCharacter->Get_CharacterType();
    wstring strSoundKey = L"";
    switch(eCharacterType)
    {
    case CHARACTER_TYPE::SWORD_MAN:
        strSoundKey = L"Swordsman_V_Death_" + to_wstring(GI->RandomInt(1, 2));
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    case CHARACTER_TYPE::ENGINEER:
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    case CHARACTER_TYPE::DESTROYER:
        CSound_Manager::GetInstance()->Play_Sound(strSoundKey, CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);
        break;
    }*/
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Destroyer_TagIn::Tick_State(_float fTimeDelta)
{
    m_pStateMachineCom->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
}

void CState_Destroyer_TagIn::Exit_State()
{

}

CState_Destroyer_TagIn* CState_Destroyer_TagIn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_TagIn* pInstance = new CState_Destroyer_TagIn(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_TagIn");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_TagIn::Free()
{
    __super::Free();
}
