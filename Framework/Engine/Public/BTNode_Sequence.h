#pragma once

#include "BTNode.h"

BEGIN(Engine)

class ENGINE_DLL CBTNode_Sequence final : public CBTNode
{
private:
	CBTNode_Sequence();
	virtual ~CBTNode_Sequence() = default;

public:
	virtual HRESULT Initialize_Prototype(CBehaviorTree * pBT = nullptr);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float & fTimeDelta);

private:
	list<CBTNode*>::iterator m_curIter;
	NODE_STATE m_eResult = NODE_STATE::NODE_FAIL;

public:
	static CBTNode_Sequence* Create(CBehaviorTree * pBT = nullptr);
	virtual void Free() override;

};

END

