#pragma once

#include "Spawner.h"

BEGIN(Client)
class CSpawner_Witch01 final : public CSpawner
{
private:
	CSpawner_Witch01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CSpawner_Witch01(const CSpawner_Witch01& rhs);
	virtual ~CSpawner_Witch01() = default;

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
	_int m_iMaxPumpkinCount = 0;
	_int m_iMaxClownCount = 0;
	_int m_iMaxWizardCount = 0;

	vector<CGameObject*> m_vecPumpkin;
	vector<CGameObject*> m_vecClown;
	vector<CGameObject*> m_vecWizard;

public:
	static CSpawner_Witch01* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

