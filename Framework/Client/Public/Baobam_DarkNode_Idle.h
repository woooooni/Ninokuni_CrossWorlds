#pragma once

#include "Baobam_DarkNode_Base.h"

BEGIN(Client)

class CBaobam_DarkNode_Idle final : public CBaobam_DarkNode_Base
{
private:
	CBaobam_DarkNode_Idle();
	virtual ~CBaobam_DarkNode_Idle() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<wstring> m_vecAnimationName;
	_uint			m_iCurAnimationIndex = 0;

public:
	static CBaobam_DarkNode_Idle* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName);
	virtual void Free() override;
};

END

