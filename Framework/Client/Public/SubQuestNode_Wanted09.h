#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Wanted09 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Wanted09();
	virtual ~CSubQuestNode_Wanted09() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pTumba = nullptr;
	CGameObject* m_pKuu = nullptr;

public:
	static CSubQuestNode_Wanted09* Create();
	virtual void Free() override;
};

