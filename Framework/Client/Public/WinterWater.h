#pragma once

#include "Client_Defines.h"
#include "Water.h"

BEGIN(Client)

class CWinterWater final : public CWater
{
private:
	enum TEX_TYPE { TEX_DIFFUSE, TEX_NOISE1, TEX_NOISE2, TEX_NORMAL, TEX_END };
private:
	explicit CWinterWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CWinterWater(const CWinterWater& rhs);
	virtual ~CWinterWater() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_ModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName);

	virtual HRESULT Bind_ShaderResources();

private:
	class CShader* m_pShaderCom = { nullptr };
	class CTexture* m_pTextureCom[TEX_TYPE::TEX_END] = {};
	class CModel* m_pModelCom = nullptr;
	class CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };

public:
	static CWinterWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);

public:
	virtual void Free() override;
};

END

