#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Wanted04 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Wanted04();
	virtual ~CSubQuestNode_Wanted04() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pCriminal = nullptr;
	CGameObject* m_pKuu = nullptr;

	_float m_fTalkChangeTime = 0.f;
	_float m_fTime = 0.f;

	_bool m_bIsCreateSpot = false;

public:
	static CSubQuestNode_Wanted04* Create();
	virtual void Free() override;
};

