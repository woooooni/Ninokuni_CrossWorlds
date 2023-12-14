#include "BehaviorTree.h"

CBehaviorTree::CBehaviorTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CBehaviorTree::CBehaviorTree(const CBehaviorTree& rhs)
	: CComponent(rhs)
{
}

HRESULT CBehaviorTree::Initialize_Prototype(CGameObject* pObject)
{
	return S_OK;
}

HRESULT CBehaviorTree::Initialize(void* pArg)
{
	return S_OK;
}

void CBehaviorTree::Tick(const _float& fTimeDelta)
{
	m_pRootNode->Tick(fTimeDelta);
}

void CBehaviorTree::LateTick(const _float& fTimeDelta)
{
}

void CBehaviorTree::Free()
{
	__super::Free();

	for (auto& iter : m_BlackBoard)
		Safe_Delete(iter.second);

	m_BlackBoard.clear();
	Safe_Release(m_pRootNode);
}
