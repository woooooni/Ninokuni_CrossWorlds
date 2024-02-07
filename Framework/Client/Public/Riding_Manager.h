#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CRiding_Manager : public CBase
{	
	DECLARE_SINGLETON(CRiding_Manager)

public:
	enum VEHICLE_TYPE { UDADAK, BIPLANE, VEHICLE_END };

private:
	CRiding_Manager();
	virtual ~CRiding_Manager() = default;

public: // Getter & Setter
	_bool Is_CharacterOnBoard() { return m_bIsRiding; }

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public: // Clone
	HRESULT Ready_Vehicle_GameObject(LEVELID eID);
	HRESULT Ready_Vehicle_GameObjectToLayer(LEVELID eID);

public:
	class CVehicle_Flying_Biplane* Get_Character_Biplane() { return m_pBiplane; }

	_float Get_Character_BiplaneSpeed();
	void Set_Character_BiplaneSpeed(_float fSpeed);

	HRESULT Ride_ForCharacter(VEHICLE_TYPE eType, _bool bOnOff);

private:
	class CVehicle_Udadak* m_pUdadak = { nullptr };
	class CVehicle_Flying_Biplane* m_pBiplane = { nullptr };

	_bool m_bIsRiding = { false }; // 우다닥을 탈때만 제어한다.

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};
END
