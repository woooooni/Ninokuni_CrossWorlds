#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CCurlingGame_Manager : public CBase
{
	DECLARE_SINGLETON(CCurlingGame_Manager)

private:
	CCurlingGame_Manager();
	virtual ~CCurlingGame_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(const _float& fTimeDelta);
	void LateTick(const _float& fTimeDelta);

public:
	HRESULT Set_Game(const _bool& bStart);

private:
	HRESULT Ready_Objects();

	void Test(const _float& fTimeDelta);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	_bool m_bReserved = false;
	_bool m_bPlaying = false;

public:
	virtual void Free() override;
};

END
