#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_FindCat03 final : public CQuestNode_Base
{
private:
	CSubQuestNode_FindCat03();
	virtual ~CSubQuestNode_FindCat03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CSubQuestNode_FindCat03* Create();
	virtual void Free() override;
};

