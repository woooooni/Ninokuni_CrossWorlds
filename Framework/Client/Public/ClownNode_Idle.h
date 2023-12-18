#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Idle final : public CClownNode_Base
{
private:
	CClownNode_Idle();
	virtual ~CClownNode_Idle() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<wstring> vecAnimationName);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<wstring> m_vecAnimationName;
	_uint			m_iCurAnimationIndex = 0;

public:
	static CClownNode_Idle* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<wstring> vecAnimationName);
	virtual void Free() override;
};

END

