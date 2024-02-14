#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CPipeLine;
class CTransform;
class CVIBuffer_SkyPlane;
END


BEGIN(Client)
class CSkyPlane final : public CGameObject
{
private:
	enum PLANE_TEX { CLOUD_TEX, CLOUDE_NOISE, CLOUDE_NOISE2, CLOUDE_PERT, CLOUDE_AURA, TEX_END };
protected:
	explicit CSkyPlane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CSkyPlane(const CSkyPlane& rhs);
	virtual ~CSkyPlane() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	virtual HRESULT Ready_Components();

private:
	HRESULT SkyPlaneRSCreate();
private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CTexture* m_pTextureCom[PLANE_TEX::TEX_END] = {};
	CVIBuffer_SkyPlane* m_pSkyPlaneCom = nullptr;

	_float m_fBrightness = 0.5f;
	_float m_fScale = 0.2f;
	_float m_fTranslation = 0.0f;

private:
	ID3D11RasterizerState* m_pRasterStateNoCulling = nullptr;
public:
	static CSkyPlane* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

