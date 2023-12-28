#pragma once

#include "Animals.h"

BEGIN(Client)

class CWater final : public CDynamicObject
{
private:
	explicit CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CWater(const CWater& rhs);
	virtual ~CWater() = default;

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

public:
	static CWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);


private:
	CShader* m_pWaterShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
private:
	// Water State
	_float m_fWaterTranslation = 0.1f;
	_float m_fReflectRefractSacle = 0.1f;
	_float m_fWaterTranslationSpeed = 0.015f;

	_bool m_bFresnel = false;
public:
	virtual void Free() override;
};

END

