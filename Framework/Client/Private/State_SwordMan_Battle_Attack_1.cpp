#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Battle_Attack_1.h"

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
}

void CState_SwordMan_Battle_Attack_1::Tick_State(_float fTimeDelta)
{
    /*if (m_pModelCom->Get_Progress() >= 0.1f && m_pModelCom->Get_Progress() <= 0.2f)
        m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), 4.f, fTimeDelta);*/

    if (m_pModelCom->Get_Progress() <= 0.25f && m_pCharacter->Get_Collider(CCollider::DETECTION_TYPE::ATTACK)[0]->Is_Active())
    {
        Vec3 vDir = -1.f * (XMVector3Normalize(m_pTransformCom->Get_Right()) * 0.9f) + XMVector3Normalize((m_pTransformCom->Get_Look()) * 0.1f);
        m_pTransformCom->Move(XMVector3Normalize(vDir), 20.f, fTimeDelta);
        m_pCharacter->Look_For_Target();

        if (false == m_bGenMotionTrail[0])
        {
            m_pCharacter->Create_MotionTrail(m_MotionTrailDesc);
            m_bGenMotionTrail[0] = true;
        }
        
    }

    if(m_pModelCom->Get_Progress() > 0.25f && m_pCharacter->Get_Collider(CCollider::DETECTION_TYPE::ATTACK)[0]->Is_Active())
    {
        Vec3 vDir = 1.f * (XMVector3Normalize(m_pTransformCom->Get_Right()) * 0.95f) + XMVector3Normalize((m_pTransformCom->Get_Look()) * 0.05f);
        m_pCharacter->Look_For_Target();
        m_pTransformCom->Move(XMVector3Normalize(vDir), 30.f, fTimeDelta);

        if (false == m_bGenMotionTrail[1])
        {
            m_pCharacter->Create_MotionTrail(m_MotionTrailDesc);
            m_bGenMotionTrail[1] = true;
        }
    }
       

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

    __super::Attack_Input(fTimeDelta);
}

void CState_SwordMan_Battle_Attack_1::Exit_State()
{
    for (_uint i = 0; i < 2; ++i)
        m_bGenMotionTrail[i] = false;

    m_pCharacter->Stop_MotionTrail();
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
