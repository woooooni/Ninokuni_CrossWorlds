#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_NoisySnowField03 final : public CQuestNode_Base
{
private:
	CSubQuestNode_NoisySnowField03();
	virtual ~CSubQuestNode_NoisySnowField03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pNoisyMan = nullptr;

public:
	static CSubQuestNode_NoisySnowField03* Create();
	virtual void Free() override;
};

