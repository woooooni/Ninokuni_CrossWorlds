#include "stdafx.h"
#include "Shadow_ThiefNode_Roaming.h"

#include "BehaviorTree.h"

#include "Model.h"

CShadow_ThiefNode_Roaming::CShadow_ThiefNode_Roaming()
{
}

HRESULT CShadow_ThiefNode_Roaming::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<Vec4>* vecRoamingArea)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_vecRoamingArea = *vecRoamingArea;

	return S_OK;
}

void CShadow_ThiefNode_Roaming::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Walk"));
	m_bIsRoaming = true;
}

CBTNode::NODE_STATE CShadow_ThiefNode_Roaming::Tick(const _float& fTimeDelta)
{
	_float4 vPos;
	_float4 vDestPos;

	XMStoreFloat4(&vPos, m_tBTMonsterDesc.pOwnerTransform->Get_Position());
	XMStoreFloat4(&vDestPos, m_vecRoamingArea[m_iRoamingIndex]);

	if (m_bIsRoaming)
	{
		m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_vecRoamingArea[m_iRoamingIndex]);
		m_tBTMonsterDesc.pOwnerTransform->Move(m_tBTMonsterDesc.pOwnerTransform->Get_Look(), 1.f, fTimeDelta);

		if (vPos.x >= vDestPos.x - 0.1f && vPos.x <= vDestPos.x + 0.1f &&
			vPos.z >= vDestPos.z - 0.1f && vPos.z <= vDestPos.z + 0.1f)
		{
			m_iRoamingIndex += 1;
			if (m_iRoamingIndex == m_vecRoamingArea.size())
				m_iRoamingIndex = 0;

			m_bIsRoaming = false;
		}
	}
	else
		return NODE_STATE::NODE_SUCCESS;

	return NODE_STATE::NODE_RUNNING;
}

CShadow_ThiefNode_Roaming* CShadow_ThiefNode_Roaming::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<Vec4>* vecRoamingArea)
{
	CShadow_ThiefNode_Roaming* pInstance = new CShadow_ThiefNode_Roaming();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT, vecRoamingArea)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Roaming");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Roaming::Free()
{
	__super::Free();
}
