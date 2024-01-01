#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Attack1 final : public CClownNode_Base
{
private:
	CClownNode_Attack1();
	virtual ~CClownNode_Attack1() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CClownNode_Attack1* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

