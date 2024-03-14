#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Hit final : public CClownNode_Base
{
private:
	CClownNode_Hit();
	virtual ~CClownNode_Hit() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CClownNode_Hit* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

