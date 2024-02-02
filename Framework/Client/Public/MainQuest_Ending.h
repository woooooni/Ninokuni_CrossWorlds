#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CMainQuest_Ending final : public CQuestTree
{
private:
	CMainQuest_Ending();
	virtual ~CMainQuest_Ending() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CMainQuest_Ending* Create();
	virtual void Free() override;
};

END

