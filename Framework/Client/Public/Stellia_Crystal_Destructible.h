#pragma once

#include "Monster.h"

BEGIN(Client)

class CStellia;
class CStellia_Crystal_Controller;

class CStellia_Crystal_Destructible final : public CMonster
{
public:
	typedef struct tagCrystalDesc
	{
		CGameObject* pStellia = nullptr;
		_int		 iBingoType;
		_int		 iSelfType;
	}STELLIA_CRYSTAL_DESC;

private:
	CStellia_Crystal_Destructible(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	CStellia_Crystal_Destructible(const CStellia_Crystal_Destructible& rhs);
	virtual ~CStellia_Crystal_Destructible() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	HRESULT Render_ShadowDepth();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	void Set_CrystalData(STELLIA_CRYSTAL_DESC* pDesc);

private:
	class CTexture* m_pTextureCom = nullptr;
	class CUI_MonsterHP_World* m_pHPBar = { nullptr };

private:
	_int		m_iBingoType;
	_int		m_iSelfType = 0;
	CStellia*	m_pStellia = nullptr;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Colliders();

public:
	static CStellia_Crystal_Destructible* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

