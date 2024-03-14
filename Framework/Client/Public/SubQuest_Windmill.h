#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CSubQuest_Windmill final : public CQuestTree
{
private:
	CSubQuest_Windmill();
	virtual ~CSubQuest_Windmill() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CSubQuest_Windmill* Create();
	virtual void Free() override;
};

END

