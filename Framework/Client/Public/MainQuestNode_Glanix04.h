#pragma once

#include "QuestNode_Base.h"

#include "Glanix.h"

class CMainQuestNode_Glanix04 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Glanix04();
	virtual ~CMainQuestNode_Glanix04() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	void			BossBattle_TalkEvent(const _float& fTimeDelta);
	virtual void    TalkEvent();

private:
	CGameObject* m_pKuu = nullptr;
	CGameObject* m_pGlanix = nullptr;

	_float m_fTalkChangeTime = 3.f;
	_float m_fTime = 0.f;

	_bool m_bIsIntroTalk = false;
	_bool m_bIsRage1Talk = false;
	_bool m_bIsRage2Talk = false;

	_bool m_bIsTalk = false;

public:
	static CMainQuestNode_Glanix04* Create();
	virtual void Free() override;
};

