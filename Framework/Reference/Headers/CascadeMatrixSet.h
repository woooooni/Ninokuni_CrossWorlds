#pragma once

#include "Base.h"

BEGIN(Engine)

class CCascadeMatrixSet final : public CBase
{
public:
	CCascadeMatrixSet();
	virtual ~CCascadeMatrixSet() = default;

public:
	void Init(int iShadowMapSize);
	void Tick(const Vec3& vDirectionalDir);


	static const int m_iTotalCascades = 3;
public:

private:
	void ExtractFrustumPoints(_float fNear, _float fFar, Vec3* arrFrustumCorners);
	void ExtractFrustumBoundSphere(_float fNear, _float fFar, Vec3& vBoundCenter, _float& fBoundRadius);
	bool CascadeNeedsUpdate(const Matrix& mShadowView, _int iCascadeIdx, const Vec3& newCenter, Vec3& vOffset);

private:
	_int m_iShadowMapSize = 0;
	_float m_fCascadeTotalRange = 0.0f;
	_float m_arrCascadeRanges[4] = {};

	Vec3 m_vShadowBoundCenter;
	_float m_fShadowBoundRadius = 0.0f;
	Vec3 m_arrCascadeBoundCenter[m_iTotalCascades];
	_float m_arrCascadeBoundRadius[m_iTotalCascades] = {};

	Matrix m_WorldToShadowSpace;
	Matrix m_arrWorldToCascadeProj[m_iTotalCascades];

	Vec3 m_vToCascadeOffsetX = ::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	Vec3 m_vToCascadeOffsetY = ::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	Vec3 m_vToCascadeScale = ::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	_bool m_bAntiFlickerOn = true;
public:
	virtual void Free() override;
};

END

