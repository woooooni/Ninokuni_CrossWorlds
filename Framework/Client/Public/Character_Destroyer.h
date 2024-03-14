#pragma once
#include "Character.h"

BEGIN(Client)
class CCharacter_Destroyer final : public CCharacter
{

private:
	CCharacter_Destroyer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCharacter_Destroyer(const CCharacter_Destroyer& rhs);
	virtual ~CCharacter_Destroyer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;


public:
	virtual void On_Damaged(const COLLISION_INFO& tInfo) override;

protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

private:
	HRESULT Ready_Sockets();
	HRESULT Ready_Weapon();

private:
	_float m_fAccRadial = 0.f;
	_float m_fRadialTime = 1.f;
	_bool m_bScreenEffect = false;
	

private:
	static CCharacter_Destroyer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;

public:
	virtual void Free() override;
	friend class CCharacter_Manager;
};

END

