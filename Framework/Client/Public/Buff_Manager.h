#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Client)

class CBuff_Manager : public CBase
{	
	DECLARE_SINGLETON(CBuff_Manager)

public:
	enum BUFF_TYPE { HP_POTION, DESTROYER_FRENGE_CHARGE, BUFF_TYPE_END };

private:
	CBuff_Manager();
	virtual ~CBuff_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	void Apply_Buff(CHARACTER_TYPE eCharacterType, BUFF_TYPE eBuffType, const BUFF_DESC& BuffDesc);


private:
	void Use_Buff(BUFF_TYPE eBuffType);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;


private:
	map<BUFF_TYPE, BUFF_DESC> m_Buffs[CHARACTER_TYPE::CHARACTER_END];

private:
	_bool m_bReserved = false;

public:
	virtual void Free() override;
};

END
