#pragma once

#include "IceBearManNode_Base.h"

BEGIN(Client)

class CIceBearManNode_Idle final : public CIceBearManNode_Base
{
private:
	CIceBearManNode_Idle();
	virtual ~CIceBearManNode_Idle() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<wstring> vecAnimationName);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<wstring> m_vecAnimationName;
	_uint			m_iCurAnimationIndex = 0;

public:
	static CIceBearManNode_Idle* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<wstring> vecAnimationName);
	virtual void Free() override;
};

END

