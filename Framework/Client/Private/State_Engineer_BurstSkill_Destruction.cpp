#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Effect_Manager.h"
#include "Character_Projectile.h"
#include "State_Engineer_BurstSkill_Destruction.h"


CState_Engineer_BurstSkill_Destruction::CState_Engineer_BurstSkill_Destruction(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Engineer_BurstSkill_Destruction::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Engineer_BurstSkill_Destruction::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_pCharacter->Look_For_Target();

    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
    CSound_Manager::GetInstance()->Play_Sound(L"Engineer_V_Atk_Cast_Long_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_pCharacter->Appear_Weapon();
    
    // Effect Create
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Engineer_Skill_Destruction"), m_pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_Engineer_BurstSkill_Destruction::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Engineer_BurstSkill_Destruction::Exit_State()
{
    
}

CState_Engineer_BurstSkill_Destruction* CState_Engineer_BurstSkill_Destruction::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Engineer_BurstSkill_Destruction* pInstance = new CState_Engineer_BurstSkill_Destruction(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Engineer_BurstSkill_Destruction");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Engineer_BurstSkill_Destruction::Free()
{
    __super::Free();
}
