#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_PlantKiller04 final : public CQuestNode_Base
{
private:
	CMainQuestNode_PlantKiller04();
	virtual ~CMainQuestNode_PlantKiller04() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pRuby = nullptr;
	CGameObject* m_pKuu = nullptr;

	_bool m_bIsFadeOut = false;

public:
	static CMainQuestNode_PlantKiller04* Create();
	virtual void Free() override;
};

