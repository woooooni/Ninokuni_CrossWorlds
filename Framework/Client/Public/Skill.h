#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CSkill abstract : public CBase
{	
protected:
	CSkill();
	virtual ~CSkill() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, class CGameObject* pOwner);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	_bool  Is_Useable() { return m_bUseable; }
	_float Get_CurrCoolTime() { return m_fAccCoolTime; }
	_float Get_CoolTime() { return m_fCoolTime; }

protected:
	class CGameObject* m_pOwner = nullptr;
	ID3D11Device* m_pDevice = nullptr; 
	ID3D11DeviceContext* m_pContext = nullptr;

protected:
	_bool m_bUseable = true;
	_float m_fAccCoolTime = 2.f;
	_float m_fCoolTime = 1.f;

public:
	virtual void Free() override;
};

END

