#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CMainQuest_IntroTour final : public CQuestTree
{
private:
	CMainQuest_IntroTour();
	virtual ~CMainQuest_IntroTour() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CMainQuest_IntroTour* Create();
	virtual void Free() override;
};

END

