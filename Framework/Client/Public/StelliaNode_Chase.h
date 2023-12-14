#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "Stellia.h"

BEGIN(Client)

class CStelliaNode_Chase final : public CBTNode_Leaf
{
private:
	CStelliaNode_Chase();
	virtual ~CStelliaNode_Chase() = default;

public:
	virtual HRESULT Initialize_Prototype(CBehaviorTree* pBT);
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTemp = 0.f;

public:
	static CStelliaNode_Chase* Create(CBehaviorTree* pBT);
	virtual void Free() override;
};

END

