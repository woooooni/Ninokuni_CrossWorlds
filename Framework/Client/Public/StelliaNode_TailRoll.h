#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "Stellia.h"

BEGIN(Client)

class CStelliaNode_TailRoll final : public CBTNode_Leaf
{
private:
	CStelliaNode_TailRoll();
	virtual ~CStelliaNode_TailRoll() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTemp = 0.f;

public:
	static CStelliaNode_TailRoll* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

