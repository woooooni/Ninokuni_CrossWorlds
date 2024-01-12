#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Wanted06 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Wanted06();
	virtual ~CSubQuestNode_Wanted06() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pTumba = nullptr;

public:
	static CSubQuestNode_Wanted06* Create();
	virtual void Free() override;
};

