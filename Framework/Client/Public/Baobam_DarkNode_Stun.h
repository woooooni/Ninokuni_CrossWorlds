#pragma once

#include "Baobam_DarkNode_Base.h"

BEGIN(Client)

class CBaobam_DarkNode_Stun final : public CBaobam_DarkNode_Base
{
private:
	CBaobam_DarkNode_Stun();
	virtual ~CBaobam_DarkNode_Stun() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float  m_fTime = 0.f;

public:
	static CBaobam_DarkNode_Stun* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

