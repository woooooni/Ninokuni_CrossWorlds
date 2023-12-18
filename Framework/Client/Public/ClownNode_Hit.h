#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Hit final : public CClownNode_Base
{
private:
	CClownNode_Hit();
	virtual ~CClownNode_Hit() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CClownNode_Hit* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

