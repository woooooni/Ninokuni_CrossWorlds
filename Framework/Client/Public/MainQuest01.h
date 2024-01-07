#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CMainQuest01 final : public CQuestTree
{
private:
	CMainQuest01();
	virtual ~CMainQuest01() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);

public:
	virtual void Free() override;
};

END

