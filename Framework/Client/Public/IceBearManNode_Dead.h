#pragma once

#include "IceBearManNode_Base.h"

BEGIN(Client)

class CIceBearManNode_Dead final : public CIceBearManNode_Base
{
private:
	CIceBearManNode_Dead();
	virtual ~CIceBearManNode_Dead() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CIceBearManNode_Dead* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

