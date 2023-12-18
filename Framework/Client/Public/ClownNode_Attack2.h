#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Attack2 final : public CClownNode_Base
{
private:
	CClownNode_Attack2();
	virtual ~CClownNode_Attack2() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CClownNode_Attack2* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

