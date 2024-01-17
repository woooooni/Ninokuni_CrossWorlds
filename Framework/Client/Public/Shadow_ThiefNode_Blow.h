#pragma once

#include "Shadow_ThiefNode_Base.h"

BEGIN(Client)

class CShadow_ThiefNode_Blow final : public CShadow_ThiefNode_Base
{
private:
	CShadow_ThiefNode_Blow();
	virtual ~CShadow_ThiefNode_Blow() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTime = 0.f;
	_float m_fBlowTime = 0.f;

	_bool m_bIsStand = false;

public:
	static CShadow_ThiefNode_Blow* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

