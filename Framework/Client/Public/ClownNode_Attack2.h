#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Attack2 final : public CClownNode_Base
{
private:
	CClownNode_Attack2();
	virtual ~CClownNode_Attack2() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CClownNode_Attack2* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

