#pragma once

#include "Criminal_MonsterNode_Base.h"

BEGIN(Client)

class CCriminal_MonsterNode_Air final : public CCriminal_MonsterNode_Base
{
private:
	CCriminal_MonsterNode_Air();
	virtual ~CCriminal_MonsterNode_Air() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fCheckTime = 0.f;

	_float m_fTime = 0.f;
	_float m_fAirTime = 0.f;

	_float m_fForcedStandUpTime = 0.f;
	_float m_fAccForcedStandUpTime = 0.f;

	_bool m_bIsStand = false;
	_bool m_bIsDown = false;

public:
	static CCriminal_MonsterNode_Air* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

