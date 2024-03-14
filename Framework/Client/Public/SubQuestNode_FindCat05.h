#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_FindCat05 final : public CQuestNode_Base
{
private:
	CSubQuestNode_FindCat05();
	virtual ~CSubQuestNode_FindCat05() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pChloe = nullptr;
	CGameObject* m_pKuu = nullptr;

public:
	static CSubQuestNode_FindCat05* Create();
	virtual void Free() override;
};

