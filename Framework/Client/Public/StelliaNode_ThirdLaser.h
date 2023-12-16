#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "Stellia.h"

BEGIN(Client)

class CStelliaNode_ThirdLaser final : public CBTNode_Leaf
{
private:
	CStelliaNode_ThirdLaser();
	virtual ~CStelliaNode_ThirdLaser() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTemp = 0.f;

public:
	static CStelliaNode_ThirdLaser* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

