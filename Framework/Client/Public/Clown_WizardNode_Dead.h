#pragma once

#include "Clown_WizardNode_Base.h"

BEGIN(Client)

class CClown_WizardNode_Dead final : public CClown_WizardNode_Base
{
private:
	CClown_WizardNode_Dead();
	virtual ~CClown_WizardNode_Dead() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTime = 0.f;
	_float m_fBlowDeadTime = 0.f;

public:
	static CClown_WizardNode_Dead* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

