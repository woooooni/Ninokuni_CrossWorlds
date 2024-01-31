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
class CRigidBody;
class CVIBuffer_LensFlare;
END

BEGIN(Client)

class CGrass final : public CGameObject
{
private:
	explicit CGrass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	explicit CGrass(const CGrass& rhs);
	virtual ~CGrass() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components() override;
	HRESULT Bind_ShaderResource();

private:
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

	class CVIBuffer_LensFlare* m_pVIBufferCom = nullptr;
	class CShader* m_pShaderCom = nullptr;

	class CTexture* m_pTextureCom = nullptr;

private:
	_float m_fTessellationAmount = 12.0f;

public:
	static CGrass* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END