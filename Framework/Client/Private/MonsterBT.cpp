#include "stdafx.h"
#include "MonsterBT.h"

#include "GameInstance.h"
#include "Monster.h"

CMonsterBT::CMonsterBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBehaviorTree(pDevice, pContext)
{
}

CMonsterBT::CMonsterBT(const CMonsterBT& rhs)
	: CBehaviorTree(rhs)
{
}

HRESULT CMonsterBT::Initialize_Prototype(CMonster* pOwner)
{
	m_tBTMonsterDesc.pOwner = pOwner;
	m_tBTMonsterDesc.pOwnerModel = pOwner->Get_Component<CModel>(L"Com_Model");
	m_tBTMonsterDesc.pOwnerTransform = pOwner->Get_Component<CTransform>(L"Com_Transform");

	return S_OK;
}

HRESULT CMonsterBT::Initialize(void* pArg)
{
	return S_OK;
}

void CMonsterBT::Tick(const _float& fTimeDelta)
{
}

void CMonsterBT::LateTick(const _float& fTimeDelta)
{
}

void CMonsterBT::Free()
{
	__super::Free();
}
