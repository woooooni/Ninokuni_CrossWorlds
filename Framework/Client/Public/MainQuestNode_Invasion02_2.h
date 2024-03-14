#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Invasion02_2 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Invasion02_2();
	virtual ~CMainQuestNode_Invasion02_2() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CMainQuestNode_Invasion02_2* Create();
	virtual void Free() override;
};

