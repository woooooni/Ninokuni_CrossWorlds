#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Windmill02 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Windmill02();
	virtual ~CSubQuestNode_Windmill02() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pVerde = nullptr;
	CGameObject* m_pKuu = nullptr;

	_bool		m_bCameraAction = false; /* Ç³Â÷ ºñÃçÁÖ±â */

public:
	static CSubQuestNode_Windmill02* Create();
	virtual void Free() override;
};

