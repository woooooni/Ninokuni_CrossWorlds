#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Wanted04 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Wanted04();
	virtual ~CSubQuestNode_Wanted04() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CSubQuestNode_Wanted04* Create();
	virtual void Free() override;
};

