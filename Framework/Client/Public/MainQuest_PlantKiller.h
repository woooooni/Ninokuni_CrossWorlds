#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CMainQuest_PlantKiller final : public CQuestTree
{
private:
	CMainQuest_PlantKiller();
	virtual ~CMainQuest_PlantKiller() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CMainQuest_PlantKiller* Create();
	virtual void Free() override;
};

END

