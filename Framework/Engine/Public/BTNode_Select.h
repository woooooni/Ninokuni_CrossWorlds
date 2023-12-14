#pragma once

#include "BTNode.h"

BEGIN(Engine)

class ENGINE_DLL CBTNode_Select final : public CBTNode
{
private:
	CBTNode_Select();
	virtual ~CBTNode_Select() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual CBTNode::NODE_STATE	Tick(const _float & fTimeDelta);

public:
	static CBTNode_Select* Create();
	virtual void Free() override;

};

END

