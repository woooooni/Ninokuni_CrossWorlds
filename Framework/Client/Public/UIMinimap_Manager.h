#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
END

BEGIN(Client)

class CUIMinimap_Manager : public CBase
{
	DECLARE_SINGLETON(CUIMinimap_Manager)

private:
	CUIMinimap_Manager();
	virtual ~CUIMinimap_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);

public:
	_bool Is_InMinimap(class CTransform* pTransform);
	_float2 Calculate_PositionForMinimap(class CTransform* pTransform);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	CRenderer* m_pRendererCom = { nullptr };

public:
	virtual void Free() override;
};

END