#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_LensFlare;
END


BEGIN(Client)
class CLensFlare final : public CGameObject
{
private:
	struct LENSFLARE_DESC
	{
		_int iCats_Shadows = 0;
		_int iUse_Cascades = 0;
		_float fVolumetric_Strength = 0.0f;
		_int iScreen_space_shadow = 0;

		Vec4 vScreen_space_position = Vec4(0.0f,0.0f,0.0f,0.0f);
		Vec4 vPosition = Vec4(0.0f,10.0f,0.0f,1.0f);
		Vec3 vDirection = Vec3(0.0f, -1.0f, 0.0f);
		_float pad = 0.0f;
		Vec3 vColor = Vec3(1.0f, 1.0f, 1.0f);
		_float pad2 = 0.0f;
	};
private:
	explicit CLensFlare(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CLensFlare(const CLensFlare& rhs);
	virtual ~CLensFlare() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	virtual HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_LensFlare* m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;


private:
	LENSFLARE_DESC m_LensDesc = {};
public:
	static CLensFlare* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

