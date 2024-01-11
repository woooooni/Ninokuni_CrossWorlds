#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Windmill10 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Windmill10();
	virtual ~CSubQuestNode_Windmill10() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CSubQuestNode_Windmill10* Create();
	virtual void Free() override;
};

