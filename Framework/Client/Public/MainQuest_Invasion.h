#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CMainQuest_Invasion final : public CQuestTree
{
private:
	CMainQuest_Invasion();
	virtual ~CMainQuest_Invasion() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CMainQuest_Invasion* Create();
	virtual void Free() override;
};

END

