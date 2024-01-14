#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Wanted08 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Wanted08();
	virtual ~CSubQuestNode_Wanted08() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pKuu = nullptr;

	_float m_fTalkChangeTime = 0.f;
	_float m_fTime = 0.f;

public:
	static CSubQuestNode_Wanted08* Create();
	virtual void Free() override;
};

