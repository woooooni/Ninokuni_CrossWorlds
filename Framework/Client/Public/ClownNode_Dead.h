#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Dead final : public CClownNode_Base
{
private:
	CClownNode_Dead();
	virtual ~CClownNode_Dead() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CClownNode_Dead* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

