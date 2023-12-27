#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_Animal_Fly.h"

CState_Animal_Fly::CState_Animal_Fly(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_Animal_Fly::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Animal_Fly::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);

	m_OriginMatrix = m_pTransformCom->Get_WorldFloat4x4();
}

void CState_Animal_Fly::Tick_State(_float fTimeDelta)
{
	Quaternion quat;
	Matrix MatRot;
	Vec4 vOut;

	Vec3 vPoint = Vec3(-0.724f, 10.430f, 111.816f);

	m_pTransformCom->RevolutionRotation(vPoint, Vec3(0.0f, 1.0f, 0.0f), ::XMConvertToRadians(m_fAngle));
}

void CState_Animal_Fly::Exit_State()
{

}

CState_Animal_Fly* CState_Animal_Fly::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Animal_Fly* pInstance = new CState_Animal_Fly(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Animal_Fly");
		Safe_Release<CState_Animal_Fly*>(pInstance);
	}

	return pInstance;
}

void CState_Animal_Fly::Free()
{
	__super::Free();
}
