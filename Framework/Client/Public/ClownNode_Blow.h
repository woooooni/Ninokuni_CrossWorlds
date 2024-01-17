#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Blow final : public CClownNode_Base
{
private:
	CClownNode_Blow();
	virtual ~CClownNode_Blow() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTime = 0.f;
	_float m_fBlowTime = 0.f;

	_bool m_bIsStand = false;

public:
	static CClownNode_Blow* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

