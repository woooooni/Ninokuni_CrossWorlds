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

private:
	_float m_fTime = 0.f;
	_float m_fBlowDeadTime = 0.f;

public:
	static CClownNode_Dead* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

