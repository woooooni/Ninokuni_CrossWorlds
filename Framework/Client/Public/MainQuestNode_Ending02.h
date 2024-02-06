#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Ending02 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Ending02();
	virtual ~CMainQuestNode_Ending02() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pKuu = nullptr;
	_bool		 m_bIsTalkStart = false;

	_bool		m_bFadeOut = false;
	_bool		m_bFadeIn = false;

	const _float m_fFadeDuration = 1.f;
	_bool		m_bFinishIntro = false;

public:
	static CMainQuestNode_Ending02* Create();
	virtual void Free() override;
};

