#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CTransform;
END


BEGIN(Client)

class CWitch_Barricade : public CMonster
{
private:
	explicit CWitch_Barricade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& eStat);
	explicit CWitch_Barricade(const CWitch_Barricade& rhs);
	virtual ~CWitch_Barricade() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:

public:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_Components() { return S_OK; };
	virtual HRESULT Ready_Colliders();
	virtual HRESULT Ready_States();
private:
	_bool m_bSunDisappear = false;

public:
	static CWitch_Barricade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& eStat);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

