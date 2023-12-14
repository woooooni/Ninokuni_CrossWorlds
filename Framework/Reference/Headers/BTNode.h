#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class CBehaviorTree;
class ENGINE_DLL CBTNode abstract : public CBase
{
public:
	enum class NODE_STATE { NODE_RUNNING, NODE_FAIL, NODE_SUCCESS, NODE_END };

protected:
	CBTNode();
	virtual ~CBTNode() = default;

public:
	virtual HRESULT Initialize_Prototype(CBehaviorTree* pBT);
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	void Add_ChildNode(CBTNode* pNode);

protected:
	list<CBTNode*> m_ltChildNode;
	CBehaviorTree* m_pBT = nullptr;

public:
	virtual void Free() override;
};

END

