#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Camera.h"


BEGIN(Client)

class CGame_Manager : public CBase
{	
	DECLARE_SINGLETON(CGame_Manager)

private:
	CGame_Manager();
	virtual ~CGame_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	const wstring& Get_UserName() { return m_strUserName; }
	void Set_UserName(const wstring& strUserName) { m_strUserName = strUserName; }

public:
	class CPlayer* Get_Player() { return m_pPlayer; }
	class CKuu*	   Get_Kuu() { return m_pKuu; }

public:
	_float Calculate_Elemental(ELEMENTAL_TYPE eAttackerElemental, ELEMENTAL_TYPE eHitElemental)
	{
		switch (eAttackerElemental)
		{
		case ELEMENTAL_TYPE::FIRE :
			if (eHitElemental == ELEMENTAL_TYPE::WOOD)
				return 1.2f;
			else if (eHitElemental == ELEMENTAL_TYPE::FIRE)
				return 1.f;
			else if (eHitElemental == ELEMENTAL_TYPE::WATER)
				return 0.8f;
			else
				return 0;
			break;
		case ELEMENTAL_TYPE::WATER:
			if (eHitElemental == ELEMENTAL_TYPE::FIRE)
				return 1.2f;
			else if (eHitElemental == ELEMENTAL_TYPE::WATER)
				return 1.f;
			else if (eHitElemental == ELEMENTAL_TYPE::WOOD)
				return 0.8f;
			else
				return 0;

			break;
		case ELEMENTAL_TYPE::WOOD:
			if (eHitElemental == ELEMENTAL_TYPE::WATER)
				return 1.2f;
			else if (eHitElemental == ELEMENTAL_TYPE::WOOD)
				return 1.f;
			else if (eHitElemental == ELEMENTAL_TYPE::FIRE)
				return 0.8f;
			else
				return 1.f;
			break;
		}

		return 1.f;
	}
	void Set_KuuTarget_Player();

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	class CPlayer* m_pPlayer = nullptr;
	class CKuu*	   m_pKuu = nullptr;

private:
	wstring m_strUserName = L"";
	_bool m_bReserved = false;

public:
	virtual void Free() override;
};

END
