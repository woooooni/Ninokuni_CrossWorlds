#pragma once

#include "BTNode.h"

BEGIN(Engine)

class ENGINE_DLL CBTNode_Loop final : public CBTNode
{
private:
	CBTNode_Loop();
	virtual ~CBTNode_Loop() = default;

public:
	virtual HRESULT Initialize_Prototype(function<_bool()> conditionFunc);
	virtual CBTNode::NODE_STATE	Tick(const _float & fTimeDelta);

private:
	function<_bool()> m_conditionFunc;

public:
	static CBTNode_Loop* Create(function<_bool()> conditionFunc);
	virtual void Free() override;
};

END

