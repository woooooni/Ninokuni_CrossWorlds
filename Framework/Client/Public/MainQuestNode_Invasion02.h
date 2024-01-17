#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Invasion02 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Invasion02();
	virtual ~CMainQuestNode_Invasion02() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pKuu = nullptr;

	_bool		m_bIsStart = false;

public:
	static CMainQuestNode_Invasion02* Create();
	virtual void Free() override;
};

