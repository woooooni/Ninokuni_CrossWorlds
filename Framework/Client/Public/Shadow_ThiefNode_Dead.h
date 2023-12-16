#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "Shadow_Thief.h"

BEGIN(Client)

class CShadow_ThiefNode_Dead final : public CBTNode_Leaf
{
private:
	CShadow_ThiefNode_Dead();
	virtual ~CShadow_ThiefNode_Dead() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTemp = 0.f;

public:
	static CShadow_ThiefNode_Dead* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

