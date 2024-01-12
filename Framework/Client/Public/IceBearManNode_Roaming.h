#pragma once

#include "IceBearManNode_Base.h"

BEGIN(Client)

class CIceBearManNode_Roaming final : public CIceBearManNode_Base
{
private:
	CIceBearManNode_Roaming();
	virtual ~CIceBearManNode_Roaming() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<Vec4>* vecRoamingArea);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<Vec4> m_vecRoamingArea;
	_uint			m_iRoamingIndex = 0;

	_bool		m_bIsRoaming = true;

public:
	static CIceBearManNode_Roaming* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<Vec4>* vecRoamingArea);
	virtual void Free() override;
};

END

