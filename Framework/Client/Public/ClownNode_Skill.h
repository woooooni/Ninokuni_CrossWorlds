#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Skill final : public CClownNode_Base
{
private:
	CClownNode_Skill();
	virtual ~CClownNode_Skill() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CClownNode_Skill* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

