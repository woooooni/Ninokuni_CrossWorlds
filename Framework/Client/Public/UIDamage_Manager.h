#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CUIDamage_Manager : public CBase
{
	DECLARE_SINGLETON(CUIDamage_Manager)

public:
	enum UI_DAMAGEFONTCOLOR { FONT_BLUE, // Weakness
		FONT_WHITE, FONT_WHITE_AND_GOLD, // General
		FONT_GOLD, FONT_GOLD_AND_RED, // strong
		FONT_DAMAGECOLOR_END };

private:
	CUIDamage_Manager();
	virtual ~CUIDamage_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);

public:
	HRESULT Ready_DamageNumberPrototypes();

public:
	_int Calculate_DamageColor(ELEMENTAL_TYPE eMonsterType, ELEMENTAL_TYPE eWeaponType, _int iDamage);

public:
	HRESULT Create_PlayerDamageNumber(class CTransform* pTransformCom, _int iDamage); // ColorRed

public:
	HRESULT Create_MonsterDamageNumber(class CTransform* pTransformCom, UI_DAMAGEFONTCOLOR eType, _int iDamage);
	//HRESULT Create_CommonDamageNumber(class CTransform* pTransformCom, _int iDamage);
	HRESULT Create_Miss(class CTransform* pTransformCom);
	HRESULT Create_Critical(_uint eDamageFontType, _float2 vPosition);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	_uint m_iStandard = { 100000 };
	_uint m_iMaxStandard = { 500000 };

public:
	virtual void Free() override;
};
END