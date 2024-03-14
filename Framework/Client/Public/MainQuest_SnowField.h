#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CMainQuest_SnowField final : public CQuestTree
{
private:
	CMainQuest_SnowField();
	virtual ~CMainQuest_SnowField() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CMainQuest_SnowField* Create();
	virtual void Free() override;
};

END

