#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_SpecialSkill_BattleCry.h"

#include "Effect_Manager.h"
#include "Monster.h"

CState_Destroyer_SpecialSkill_BattleCry::CState_Destroyer_SpecialSkill_BattleCry(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_SpecialSkill_BattleCry::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_SpecialSkill_BattleCry::Enter_State(void* pArg)
{
    wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
    CSound_Manager::GetInstance()->Play_Sound(L"Destroyer_V_Atk_Cast_Long_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_pCharacter->Look_For_Target();
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Destroyer_Skill_BattleCry"), pTransformCom->Get_WorldMatrix(), m_pCharacter);

    m_bProvocation = false;
    
}

void CState_Destroyer_SpecialSkill_BattleCry::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_CurrAnimationFrame_WithRatio() >= 0.5f)
    {
        if (false == m_bProvocation)
        {
            m_bProvocation = true;

            list<CGameObject*>& Monsters = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER);
            for (auto& pObject : Monsters)
            {
                if (pObject->Is_Dead() || pObject->Is_ReserveDead() || pObject->Get_ObjectType() != OBJ_TYPE::OBJ_MONSTER)
                    continue;

                CMonster* pMonster = dynamic_cast<CMonster*>(pObject);                
                if (nullptr == pMonster)
                    continue;

                CTransform* pMonsterTransform = pMonster->Get_Component_Transform();
                if (nullptr == pMonsterTransform)
                    continue;

                Vec3 vDir = m_pTransformCom->Get_Position() - pMonsterTransform->Get_Position();

                if (vDir.Length() <= 30.f)
                    pMonster->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, true);
            }
        }
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);

}

void CState_Destroyer_SpecialSkill_BattleCry::Exit_State()
{
    m_bProvocation = false;
}

CState_Destroyer_SpecialSkill_BattleCry* CState_Destroyer_SpecialSkill_BattleCry::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_SpecialSkill_BattleCry* pInstance = new CState_Destroyer_SpecialSkill_BattleCry(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_Destroyer_SpecialSkill_BattleCry");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_SpecialSkill_BattleCry::Free()
{
    __super::Free();
}
