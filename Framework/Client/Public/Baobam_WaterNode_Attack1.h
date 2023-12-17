#pragma once

#include "Baobam_WaterNode_Base.h"

BEGIN(Client)

class CBaobam_WaterNode_Attack1 final : public CBaobam_WaterNode_Base
{
private:
	CBaobam_WaterNode_Attack1();
	virtual ~CBaobam_WaterNode_Attack1() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CBaobam_WaterNode_Attack1* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

