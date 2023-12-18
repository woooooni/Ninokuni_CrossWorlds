#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Stun final : public CClownNode_Base
{
private:
	CClownNode_Stun();
	virtual ~CClownNode_Stun() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float  m_fTime = 0.f;

public:
	static CClownNode_Stun* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

