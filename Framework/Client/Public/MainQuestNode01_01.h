#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode01_01 final : public CQuestNode_Base
{
private:
	CMainQuestNode01_01();
	virtual ~CMainQuestNode01_01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	static CMainQuestNode01_01* Create();
	virtual void Free() override;
};

