#pragma once

#include "Baobam_WaterNode_Base.h"

BEGIN(Client)

class CBaobam_WaterNode_Air final : public CBaobam_WaterNode_Base
{
private:
	CBaobam_WaterNode_Air();
	virtual ~CBaobam_WaterNode_Air() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTime = 0.f;
	_float m_fAirTime = 0.f;

	_bool m_bIsStand = false;


public:
	static CBaobam_WaterNode_Air* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

