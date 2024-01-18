#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Client)

class CTowerDefence_Manager : public CBase
{	

	
	DECLARE_SINGLETON(CTowerDefence_Manager)

public:
	enum TOWER_DEFENCE_PHASE { DEFENCE_PREPARE, DEFENCE_PROGRESS, DEFENCE_FINISH, DEFENCE_END, PHASE_END };

private:
	CTowerDefence_Manager();
	virtual ~CTowerDefence_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	void Prepare_Defence();
	void Start_Defence();
	void Finish_Defence();
	void End_Defence();

private:
	void Tick_Defence_Prepare(_float fTimeDelta);
	void Tick_Defence_Progress(_float fTimeDelta);
	void Tick_Defence_Finish(_float fTimeDelta);

private:
	void LateTick_Defence_Prepare(_float fTimeDelta);
	void LateTick_Defence_Progress(_float fTimeDelta);
	void LateTick_Defence_Finish(_float fTimeDelta);


private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	_bool m_bReserved = false;
	TOWER_DEFENCE_PHASE m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_END;

public:
	virtual void Free() override;
};

END
