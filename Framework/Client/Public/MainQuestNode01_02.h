#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode01_02 final : public CQuestNode_Base
{
private:
	CMainQuestNode01_02();
	virtual ~CMainQuestNode01_02() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CMainQuestNode01_02* Create();
	virtual void Free() override;
};

