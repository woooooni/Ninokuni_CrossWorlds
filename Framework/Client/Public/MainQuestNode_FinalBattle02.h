#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_FinalBattle02 final : public CQuestNode_Base
{
private:
	CMainQuestNode_FinalBattle02();
	virtual ~CMainQuestNode_FinalBattle02() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pWitch = nullptr;

public:
	static CMainQuestNode_FinalBattle02* Create();
	virtual void Free() override;
};

