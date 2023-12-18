#pragma once

#include "Shadow_ThiefNode_Base.h"

BEGIN(Client)

class CShadow_ThiefNode_Roaming final : public CShadow_ThiefNode_Base
{
private:
	CShadow_ThiefNode_Roaming();
	virtual ~CShadow_ThiefNode_Roaming() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<_vector> m_vecRoamingArea;
	_uint			m_iRoamingIndex = 0;

	_bool		m_bIsRoaming = true;

public:
	static CShadow_ThiefNode_Roaming* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void Free() override;
};

END

