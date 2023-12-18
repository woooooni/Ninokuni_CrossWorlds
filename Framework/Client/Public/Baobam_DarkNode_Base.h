#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "Baobam_Dark.h"

BEGIN(Client)

class CBaobam_DarkNode_Base abstract : public CBTNode_Leaf
{
public:
	CBaobam_DarkNode_Base();
	virtual ~CBaobam_DarkNode_Base() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

	virtual CBTNode::NODE_STATE Atk_BehaviorTick(const wstring& strAnimName, _float fDestWaitTime, const _float& fTimeDelta);

	virtual _bool Behavior_Wait(_float fDestWaitTime, const _float& fTimeDelta);

protected:
	_bool	m_bWait = false;

private:
	_float  m_fSourWaitTime = 0.f;

public:
	virtual void Free() override;
};

END

