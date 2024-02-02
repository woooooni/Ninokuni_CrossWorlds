#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_WitchForest08 final : public CQuestNode_Base
{
private:
	CMainQuestNode_WitchForest08();
	virtual ~CMainQuestNode_WitchForest08() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pKuu = nullptr;

	_float m_fTalkChangeTime = 0.f;
	_float m_fTime = 0.f;

public:
	static CMainQuestNode_WitchForest08* Create();
	virtual void Free() override;
};

#pragma once
class CCMainQuestNode_WitchForeset08
{
};

