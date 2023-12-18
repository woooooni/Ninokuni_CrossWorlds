#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
END

BEGIN(Client)

class CEffects_MouseClick final : public CGameObject
{
protected:
	CEffects_MouseClick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CEffects_MouseClick(const CEffects_MouseClick& rhs);
	virtual ~CEffects_MouseClick() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	_vector m_vPosition = {};

	_uint  m_iCount   = 0;
	_float m_fTimeAcc = 0.f;

public:
	static CEffects_MouseClick* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END