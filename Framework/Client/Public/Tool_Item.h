#pragma once

#include "Tool.h"
#include "GameItem_Weapon.h"

BEGIN(Client)

class CTool_Item final : public CTool
{
public:
	typedef struct tagToolWeaponDesc
	{
		string strName = "";
		int eCode = CODE_END;
		string strContent = "";
		int eMainCategory = CGameItem_Weapon::CATEGORY_END;
		int eWeaponCategory = CGameItem_Weapon::WEAPON_END;
		_float	fAtk = 0.f;
	}TOOLDESC_WEAPON;

private:
	CTool_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Item() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	void	Show_Button();
	void	Show_Desc();

	void	Show_Weapon();
	void	Show_Armor();

private:
	/* Weapon, Armor */
	_bool	m_bShowWeapon = true;
	_bool	m_bShowArmor = false;

	/* Desc */
	char szFilePath[MAX_PATH] = "";
	char strName[MAX_PATH] = "";
	char strContent[MAX_PATH] = "";


	TOOLDESC_WEAPON m_tWeaponDesc = {};

	/* enum */
	int m_iCodes[CODE_END];

public:
	static CTool_Item* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END

