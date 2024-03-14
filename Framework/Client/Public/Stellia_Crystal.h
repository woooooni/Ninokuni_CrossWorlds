#pragma once

#include "Stellia.h"

BEGIN(Client)

class CStellia_Crystal final : public CGameObject
{
private:
	CStellia_Crystal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CStellia_Crystal(const CStellia_Crystal& rhs);
	virtual ~CStellia_Crystal() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	HRESULT Render_ShadowDepth();

public:
	_int Get_CrystalType() { return m_iCrystalType; }
	void Set_CrystalType(_int iType) { m_iCrystalType = iType; }

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CTexture* m_pDissoveTexture = nullptr;

private:
	_int		m_iCrystalType = 0;

private:
	_bool m_bStartDissolve = false;
	_float4 m_vDissolveColor = _float4(0.427f, 0.894f, 1.f, 1.f);
	_float  m_fDissolveTotal = 10.f;
	_float  m_fDissolveDuration = 5.f;
	_float  m_fDissolveWeight = 0.f;
	_float  m_fDissolveSpeed = 5.f;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();

public:
	static CStellia_Crystal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

