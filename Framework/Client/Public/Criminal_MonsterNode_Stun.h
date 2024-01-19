#pragma once

#include "Criminal_MonsterNode_Base.h"

BEGIN(Client)

class CCriminal_MonsterNode_Stun final : public CCriminal_MonsterNode_Base
{
private:
	CCriminal_MonsterNode_Stun();
	virtual ~CCriminal_MonsterNode_Stun() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float  m_fTime = 0.f;

public:
	static CCriminal_MonsterNode_Stun* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

