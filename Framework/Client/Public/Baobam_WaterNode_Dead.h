#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "Baobam_Water.h"

BEGIN(Client)

class CBaobam_WaterNode_Dead final : public CBTNode_Leaf
{
private:
	CBaobam_WaterNode_Dead();
	virtual ~CBaobam_WaterNode_Dead() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTemp = 0.f;

public:
	static CBaobam_WaterNode_Dead* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

