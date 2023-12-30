#pragma once

#include "Client_Defines.h"
#include "Base.h"


/*
* [TW] 모든 무기 모델들을 가지고 있습니다.
*/
BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CWeapon;
class CWeapon_Manager : public CBase
{	
	DECLARE_SINGLETON(CWeapon_Manager)

private:
	CWeapon_Manager();
	virtual ~CWeapon_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	class CModel* Get_WeaponModel(CHARACTER_TYPE eType, const wstring& strWeaponModelName);


private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	vector<class CModel*> m_pWeaponModels[CHARACTER_TYPE::CHARACTER_END];

private:
	_bool m_bReserved = false;
	

private:
	HRESULT Loading_Weapon_Models(const wstring& strFolderPath);


public:
	virtual void Free() override;
};

END
