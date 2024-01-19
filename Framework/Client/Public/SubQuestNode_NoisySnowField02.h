#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_NoisySnowField02 final : public CQuestNode_Base
{
private:
	CSubQuestNode_NoisySnowField02();
	virtual ~CSubQuestNode_NoisySnowField02() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pBella = nullptr;
	CGameObject* m_pKuu = nullptr;

public:
	static CSubQuestNode_NoisySnowField02* Create();
	virtual void Free() override;
};

