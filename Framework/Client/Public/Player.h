#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Character.h"
/*
* [TW] 캐릭터 하나를 가지고 있습니다.
*/


BEGIN(Client)

class CPlayer final : public CBase
{	
private:
	CPlayer();
	virtual ~CPlayer() = default;

public:
	HRESULT Initialize(void* pArg);

public:
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	class CCharacter* Get_Character() { return m_pCharacter; }
	HRESULT Set_Character(CHARACTER_TYPE eType, Vec4 vEnterPosition, _bool bEnterScene);
	HRESULT Tag_Character(CHARACTER_TYPE eType);
	HRESULT Set_Character_Initial_Position(Vec4 vEnterPosition, _bool bEnterDoor = false);

public:
	void Increase_Gold(_int iGold) { m_iGold += iGold; }
	_bool Decrease_Gold(_int iGold) { 
		if (0 > m_iGold - iGold)
			return false;

		m_iGold = max(0, m_iGold - iGold);
		return true;
	}

private:
	class CCharacter* m_pCharacter = nullptr;

private:
	_int m_iGold = 0;

private:
	static CPlayer* Create();

public:
	virtual void Free() override;
	friend class CGame_Manager;
};

END
