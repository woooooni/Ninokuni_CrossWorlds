#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Windmill07 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Windmill07();
	virtual ~CSubQuestNode_Windmill07() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pBeard = nullptr;
	CGameObject* m_pKuu = nullptr;

public:
	static CSubQuestNode_Windmill07* Create();
	virtual void Free() override;
};

