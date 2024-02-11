#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class CTexture;
END


BEGIN(Client)

class CPerlinFire : public CGameObject
{
private:
	enum TEX_TYPE { JITTER_TEX, PERM_TEX, TEX_END };
private:
	explicit CPerlinFire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CPerlinFire(const CPerlinFire& rhs);
	virtual ~CPerlinFire() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Ready_Components();
	virtual HRESULT Prepare_Texture();

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;

	ID3D11Texture2D* m_pNoiseTexture = nullptr;
	ID3D11ShaderResourceView* m_pSRV[TEX_TYPE::TEX_END] = { nullptr, nullptr };

private:
	_bool m_bJitter = true;
	_int m_iSampleingRate = 16;
	_float m_fSpeed = 0.6f;
	_float m_fNoiseScale = 1.35f;
	_float m_fRoughness = 3.20f;
	_float m_fShapeSize = 3.0f;
	_float m_fFrequencyWeights[5] = { 1.0f, 0.5f, 0.25f, 0.125f, 0.0625f };
	_float m_fTime = 0.0f;
public:
	static CPerlinFire* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

