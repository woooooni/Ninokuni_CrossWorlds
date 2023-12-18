#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Loading_Logo final : public CUI
{
protected:
	CUI_Loading_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Loading_Logo(const CUI_Loading_Logo& rhs);
	virtual ~CUI_Loading_Logo() = default;

public:
	void Set_TextureIndex(_int iIndex) { m_iTextureIndex = iIndex; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Ready_Components() override;
	HRESULT Ready_State();
	HRESULT Bind_ShaderResources();

private:
	_int m_iTextureIndex = { -1 };
	_float m_fTimeAcc = { 0.f };

public:
	static CUI_Loading_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END