#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_NoisySnowField04 final : public CQuestNode_Base
{
private:
	CSubQuestNode_NoisySnowField04();
	virtual ~CSubQuestNode_NoisySnowField04() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pNoisyMan = nullptr;
	CGameObject* m_pKuu = nullptr;

	_bool       m_bIsGameReady = false;
	_bool		m_bCameraAction = false;

public:
	static CSubQuestNode_NoisySnowField04* Create();
	virtual void Free() override;
};

