#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Camera_Manager.h"
#include "State_Destroyer_Skill_LeafSlam.h"
#include "Effect_Manager.h"

CState_Destroyer_Skill_LeafSlam::CState_Destroyer_Skill_LeafSlam(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_Skill_LeafSlam::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_MotionTrailDesc.fAlphaSpeed = 0.05f;
    m_MotionTrailDesc.fBlurPower = 0.f;
    m_MotionTrailDesc.vRimColor = { 0.2f, 0.8f, 1.f, 1.f };
    m_MotionTrailDesc.vBloomPower = { 0.2f, 0.8f, 1.f };
    m_MotionTrailDesc.fMotionTrailTime = 0.1f;
    
    return S_OK;
}

void CState_Destroyer_Skill_LeafSlam::Enter_State(void* pArg)
{
    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
    CSound_Manager::GetInstance()->Play_Sound(L"Destroyer_V_Atk_Medium_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_pCharacter->Appear_Weapon();
    m_iCurrAnimIndex = m_AnimIndices[0];
    m_pModelCom->Set_Animation(m_iCurrAnimIndex);
    m_pCharacter->Look_For_Target();

    m_bTrailStart = false;

    if (false == m_bTrailStart)
    {
        m_bTrailStart = true;
        m_pCharacter->Generate_MotionTrail(m_MotionTrailDesc);
    }

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Destroyer_Skill_LeafSlam"), pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_Destroyer_Skill_LeafSlam::Tick_State(_float fTimeDelta)
{
    if (m_iCurrAnimIndex == m_AnimIndices[0])
    {
        m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), 15.f, fTimeDelta);
    }
    

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
    {
        if (m_iCurrAnimIndex == m_AnimIndices[0])
        {
            m_iCurrAnimIndex = m_AnimIndices[1];
            m_pModelCom->Set_Animation(m_iCurrAnimIndex);
            CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
            m_pCharacter->Stop_MotionTrail();
            
        }
        else
        {
            m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
        }
    }

    __super::Attack_Input(fTimeDelta);
}

void CState_Destroyer_Skill_LeafSlam::Exit_State()
{
    m_bTrailStart = false;
}


CState_Destroyer_Skill_LeafSlam* CState_Destroyer_Skill_LeafSlam::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_Skill_LeafSlam* pInstance = new CState_Destroyer_Skill_LeafSlam(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_Skill_LeafSlam::Free()
{
    __super::Free();
}
