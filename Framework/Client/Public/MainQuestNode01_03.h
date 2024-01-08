#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode01_03 final : public CQuestNode_Base
{
private:
	CMainQuestNode01_03();
	virtual ~CMainQuestNode01_03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CMainQuestNode01_03* Create();
	virtual void Free() override;
};

