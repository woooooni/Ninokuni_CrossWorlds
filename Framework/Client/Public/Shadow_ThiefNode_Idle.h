#pragma once

#include "Shadow_ThiefNode_Base.h"

BEGIN(Client)

class CShadow_ThiefNode_Idle final : public CShadow_ThiefNode_Base
{
private:
	CShadow_ThiefNode_Idle();
	virtual ~CShadow_ThiefNode_Idle() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CShadow_ThiefNode_Idle* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

