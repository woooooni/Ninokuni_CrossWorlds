#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Invasion03 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Invasion03();
	virtual ~CMainQuestNode_Invasion03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void	ProgressTalk();
	virtual void    TalkEvent();

private:
	CGameObject* m_pKuu = nullptr;

	_float m_fTalkChangeTime = 0.f;
	_float m_fTime = 0.f;

	_bool m_bIsShowDialog = false;

	_int	m_iPrevKillCount = 0;
	_int	m_iDestKillCount = 7;

public:
	static CMainQuestNode_Invasion03* Create();
	virtual void Free() override;
};

