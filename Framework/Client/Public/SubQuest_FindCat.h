#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CSubQuest_FindCat final : public CQuestTree
{
private:
	CSubQuest_FindCat();
	virtual ~CSubQuest_FindCat() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CSubQuest_FindCat* Create();
	virtual void Free() override;
};

END

