#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "Shadow_Thief.h"

BEGIN(Client)

class CShadow_ThiefNode_Idle final : public CBTNode_Leaf
{
private:
	CShadow_ThiefNode_Idle();
	virtual ~CShadow_ThiefNode_Idle() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTemp = 0.f;

public:
	static CShadow_ThiefNode_Idle* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

