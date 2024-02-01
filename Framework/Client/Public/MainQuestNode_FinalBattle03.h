#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_FinalBattle03 final : public CQuestNode_Base
{
private:
	CMainQuestNode_FinalBattle03();
	virtual ~CMainQuestNode_FinalBattle03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pWitch = nullptr;
	CGameObject* m_pKuu = nullptr;

	_bool m_bIsWitchDisAppear = false;

public:
	static CMainQuestNode_FinalBattle03* Create();
	virtual void Free() override;
};

