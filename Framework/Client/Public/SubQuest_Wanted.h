#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CSubQuest_Wanted final : public CQuestTree
{
private:
	CSubQuest_Wanted();
	virtual ~CSubQuest_Wanted() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CSubQuest_Wanted* Create();
	virtual void Free() override;
};

END

