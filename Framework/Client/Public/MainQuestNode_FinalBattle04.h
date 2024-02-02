#pragma once

#include "QuestNode_Base.h"

#include "Glanix.h"

class CMainQuestNode_FinalBattle04 final : public CQuestNode_Base
{
private:
	CMainQuestNode_FinalBattle04();
	virtual ~CMainQuestNode_FinalBattle04() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pStellia = nullptr;

public:
	static CMainQuestNode_FinalBattle04* Create();
	virtual void Free() override;
};

