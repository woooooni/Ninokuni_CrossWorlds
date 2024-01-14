#pragma once

#include "Engine_Defines.h"
#include "Base.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CGlanix;
class CGlanix_IcePillar;

class CGlanix_IcePillar_Controller
{
public:
	CGlanix_IcePillar_Controller();
	virtual ~CGlanix_IcePillar_Controller();

public:
	void Tick(const _float fTimeDelta);

public:
	HRESULT Create_Pillars(const _int& iNum, const _float& fRadius, const Vec4& vOriginPos, CGlanix* pGlanix);
	HRESULT Delete_Pillar(const _int iIndex);
	HRESULT Clear_Pillars();

private:
	void Accelerate_PillarsSpeed();
	void Decelerate_PillarsSpeed();

private:
	Vec4 m_vOriginPos;
	CGlanix* m_pGlanix = nullptr;
	vector<CGlanix_IcePillar*> m_pPillars;

	_float m_fAcc = 0.f;
	const _float m_fStopDuration = 4.f;
	const _float m_fLerptime = 1.f;

	const _float m_fDefaultRotSpeed = 0.35f;

	_bool  m_bCollision = false;

	_bool	m_bOnCollider = false;
	_bool	m_bOffCollider = false;

	CModel* m_pGlanixModelCom = nullptr;

	_int		m_iKillCount = 0;
};

END
