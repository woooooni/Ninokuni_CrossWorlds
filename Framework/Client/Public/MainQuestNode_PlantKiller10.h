#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_PlantKiller10 final : public CQuestNode_Base
{
private:
	CMainQuestNode_PlantKiller10();
	virtual ~CMainQuestNode_PlantKiller10() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pCyan = nullptr;
	CGameObject* m_pKuu = nullptr;

	_bool m_bIsFadeOut = false;
	_bool m_bIsFadeIn = false;

public:
	static CMainQuestNode_PlantKiller10* Create();
	virtual void Free() override;
};

