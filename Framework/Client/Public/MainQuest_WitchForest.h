#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CMainQuest_WitchForest final : public CQuestTree
{
private:
	CMainQuest_WitchForest();
	virtual ~CMainQuest_WitchForest() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CMainQuest_WitchForest* Create();
	virtual void Free() override;
};

END

