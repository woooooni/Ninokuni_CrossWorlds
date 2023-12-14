#pragma once

#include "BTNode.h"

BEGIN(Engine)

class ENGINE_DLL CBTNode_Sequence final : public CBTNode
{
private:
	CBTNode_Sequence();
	virtual ~CBTNode_Sequence() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual CBTNode::NODE_STATE	Tick(const _float & fTimeDelta);

public:
	static CBTNode_Sequence* Create();
	virtual void Free() override;

};

END

