#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Air final : public CClownNode_Base
{
private:
	CClownNode_Air();
	virtual ~CClownNode_Air() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTime = 0.f;
	_float m_fAirTime = 0.f;

	_bool m_bIsStand = false;


public:
	static CClownNode_Air* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

