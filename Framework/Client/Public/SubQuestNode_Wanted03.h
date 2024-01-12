#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Wanted03 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Wanted03();
	virtual ~CSubQuestNode_Wanted03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CSubQuestNode_Wanted03* Create();
	virtual void Free() override;
};

