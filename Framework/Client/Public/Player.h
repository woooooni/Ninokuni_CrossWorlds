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
	HRESULT Set_Character(CHARACTER_TYPE eType, _bool  bEnterScene);

private:
	class CCharacter* m_pCharacter = nullptr;


private:
	static CPlayer* Create();

public:
	virtual void Free() override;
	friend class CGame_Manager;
};

END
