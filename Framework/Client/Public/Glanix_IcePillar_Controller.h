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

	const _float& Get_Radius() const { return m_fRadius; }
	const _float& Get_DeathDistnace() const { return m_fDeathDistance; }

	void PillarControllerFree();
	_float Get_DecalSize() { return m_fDecalSize; }

private:
	void Accelerate_PillarsSpeed();
	void Decelerate_PillarsSpeed();

#ifdef _DEBUG
public:
	HRESULT Render();
private:
	HRESULT Ready_DebugDraw();
	HRESULT Render_DebugDraw();
#endif

private:

	_float	m_fRadius = 0.f;

	Vec4 m_vOriginPos;
	CGlanix* m_pGlanix = nullptr;
	vector<CGlanix_IcePillar*> m_pPillars;

	_float m_fAcc = 0.f;
	const _float m_fStopDuration = 4.f;

	const _float m_fDefaultRotSpeed = 0.35f;

	_bool  m_bCollision = false;

	_bool	m_bOnCollider = false;
	_bool	m_bOffCollider = false;

	CModel* m_pGlanixModelCom = nullptr;

	_int	m_iKillCount = 0;

	_float	 m_fDeathDistance = 0.f;


	/* 2번째 패턴 */
	_bool	m_bAcceleration = true;
	const	_float m_fNormalSpeedDuration = 1.5f;

	/* 데칼 */
	class CDecal* m_pWarningDecal = nullptr;
	_float4x4 m_matWarning = {};
	_bool m_bIsSmallerDecal = false;
	_float m_fSmallerDest = 0.f;
	_float m_fSmallerValue = 8.34;
	_float m_fDecalSize = 42.f;

#pragma region Debug Draw 
	BasicEffect* m_pEffect = nullptr;
	BoundingSphere* m_pSphere = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
#pragma endregion
};

END
