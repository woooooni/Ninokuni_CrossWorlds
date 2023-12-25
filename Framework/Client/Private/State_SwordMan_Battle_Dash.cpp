#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Animation.h"
#include "State_SwordMan_Battle_Dash.h"

CState_SwordMan_Battle_Dash::CState_SwordMan_Battle_Dash(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Battle_Dash::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    return S_OK;
}

void CState_SwordMan_Battle_Dash::Enter_State(void* pArg)
{
    // 마우스 방향으로 구르기
    POINT pt = GI->GetMousePos();

    _vector vMousePos = XMVectorSet(
        _float(pt.x / (g_iWinSizeX * .5f) - 1.f),
        _float(pt.y / (g_iWinSizeY * -.5f) + 1.f),
        1.f, 1.f);



    _matrix ViewMatrixInv = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
    _matrix ProjMatrixInv = GI->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);

    vMousePos = XMVector3TransformCoord(vMousePos, ProjMatrixInv);
    vMousePos = XMVector3TransformCoord(vMousePos, ViewMatrixInv);


    m_vDir = XMVector3Normalize(vMousePos - m_pTransformCom->Get_Position());
    m_pTransformCom->LookAt_ForLandObject(XMLoadFloat3(&m_vDir) + m_pTransformCom->Get_Position());

    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Battle_Dash::Tick_State(_float fTimeDelta)
{
    m_pTransformCom->Move(m_pTransformCom->Get_Look(), 2.f, fTimeDelta);

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_SwordMan_Battle_Dash::Exit_State()
{
    m_iCurrAnimIndex = 0;
}


CState_SwordMan_Battle_Dash* CState_SwordMan_Battle_Dash::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Battle_Dash* pInstance = new CState_SwordMan_Battle_Dash(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Battle_Dash");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Battle_Dash::Free()
{
    __super::Free();
}
