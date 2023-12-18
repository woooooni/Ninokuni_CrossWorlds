#pragma once

#include "Baobam_DarkNode_Base.h"

BEGIN(Client)

class CBaobam_DarkNode_Dead final : public CBaobam_DarkNode_Base
{
private:
	CBaobam_DarkNode_Dead();
	virtual ~CBaobam_DarkNode_Dead() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CBaobam_DarkNode_Dead* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

