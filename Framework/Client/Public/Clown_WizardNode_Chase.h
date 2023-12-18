#pragma once

#include "Clown_WizardNode_Base.h"

BEGIN(Client)

class CClown_WizardNode_Chase final : public CClown_WizardNode_Base
{
private:
	CClown_WizardNode_Chase();
	virtual ~CClown_WizardNode_Chase() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CClown_WizardNode_Chase* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT);
	virtual void Free() override;
};

END

