#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Client)

class CSkill_Manager : public CBase
{	
	DECLARE_SINGLETON(CSkill_Manager)

private:
	CSkill_Manager();
	virtual ~CSkill_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	_bool m_bReserved = false;

public:
	virtual void Free() override;
};

END
