#pragma once
#include "Character.h"

BEGIN(Client)
class CCharacter_SwordMan final : public CCharacter
{

private:
	CCharacter_SwordMan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCharacter_SwordMan(const CCharacter_SwordMan& rhs);
	virtual ~CCharacter_SwordMan() = default;

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
	HRESULT Ready_Parts();
	

private:
	static CCharacter_SwordMan* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;

public:
	virtual void Free() override;
	friend class CCharacter_Manager;
};

END

