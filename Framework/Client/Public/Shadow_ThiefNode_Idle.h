#pragma once

#include "Shadow_ThiefNode_Base.h"

BEGIN(Client)

class CShadow_ThiefNode_Idle final : public CShadow_ThiefNode_Base
{
private:
	CShadow_ThiefNode_Idle();
	virtual ~CShadow_ThiefNode_Idle() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	vector<wstring> m_vecAnimationName;
	_uint			m_iCurAnimationIndex = 0;

public:
	static CShadow_ThiefNode_Idle* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT, vector<wstring> vecAnimationName);
	virtual void Free() override;
};

END

