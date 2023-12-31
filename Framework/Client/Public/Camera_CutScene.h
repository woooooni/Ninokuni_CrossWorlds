#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_CutScene final : public CCamera
{
private:
	CCamera_CutScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_CutScene(const CCamera_CutScene& rhs);
	virtual ~CCamera_CutScene() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	/* Tool */
	HRESULT Add_CutSceneDesc(string strCutSceneName);
	HRESULT Del_CutSceneDesc(string strCutSceneName);
	const _bool Has_CutSceneDesc(string strCutSceneName);
	HRESULT Change_CutSceneName(string strOriginName, string strChangedName);

	HRESULT Add_PathDesc(string strCutSceneName);
	HRESULT Del_PathDesc(string strCutSceneName, const _uint iIndex);
	
	/* Debug */
	HRESULT Start_CutScene(string strCutSceneName);
	Vec3 Get_Bezier_CamPosition(string strCutSceneName);
	Vec3 Get_Bezier_CamLookAt(string strCutSceneName);

	/* Access */
	CAMERA_CUTSCENE_DESC Get_CurCutSceneDesc() const { return m_CurrCutSceneDesc; }
	map<string, CAMERA_CUTSCENE_DESC> Get_CutSceneDescs() const { return m_CutSceneDescs; }

private:
	virtual HRESULT Ready_Components() override;

private:
	Vec3 Calculate_Bezier_Position(Vec3 vPoint[MAX_BEZIER_POINT], const _float& fRatio);

private:
	/* 모든 컷신 구조체 */
	map<string, CAMERA_CUTSCENE_DESC> m_CutSceneDescs;

	/* 현재 컷신 구조체 */
	CAMERA_CUTSCENE_DESC	m_CurrCutSceneDesc;


public:
	static CCamera_CutScene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END