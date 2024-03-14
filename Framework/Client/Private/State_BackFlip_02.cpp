#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_BackFlip_02.h"
#include "Whale.h"

CState_BackFlip_02::CState_BackFlip_02(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_BackFlip_02::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;



	return S_OK;
}

void CState_BackFlip_02::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_BackFlip_02::Tick_State(_float fTimeDelta)
{
	CWhale* pWhale = static_cast<CWhale*>(m_pOwner);

	if (0 == pWhale->Get_RomingPoints()->size())
		return;

	_int iCurRomingPoint = pWhale->Get_CurRomingPoint();

	Vec4 vPos = m_pTransformCom->Get_Position();
	Vec4 vNextPos = pWhale->Get_RomingPointIndex(iCurRomingPoint + 1);

	if (0 == iCurRomingPoint)
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_SWIM, &vNextPos);

	Vec4 vDir = vNextPos - vPos;
	vDir.Normalize();

	m_pTransformCom->Move(vDir, *pWhale->Get_Speed(), fTimeDelta);
	m_pTransformCom->Rotation_Acc(::XMVector3Normalize(m_pTransformCom->Get_Look()), ::XMConvertToRadians(fTimeDelta * 25.0f));

	if (vPos.x >= vNextPos.x - 0.3f && vPos.x <= vNextPos.x + 0.3f &&
		vPos.y >= vNextPos.y - 0.3f && vPos.y <= vNextPos.y + 0.3f &&
		vPos.z >= vNextPos.z - 0.3f && vPos.z <= vNextPos.z + 0.3f)
	{
		pWhale->Set_CurRomingPoint(pWhale->Get_CurRomingPoint() + 1);


		if (pWhale->Get_CurRomingPoint() == pWhale->Get_RomingPoints()->size())
			pWhale->Set_CurRomingPoint(0);
	}

}

void CState_BackFlip_02::Exit_State()
{

}

CState_BackFlip_02* CState_BackFlip_02::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_BackFlip_02* pInstance = new CState_BackFlip_02(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_BackFlip_02");
		Safe_Release<CState_BackFlip_02*>(pInstance);
	}

	return pInstance;
}

void CState_BackFlip_02::Free()
{
	__super::Free();
}
