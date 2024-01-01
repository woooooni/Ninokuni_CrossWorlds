#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx abstract : public CGameObject
{
protected:
	CVfx(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx(const CVfx& rhs);
	virtual ~CVfx() = default;

public:
	void Set_Owner(CGameObject* pGameObject) { m_pOwnerObject = pGameObject; }
	void Set_WorldMatrix(_matrix WorldMatrix) { XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix); }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

protected:
	_float4x4 m_WorldMatrix;

	_uint  m_iCount   = 0;
	_float m_fTimeAcc = 0.f;

protected:
	class CGameObject* m_pOwnerObject = nullptr;

public:
	virtual void Free() override;
};

END