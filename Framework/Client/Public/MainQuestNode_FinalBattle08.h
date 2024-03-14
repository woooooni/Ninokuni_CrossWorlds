#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_FinalBattle08 final : public CQuestNode_Base
{
private:
	CMainQuestNode_FinalBattle08();
	virtual ~CMainQuestNode_FinalBattle08() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pWitch = nullptr;

	_float m_fTalkChangeTime = 3.f;
	_float m_fTime = 0.f;

	_bool m_bIsTalk = false;
	_bool m_bIsWitchEscape = false;

	_bool m_bFadeOut = false;

	_bool	m_bStartBlend = false;
public:
	static CMainQuestNode_FinalBattle08* Create();
	virtual void Free() override;
};

