#pragma once

#include "NpcNode_Base.h"

BEGIN(Client)
class CNpc_RoamingNode final : public CNpcNode_Base
{
	CNpc_RoamingNode();
	virtual ~CNpc_RoamingNode() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<_vector> m_vecRoamingArea;
	_uint			m_iRoamingIndex = 0;

	_bool		m_bIsRoaming = true;

	wstring		m_strAnimName = TEXT("");

public:
	static CNpc_RoamingNode* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void Free() override;

};
END

