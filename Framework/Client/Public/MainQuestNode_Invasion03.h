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
	CGameObject* m_pKuu = nullptr;

	_float m_fAccTime = 0.f;
	_float m_fDeleteTime = 0.f;

	_bool m_bIsFadeOut = false;

public:
	static CMainQuestNode_Invasion03* Create();
	virtual void Free() override;
};

