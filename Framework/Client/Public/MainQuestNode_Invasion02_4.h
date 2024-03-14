#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Invasion02_4 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Invasion02_4();
	virtual ~CMainQuestNode_Invasion02_4() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pWitch = nullptr;
	CGameObject* m_pKuu = nullptr;

	_bool m_bIsWitchAppear = false;

	_float	m_fWitchOriginHeight = 0.f;
	_float	m_fWitchDestHeight = 12.f;

public:
	static CMainQuestNode_Invasion02_4* Create();
	virtual void Free() override;
};

