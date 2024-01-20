#pragma once

#include "PumpkinCandleNode_Base.h"

BEGIN(Client)

class CPumpkinCandleNode_Air final : public CPumpkinCandleNode_Base
{
private:
	CPumpkinCandleNode_Air();
	virtual ~CPumpkinCandleNode_Air() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTime = 0.f;
	_float m_fAirTime = 0.f;

	_bool m_bIsStand = false;


public:
	static CPumpkinCandleNode_Air* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

