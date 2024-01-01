#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Dead final : public CClownNode_Base
{
private:
	CClownNode_Dead();
	virtual ~CClownNode_Dead() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CClownNode_Dead* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

