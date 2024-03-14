#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_NoisySnowField04_2 final : public CQuestNode_Base
{
private:
	CSubQuestNode_NoisySnowField04_2();
	virtual ~CSubQuestNode_NoisySnowField04_2() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pNoisyMan = nullptr;
	CGameObject* m_pKuu = nullptr;

	_bool		m_bFadeOut = false;

public:
	static CSubQuestNode_NoisySnowField04_2* Create();
	virtual void Free() override;
};

