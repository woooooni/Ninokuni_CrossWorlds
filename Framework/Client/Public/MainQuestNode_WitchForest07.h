#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_WitchForest07 final : public CQuestNode_Base
{
private:
	CMainQuestNode_WitchForest07();
	virtual ~CMainQuestNode_WitchForest07() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CMainQuestNode_WitchForest07* Create();
	virtual void Free() override;
};

