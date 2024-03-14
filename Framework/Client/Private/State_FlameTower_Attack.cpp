#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Defence_Tower.h"
#include "Effect_Manager.h"
#include "State_FlameTower_Attack.h"
#include "Effect.h"

CState_FlameTower_Attack::CState_FlameTower_Attack(CStateMachine* pMachine)
    : CState_DefenceTower(pMachine)
{
}

HRESULT CState_FlameTower_Attack::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_FlameTower_Attack::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
    m_fAccOnOffAttack = 0.f;
    m_fOnOffAttackTime = 0.5f;
    m_bFire = false;

    wstring strSoundKey = L"Obj_FlareTower_Shot_" + to_wstring(GI->RandomInt(1, 2)) + L".mp3";
    GI->Play_Sound(strSoundKey, CHANNELID::SOUND_AIRPLANE, 0.5f);
}

void CState_FlameTower_Attack::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_CurrAnimationFrame() == 5)
    {
        if (false == m_bFire)
        {
            Fire();
            m_bFire = true;
        }
        else
        {
            wstring strSoundKey = L"Ele_Shot_Fire_" + to_wstring(GI->RandomInt(1, 4)) + L".mp3";
            GI->Play_Sound(strSoundKey, CHANNELID::SOUND_BOSS, 0.5f);
        }
    }

    m_fAccOnOffAttack += fTimeDelta;
    if (m_fAccOnOffAttack >= m_fOnOffAttackTime)
    {
        m_pTower->Set_Collider_AttackMode(CCollider::ATTACK_TYPE::WEAK, 0.f, 0.f, 0.f, false);

        _bool bActive = m_pTower->Get_Collider(CCollider::DETECTION_TYPE::ATTACK)[0]->Is_Active();
        m_pTower->Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, !bActive);

        m_fAccOnOffAttack = 0.f;
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CDefence_Tower::DEFENCE_TOWER_STATE::TOWER_STATE_IDLE);
}

void CState_FlameTower_Attack::Exit_State()
{
    m_bFire = false;
    m_fAccOnOffAttack = 0.f;
    if (nullptr != m_pEffect_InBound)
    {
        m_pEffect_InBound->Set_Dead(true);
        Safe_Release(m_pEffect_InBound);
        m_pEffect_InBound = nullptr;
    }

    if (nullptr != m_pEffect_OutBound)
    {
        m_pEffect_OutBound->Set_Dead(true);
        Safe_Release(m_pEffect_OutBound);
        m_pEffect_OutBound = nullptr;
    }

    m_pTower->Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
}

void CState_FlameTower_Attack::Fire()
{
    _matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
    WorldMatrix.r[CTransform::STATE_POSITION] += XMVectorSet(0.f, 0.2f, 0.f, 0.f);
    CEffect_Manager::GetInstance()->Generate_Effect(L"Defence_FlameTower_Fire_1", WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(10.f, 10.f, 10.f), _float3(0.f, 0.f, 0.f));
    
    CEffect_Manager::GetInstance()->Generate_Effect(L"Defence_FlameTower_Fire_0", WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(10.f, 10.f, 10.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pEffect_InBound);

    CEffect_Manager::GetInstance()->Generate_Effect(L"Defence_FlameTower_Fire_0", WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(20.f, 20.f, 20.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pEffect_OutBound);

    Safe_AddRef(m_pEffect_InBound);
    Safe_AddRef(m_pEffect_OutBound);
}

CState_FlameTower_Attack* CState_FlameTower_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_FlameTower_Attack* pInstance = new CState_FlameTower_Attack(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_FlameTower_Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_FlameTower_Attack::Free()
{
    __super::Free();

    if (nullptr != m_pEffect_InBound)
    {
        m_pEffect_InBound->Set_Dead(true);
        Safe_Release(m_pEffect_InBound);
    }

    if (nullptr != m_pEffect_OutBound)
    {
        m_pEffect_OutBound->Set_Dead(true);
        Safe_Release(m_pEffect_OutBound);
    }
}
