#pragma once

#include "PumpkinCandleNode_Base.h"

BEGIN(Client)

class CPumpkinCandleNode_Stun final : public CPumpkinCandleNode_Base
{
private:
	CPumpkinCandleNode_Stun();
	virtual ~CPumpkinCandleNode_Stun() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float  m_fTime = 0.f;

public:
	static CPumpkinCandleNode_Stun* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

