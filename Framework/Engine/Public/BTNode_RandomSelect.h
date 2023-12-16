#pragma once

#include "BTNode.h"

BEGIN(Engine)

class ENGINE_DLL CBTNode_RandomSelect final : public CBTNode
{
private:
	CBTNode_RandomSelect();
	virtual ~CBTNode_RandomSelect() = default;

public:
	virtual HRESULT Initialize_Prototype(CBehaviorTree * pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float & fTimeDelta);

private:
	_int iIndex = 0;
	_bool m_bIsTemp = false;
public:
	static CBTNode_RandomSelect* Create(CBehaviorTree * pBT);
	virtual void Free() override;

};

END

