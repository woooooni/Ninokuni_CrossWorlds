#pragma once

#include "BTNode.h"

BEGIN(Engine)

class ENGINE_DLL CBTNode_Select final : public CBTNode
{
private:
	CBTNode_Select();
	virtual ~CBTNode_Select() = default;

public:
	virtual HRESULT Initialize_Prototype(CBehaviorTree * pBT = nullptr);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float & fTimeDelta);

private:
	list<CBTNode*>::iterator m_curIter;
	NODE_STATE m_eResult = NODE_STATE::NODE_FAIL;

public:
	static CBTNode_Select* Create(CBehaviorTree * pBT = nullptr);
	virtual void Free() override;

};

END

