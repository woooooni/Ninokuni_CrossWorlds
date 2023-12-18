#pragma once

#include "PumpkinCandleNode_Base.h"

BEGIN(Client)

class CPumpkinCandleNode_Roaming final : public CPumpkinCandleNode_Base
{
private:
	CPumpkinCandleNode_Roaming();
	virtual ~CPumpkinCandleNode_Roaming() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<_vector> m_vecRoamingArea;
	_uint			m_iRoamingIndex = 0;

	_bool		m_bIsRoaming = true;

public:
	static CPumpkinCandleNode_Roaming* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void Free() override;
};

END

