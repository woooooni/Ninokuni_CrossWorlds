#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_TagOut.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Particle_Manager.h"

CState_SwordMan_TagOut::CState_SwordMan_TagOut(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_TagOut::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_TagOut::Enter_State(void* pArg)
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

    GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Change_Player"),
        m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 1.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
}

void CState_SwordMan_TagOut::Tick_State(_float fTimeDelta)
{
    CGame_Manager::GetInstance()->Get_Player()->Switch_Tag_Character();
}

void CState_SwordMan_TagOut::Exit_State()
{

}

CState_SwordMan_TagOut* CState_SwordMan_TagOut::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_TagOut* pInstance = new CState_SwordMan_TagOut(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_TagOut");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_TagOut::Free()
{
    __super::Free();
}
