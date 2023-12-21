#pragma once

#include "IceBearManNode_Base.h"

BEGIN(Client)

class CIceBearManNode_Attack2 final : public CIceBearManNode_Base
{
private:
	CIceBearManNode_Attack2();
	virtual ~CIceBearManNode_Attack2() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CIceBearManNode_Attack2* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

