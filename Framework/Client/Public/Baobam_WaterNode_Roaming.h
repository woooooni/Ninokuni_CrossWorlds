#pragma once

#include "Baobam_WaterNode_Base.h"

BEGIN(Client)

class CBaobam_WaterNode_Roaming final : public CBaobam_WaterNode_Base
{
private:
	CBaobam_WaterNode_Roaming();
	virtual ~CBaobam_WaterNode_Roaming() = default;

public:
	virtual HRESULT Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<_vector> m_vecRoamingArea;
	_uint			m_iRoamingIndex = 0;

	_bool		m_bIsRoaming = true;

public:
	static CBaobam_WaterNode_Roaming* Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT, vector<_vector>* vecRoamingArea);
	virtual void Free() override;
};

END

