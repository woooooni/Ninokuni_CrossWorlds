#pragma once

#include "Criminal_MonsterNode_Base.h"

BEGIN(Client)

class CCriminal_MonsterNode_Chase final : public CCriminal_MonsterNode_Base
{
private:
	CCriminal_MonsterNode_Chase();
	virtual ~CCriminal_MonsterNode_Chase() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CCriminal_MonsterNode_Chase* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

