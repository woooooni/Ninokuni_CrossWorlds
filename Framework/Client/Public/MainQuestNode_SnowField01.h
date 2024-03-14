#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_SnowField01 final : public CQuestNode_Base
{
private:
	CMainQuestNode_SnowField01();
	virtual ~CMainQuestNode_SnowField01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CMainQuestNode_SnowField01* Create();
	virtual void Free() override;
};

