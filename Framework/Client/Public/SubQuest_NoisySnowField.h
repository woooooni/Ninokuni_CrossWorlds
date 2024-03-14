#pragma once

#include "QuestTree.h"

BEGIN(Client)

class CSubQuest_NoisySnowField final : public CQuestTree
{
private:
	CSubQuest_NoisySnowField();
	virtual ~CSubQuest_NoisySnowField() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	static CSubQuest_NoisySnowField* Create();
	virtual void Free() override;
};

END

