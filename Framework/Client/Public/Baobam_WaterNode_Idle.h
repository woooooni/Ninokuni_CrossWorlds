#pragma once

#include "Baobam_WaterNode_Base.h"

BEGIN(Client)

class CBaobam_WaterNode_Idle final : public CBaobam_WaterNode_Base
{
private:
	CBaobam_WaterNode_Idle();
	virtual ~CBaobam_WaterNode_Idle() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<wstring> m_vecAnimationName;
	_uint			m_iCurAnimationIndex = 0;

public:
	static CBaobam_WaterNode_Idle* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName);
	virtual void Free() override;
};

END

