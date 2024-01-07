#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CQuest_Manager final : public CBase
{
	DECLARE_SINGLETON(CQuest_Manager)
private:
	CQuest_Manager();
	virtual ~CQuest_Manager() = default;

public:
	HRESULT Reserve_Manager(const wstring & strPatriclePath);
	void	Tick(_float fTimeDelta);

public:

private:

public:
	virtual void Free() override;
};

END

