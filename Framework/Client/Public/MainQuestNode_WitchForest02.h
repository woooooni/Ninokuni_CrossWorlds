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
	_bool		 m_bIsStart = false;
	_bool		 m_bBGMStart = false;

public:
	static CMainQuestNode_WitchForest02* Create();
	virtual void Free() override;
};

