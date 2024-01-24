#pragma once

#include "Base.h"

BEGIN(Engine)
/* 빛 정볼르 보관한다. */

class CLight final : public CBase
{
private:
	CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight() = default;

public:
	const LIGHTDESC* Get_LightDesc() const {
		return &m_LightDesc;
	}


	LIGHTDESC* Get_ModifyLightDesc() { return &m_LightDesc; }
	_uint Get_LightID() { return m_iLightID; }

public:
	void Set_LightID(_uint iID) { m_iLightID = iID; }

public:
	HRESULT Initialize(const LIGHTDESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

public:
	HRESULT DirectionalLight(class CShader* pShader);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

private:
	LIGHTDESC				m_LightDesc;
	_uint m_iLightID = 0;
public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	virtual void Free() override;
};

END