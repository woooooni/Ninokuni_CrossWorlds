#pragma once

#include "QuestNode_Base.h"

BEGIN(Client)

class CMainQuestNode_IntroTour03 final : public CQuestNode_Base
{
private:
	CMainQuestNode_IntroTour03();
	virtual ~CMainQuestNode_IntroTour03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

public:
	static CMainQuestNode_IntroTour03* Create();
	virtual void Free() override;
};

END