#pragma once

#include "DynamicObject.h"

BEGIN(Client)

class CWinterWater final : public CDynamicObject
{
private:
	enum TEX_TYPE { TEX_DIFFUSE, TEX_NORMAL, TEX_MASK1, TEX_MASK2, TEX_END };
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
	CShader* m_pWaterShaderCom = nullptr;
	CTexture* m_pTextureCom[TEX_END] = {};

private:
	_float m_fWaterTranslationSpeed = 0.015f;
	_float m_fTime = 0.0f;
	Vec2 m_vDirection;

public:
	static CWinterWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);

public:
	virtual void Free() override;
};

END

