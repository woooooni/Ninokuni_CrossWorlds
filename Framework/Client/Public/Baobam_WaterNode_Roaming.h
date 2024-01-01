#pragma once

#include "Baobam_WaterNode_Base.h"

BEGIN(Client)

class CBaobam_WaterNode_Roaming final : public CBaobam_WaterNode_Base
{
private:
	CBaobam_WaterNode_Roaming();
	virtual ~CBaobam_WaterNode_Roaming() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<_vector>* vecRoamingArea);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<_vector> m_vecRoamingArea;
	_uint			m_iRoamingIndex = 0;

	_bool		m_bIsRoaming = true;

public:
	static CBaobam_WaterNode_Roaming* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<_vector>* vecRoamingArea);
	virtual void Free() override;
};

END

