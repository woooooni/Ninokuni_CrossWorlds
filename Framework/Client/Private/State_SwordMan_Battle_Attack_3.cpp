#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Battle_Attack_3.h"
#include "Utils.h"
#include "Weapon.h"
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

    m_bGenTrail = false;
    m_iGenTrailCount = 0;
    m_pCharacter->Appear_Weapon();
    m_pCharacter->Look_For_Target();
    m_pModelCom->Set_Animation(m_AnimIndices[0], MIN_TWEEN_DURATION);


}

void CState_SwordMan_Battle_Attack_3::Tick_State(_float fTimeDelta)
{
    /*if (m_pModelCom->Get_Progress() >= 0.2f && m_pModelCom->Get_Progress() <= 0.3f)
        m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), 6.f, fTimeDelta);*/

    if (false == m_pModelCom->Is_Tween())
    {
        if (m_pCharacter->Get_Collider(CCollider::DETECTION_TYPE::ATTACK)[0]->Is_Active())
        {
            Vec3 vDir = 1.f * XMVector3Normalize((m_pTransformCom->Get_Look()));
            m_pTransformCom->Move(XMVector3Normalize(vDir), 10.f, fTimeDelta);
        }

        if (2 > m_iGenTrailCount)
        {
            if (m_pModelCom->Get_CurrAnimationFrame() >= 10.f)
            {
                if (false == m_bGenTrail)
                {
                    m_iGenTrailCount++;
                    m_pCharacter->Get_Weapon()->Generate_Trail(L"", L"", L"Distortion_1.png", Vec4(1.f, 0.8f, 0.f, 1.f), 4);
                    m_bGenTrail = true;
                }
                    
            }
            
            if (m_pModelCom->Get_CurrAnimationFrame() >= 20.f)
            {
                if (true == m_bGenTrail)
                {
                    m_iGenTrailCount++;
                    m_pCharacter->Get_Weapon()->Stop_Trail();
                }
            }
        }
        
        
    }    
        
    if (true == CCamera_Manager::GetInstance()->Get_CurCamera()->Is_Lerp_Fov())
        m_pCharacter->Get_RendererCom()->Set_RadialBlur(true);
    

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_SwordMan_Battle_Attack_3::Exit_State()
{
    if (CAMERA_TYPE::FOLLOW == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
        CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Lerp_Fov(XMConvertToRadians(60.f), 0.2f, LERP_MODE::EASE_IN);

    m_bGenTrail = false;
    m_iGenTrailCount = 0;
    m_pCharacter->Get_RendererCom()->Set_RadialBlur(false);
    m_pCharacter->Get_Weapon()->Stop_Trail();
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
