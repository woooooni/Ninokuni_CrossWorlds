#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Glanix07 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Glanix07();
	virtual ~CMainQuestNode_Glanix07() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CMainQuestNode_Glanix07* Create();
	virtual void Free() override;
};

