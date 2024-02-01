#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_PlantKiller07 final : public CQuestNode_Base
{
private:
	CMainQuestNode_PlantKiller07();
	virtual ~CMainQuestNode_PlantKiller07() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CMainQuestNode_PlantKiller07* Create();
	virtual void Free() override;
};

