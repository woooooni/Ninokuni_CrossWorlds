#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_NoisySnowField05 final : public CQuestNode_Base
{
private:
	CSubQuestNode_NoisySnowField05();
	virtual ~CSubQuestNode_NoisySnowField05() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	_bool			m_bFadeOut = false;

public:
	static CSubQuestNode_NoisySnowField05* Create();
	virtual void Free() override;
};

