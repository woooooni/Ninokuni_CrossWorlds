#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_WitchForest02 final : public CQuestNode_Base
{
private:
	CMainQuestNode_WitchForest02();
	virtual ~CMainQuestNode_WitchForest02() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	_bool		 m_bIsStart = false;
	_bool		 m_bBGMStart = false;
	_bool		 m_bStartCutscene = false;

private:
	CGameObject* m_pKuu = nullptr;

	_float			m_fAcc = 0.f;
	const _float	m_fWaitTime = 1.5f;
	_bool			m_bCanStartCutscene = false;

public:
	static CMainQuestNode_WitchForest02* Create();
	virtual void Free() override;
};

