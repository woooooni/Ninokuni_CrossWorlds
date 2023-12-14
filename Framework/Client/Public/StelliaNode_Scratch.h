#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "Stellia.h"

BEGIN(Client)

class CStelliaNode_Scratch final : public CBTNode_Leaf
{
private:
	CStelliaNode_Scratch();
	virtual ~CStelliaNode_Scratch() = default;

public:
	virtual HRESULT Initialize_Prototype(CBehaviorTree* pBT);
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTemp = 0.f;

public:
	static CStelliaNode_Scratch* Create(CBehaviorTree* pBT);
	virtual void Free() override;
};

END
