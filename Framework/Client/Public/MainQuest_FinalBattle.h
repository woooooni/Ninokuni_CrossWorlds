#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CMainQuest_FinalBattle final : public CQuestTree
{
private:
	CMainQuest_FinalBattle();
	virtual ~CMainQuest_FinalBattle() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CMainQuest_FinalBattle* Create();
	virtual void Free() override;
};

END

