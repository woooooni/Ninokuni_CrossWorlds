#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CTransform;
class CVIBuffer_Cube;
class CTexture;
END

BEGIN(Client)

class CDecal final : public CGameObject
{
public:
	typedef struct tagDecalDesc
	{	
		_int    iTextureIndexDiffuse = 23;
		_int    iTextureIndexAlpha   = 23;

		_uint   iShaderPass = 1;
		_float  fAlpha_Discard = 0.5f;
		_float3 fBlack_Discard = _float3(0.5f, 0.5f, 0.5f);

	} DECAL_DESC;

protected:
	CDecal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CDecal(const CDecal& rhs);
	virtual ~CDecal() = default;

public:
	virtual HRESULT Initialize_Prototype(const DECAL_DESC* pDecalDesc, const wstring& strDecalFilePath);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	const DECAL_DESC& Get_DecalDesc() { return m_tDecalDesc; }
	void Set_DecalDesc(const DECAL_DESC& tDesc);
 
	class CTransform* Get_TransformCom() { return m_pTransformCom; }

private:
	_bool m_isCloned = { false };
	DECAL_DESC m_tDecalDesc;

private:
	class CRenderer*  m_pRendererCom  = nullptr;
	class CShader*    m_pShaderCom    = nullptr;
	class CTransform*     m_pTransformCom = nullptr;
	class CVIBuffer_Cube* m_pVIBufferCom = nullptr;
	class CTexture* m_pDiffuseTextureCom = nullptr;
	class CTexture* m_pAlphaTextureCom   = nullptr;

private:
	HRESULT Bind_ShaderResource();

protected:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Components() override;

public:
	static CDecal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag, const DECAL_DESC* pDecalDesc = nullptr, const wstring& strDecalFilePath = {});
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END