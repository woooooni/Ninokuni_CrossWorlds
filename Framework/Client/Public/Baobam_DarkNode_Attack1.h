#pragma once

#include "Baobam_DarkNode_Base.h"

BEGIN(Client)

class CBaobam_DarkNode_Attack1 final : public CBaobam_DarkNode_Base
{
private:
	CBaobam_DarkNode_Attack1();
	virtual ~CBaobam_DarkNode_Attack1() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CBaobam_DarkNode_Attack1* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

