#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Client)

class CVfx_TargetPoint final : public CVfx
{
protected:
	CVfx_TargetPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_TargetPoint(const CVfx_TargetPoint& rhs);
	virtual ~CVfx_TargetPoint() = default;

public:
	void Set_Active(_bool bActive) { m_bActive = bActive; }
	_bool Get_Active() { return m_bActive; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal* m_pDecalObject = { nullptr };
	_bool m_bActive = { false };

public:
	static CVfx_TargetPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END