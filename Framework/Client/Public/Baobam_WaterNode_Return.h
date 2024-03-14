#pragma once

#include "Baobam_WaterNode_Base.h"

BEGIN(Client)

class CBaobam_WaterNode_Return final : public CBaobam_WaterNode_Base
{
private:
	CBaobam_WaterNode_Return();
	virtual ~CBaobam_WaterNode_Return() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CBaobam_WaterNode_Return* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

