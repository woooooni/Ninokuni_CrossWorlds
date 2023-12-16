#pragma once

#include "BTNode.h"

#include "Model.h"

BEGIN(Engine)

class ENGINE_DLL CBTNode_Leaf abstract : public CBTNode
{
protected:
	CBTNode_Leaf();
	virtual ~CBTNode_Leaf() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree * pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float & fTimeDelta);

public:
	virtual void Free() override;

};

END

