#pragma once

#include "ClownNode_Base.h"

BEGIN(Client)

class CClownNode_Roaming final : public CClownNode_Base
{
private:
	CClownNode_Roaming();
	virtual ~CClownNode_Roaming() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<Vec4>* vecRoamingArea);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<Vec4> m_vecRoamingArea;
	_uint			m_iRoamingIndex = 0;

	_bool		m_bIsRoaming = true;

public:
	static CClownNode_Roaming* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<Vec4>* vecRoamingArea);
	virtual void Free() override;
};

END

