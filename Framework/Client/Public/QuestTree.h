#pragma once

#include "Client_Defines.h"
#include "BTNode.h"
#include "Base.h"

BEGIN(Client)

class CQuestTree abstract : public CBase
{
protected:
	CQuestTree();
	virtual ~CQuestTree() = default;

public:
	virtual HRESULT Initialize();
	virtual _bool   Tick(const _float& fTimeDelta);
	virtual void    LateTick(const _float& fTimeDelta);

public:
	virtual _bool	Get_IsRunningQuest() { return m_bIsRunningQuest; }
	virtual void	Set_IsRunningQuest(_bool bIsRunning) { m_bIsRunningQuest = bIsRunning; }

	virtual _bool	Get_IsClearQuest() { return m_bIsClearQuest; }

protected:
	CBTNode* m_pRootNode = nullptr;
	CBTNode* m_pCurNode = nullptr;

	_int m_iQuestIndex = 0;
	_int m_iCurQuestIndex = 0;

	_bool	m_bIsRunningQuest = true;
	_bool	m_bIsClearQuest = false;

public:
	virtual void Free() override;
};

END

