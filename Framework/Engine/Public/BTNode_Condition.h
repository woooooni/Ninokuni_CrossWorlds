#pragma once

#include "BTNode.h"

BEGIN(Engine)

class ENGINE_DLL CBTNode_Condition final : public CBTNode
{
private:
	CBTNode_Condition();
	virtual ~CBTNode_Condition() = default;

public:
	virtual HRESULT Initialize_Prototype(function<_bool()> conditionFunc);
	virtual CBTNode::NODE_STATE	Tick(const _float & fTimeDelta);

private:
	function<_bool()> m_conditionFunc;

public:
	static CBTNode_Condition* Create(function<_bool()> conditionFunc);
	virtual void Free() override;
};

END

