#pragma once

#include "PumpkinCandleNode_Base.h"

BEGIN(Client)

class CPumpkinCandleNode_Blow final : public CPumpkinCandleNode_Base
{
private:
	CPumpkinCandleNode_Blow();
	virtual ~CPumpkinCandleNode_Blow() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTime = 0.f;
	_float m_fBlowTime = 0.f;

	_bool m_bIsStand = false;

public:
	static CPumpkinCandleNode_Blow* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

