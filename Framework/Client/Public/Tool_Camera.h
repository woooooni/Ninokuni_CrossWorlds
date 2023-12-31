#pragma once
#include "Tool.h"

#include "Camera.h"

BEGIN(Client)

class CTool_Camera final : public CTool
{
private:
	CTool_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Camera() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Show_Select_Camera();
		 
	void Show_Camera_Prop_Default(CCamera* pCurCam);
	void Show_Camera_Prop_Free(CCamera* pCurCam);
	void Show_Camera_Prop_Follow(CCamera* pCurCam);
	void Show_Camera_Prop_CutScene(_float fTimeDelta);
		 
private: 
	void Clear_CutSceneCache();

private:
	HRESULT Ready_DebugDraw();
	HRESULT Render_DebugDraw();

	/* 베지어 곡선의 경로를 나타내기 위해 사용 */
	vector<Vec3> Subdivide_Bezier(const vector<Vec3>& controlPoints, int numSegments);

private:
	/* CutScene */
	_bool	m_bShow_Prop_CutScene		= false;
	_int	m_iCurCutSceneIndex			= -1;
	_bool	m_bShowMarker				= false;

#pragma region Debug Draw 
	BasicEffect*							m_pEffect = nullptr;
	BoundingSphere*							m_pSphere = nullptr;
	ID3D11InputLayout*						m_pInputLayout = nullptr;
	PrimitiveBatch<VertexPositionColor>*	m_pBatch = nullptr;
#pragma endregion
	
public:
	static CTool_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END