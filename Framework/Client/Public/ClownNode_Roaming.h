#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Roaming final : public CClownNode_Base
{
private:
	CClownNode_Roaming();
	virtual ~CClownNode_Roaming() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<_vector> m_vecRoamingArea;
	_uint			m_iRoamingIndex = 0;

	_bool		m_bIsRoaming = true;

public:
	static CClownNode_Roaming* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void Free() override;
};

END

