#pragma once

#include "Clown_WizardNode_Base.h"

BEGIN(Client)

class CClown_WizardNode_Roaming final : public CClown_WizardNode_Base
{
private:
	CClown_WizardNode_Roaming();
	virtual ~CClown_WizardNode_Roaming() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<_vector> m_vecRoamingArea;
	_uint			m_iRoamingIndex = 0;

	_bool		m_bIsRoaming = true;

public:
	static CClown_WizardNode_Roaming* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void Free() override;
};

END

