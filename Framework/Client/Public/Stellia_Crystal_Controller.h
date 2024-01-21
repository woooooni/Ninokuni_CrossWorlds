#pragma once

#include "Engine_Defines.h"
#include "Base.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CStellia;
class CStellia_Crystal;
class CStellia_Crystal_Destructible;

class CStellia_Crystal_Controller
{
public:
	enum CRYSTAL_TYPE { CRYSTAL_AURA, CRYSTAL_SKY, CRYSTAL_GOLD, CRYSTAL_END };

public:
	CStellia_Crystal_Controller();
	virtual ~CStellia_Crystal_Controller() = default;

public:
	void Tick(const _float fTimeDelta);

public:
	HRESULT Create_Crystals(const _int& iNum, const _float& fRadius, CStellia* pStellia);
	HRESULT Clear_Crystals();

private:
	Vec4 m_vOriginPos;
	CStellia* m_pStellia = nullptr;
	CStellia_Crystal* m_pOriginalCrystal = nullptr;
	vector<CStellia_Crystal_Destructible*> m_pCrystals;

	_int m_iOriginalType = CRYSTAL_END;

	_bool  m_bIsRespawn = false;
	_float m_fTime = 0.f;
	_float m_fRespawnTime = 0.f;
};

END

