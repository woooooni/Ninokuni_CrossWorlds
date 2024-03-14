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
	enum TEX_TYPE { DIFFUSE_TEX, NOISE_TEX, ALPHA_TEX, TEX_END };
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
	virtual HRESULT Render_Instance(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);

public:
	virtual HRESULT Ready_Components();

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom[TEX_TYPE::TEX_END] = { nullptr, nullptr, nullptr };

private:
	_float m_fTime = 0.0f;
	Vec3 m_vScrollSpeeds = Vec3(1.3f, 2.1f, 2.3f);
	Vec3 m_vScales = Vec3(1.0f, 2.0f, 3.0f);

	Vec2 m_vDistortion1 = Vec2(0.1f, 0.2f);
	Vec2 m_vDistortion2 = Vec2(0.1f, 0.3f);
	Vec2 m_vDistortion3 = Vec2(0.1f, 0.1f);

	_float m_fDistortionScale = 0.8f;
	_float m_fDistortionBias = 0.5f;

public:
	static CPerlinFire* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

