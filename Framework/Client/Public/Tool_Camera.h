#pragma once
#include "Tool.h"

#include "Camera.h"

BEGIN(Engine)
class CCamera;
END

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
	void	Show_Select_Camera();

	void	Show_Camera_Prop_Default(CCamera* pCurCam);
	void	Show_Camera_Prop_Free(CCamera* pCurCam);
	void	Show_Camera_Prop_Follow(CCamera* pCurCam);
	void	Show_Camera_Prop_CutScene(_float fTimeDelta);
	
private:
	void	Clear_CutSceneCache();
	Vec3	Calculate_Bezier(_bool bPos);

private:
	HRESULT Ready_DebugDraw();
	HRESULT Render_DebugDraw();

private:

	/* Test */
	vector<Vec3>	m_vTempPositions;
	vector<Vec3>	m_vTempLookAts;
	_int			m_iTempPositionIndex = -1;
	_int			m_iTempLookAtIndex = -1;
	LERP_TIME_DESC	m_tDesc;
	_bool			m_bPlay = false;

	/* CutScene */
	_bool	m_bShow_Prop_CutScene = false;
	_int	m_iCurCutSceneEventIndex = -1;
	string	m_strCurCutSceneEventName = "";


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