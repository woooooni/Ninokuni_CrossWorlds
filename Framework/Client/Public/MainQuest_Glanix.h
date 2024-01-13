#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CMainQuest_Glanix final : public CQuestTree
{
private:
	CMainQuest_Glanix();
	virtual ~CMainQuest_Glanix() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CMainQuest_Glanix* Create();
	virtual void Free() override;
};

END

