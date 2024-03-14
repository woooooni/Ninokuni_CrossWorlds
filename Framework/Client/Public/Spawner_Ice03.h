#pragma once

#include "Spawner.h"

BEGIN(Client)
class CSpawner_Ice03 final : public CSpawner
{
private:
	CSpawner_Ice03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CSpawner_Ice03(const CSpawner_Ice03& rhs);
	virtual ~CSpawner_Ice03() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders(); // 피킹 시 확인하기 위함.
	virtual HRESULT Spawn_Monster();

private:
	_int m_iMaxThiefCount = 0;
	_int m_iMaxBearCount = 0;

	vector<CGameObject*> m_vecThief;
	vector<CGameObject*> m_vecBear;

public:
	static CSpawner_Ice03* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

