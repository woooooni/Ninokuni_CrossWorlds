#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_Skill_WheelWind.h"

#include "Effect_Manager.h"

CState_Destroyer_Skill_WheelWind::CState_Destroyer_Skill_WheelWind(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_Skill_WheelWind::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_Skill_WheelWind::Enter_State(void* pArg)
{
	wstring strVoiceNum = to_wstring(CUtils::Random_Int(1, 3));
	CSound_Manager::GetInstance()->Play_Sound(L"Destroyer_V_Atk_Cast_Long_" + strVoiceNum + L".mp3", CHANNELID::SOUND_VOICE_CHARACTER, 0.5f, true);

    m_iCurrAnimIndex = m_AnimIndices[0];
	m_fAccFinish = 0.f;
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_iCurrAnimIndex, MIN_TWEEN_DURATION);

    // Effect Create
    CTransform* pTransformCom = m_pCharacter->Get_Component<CTransform>(L"Com_Transform");
    if (pTransformCom == nullptr)
        return;
    GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Destroyer_Skill_WheelWind"), pTransformCom->Get_WorldMatrix(), m_pCharacter);
}

void CState_Destroyer_Skill_WheelWind::Tick_State(_float fTimeDelta)
{
    if (m_iCurrAnimIndex == m_AnimIndices[0])
    {
        if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        {
            if (KEY_HOLD(KEY::NUM_1))
            {
                m_iCurrAnimIndex = m_AnimIndices[1];
                m_pModelCom->Set_Animation(m_iCurrAnimIndex, MIN_TWEEN_DURATION);
            }
            else
            {
                m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
                return;
            }
        }
        
    }
    else if (m_iCurrAnimIndex == m_AnimIndices[1])
    {
		Input(fTimeDelta);
		m_fAccFinish += fTimeDelta;
		if (KEY_NONE(KEY::NUM_1))
		{
			m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
			return;
		}

		
		if (m_fAccFinish >= m_fFinishTime)
		{
			m_fAccFinish = 0.f;
			m_iCurrAnimIndex = m_AnimIndices[2];
			m_pModelCom->Set_Animation(m_iCurrAnimIndex, MIN_TWEEN_DURATION);
		}
    }
	else if (m_iCurrAnimIndex == m_AnimIndices[2])
	{
		if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
			m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
	}
}

void CState_Destroyer_Skill_WheelWind::Exit_State()
{
	m_fAccFinish = 0.f;
}

void CState_Destroyer_Skill_WheelWind::Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::CTRL))
	{
		m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_DASH);
		return;
	}

	_bool bMove = false;
	if (KEY_HOLD(KEY::W))
	{
		bMove = true;


		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * m_fMoveSpeed * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);
	}


	if (KEY_HOLD(KEY::S))
	{
		bMove = true;

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamLook = vCamWolrd.r[CTransform::STATE_LOOK];

		vRight = XMVector3Normalize(vRight);
		vCamLook = -1.f * XMVector3Normalize(vCamLook);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamLook)) * m_fMoveSpeed * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);
		m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);
	}


	if (KEY_HOLD(KEY::A))
	{
		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = -1.f * XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * m_fMoveSpeed * fTimeDelta;

		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);

		bMove = true;

	}


	if (KEY_HOLD(KEY::D))
	{
		// m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 10.f, -1.f * fTimeDelta);

		_matrix vCamWolrd = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);

		_vector vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		_vector vCamRight = vCamWolrd.r[CTransform::STATE_RIGHT];

		vRight = XMVector3Normalize(vRight);
		vCamRight = XMVector3Normalize(vCamRight);

		_float fRadian = XMVectorGetX(XMVector3Dot(vRight, vCamRight)) * m_fMoveSpeed * fTimeDelta;


		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

		if (!bMove)
			m_pTransformCom->Move(XMVector3Normalize(m_pTransformCom->Get_Look()), m_fMoveSpeed, fTimeDelta);

		bMove = true;
	}
}

CState_Destroyer_Skill_WheelWind* CState_Destroyer_Skill_WheelWind::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_Skill_WheelWind* pInstance = new CState_Destroyer_Skill_WheelWind(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_Skill_WheelWind::Free()
{
    __super::Free();
}
