#pragma once
#include "Tool.h"

BEGIN(Client)
class CTool_UI final : public CTool
{
private:
	CTool_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_UI() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	class CGameObject* m_pTarget = { nullptr };

public:
	static CTool_UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END