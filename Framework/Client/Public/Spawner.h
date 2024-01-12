#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CCollider;
END

BEGIN(Client)
class CSpawner abstract : public CGameObject
{
protected:
	CSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CSpawner(const CSpawner& rhs);
	virtual ~CSpawner() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Ready_Components() PURE;
	virtual HRESULT Ready_Colliders() PURE; // 피킹 시 확인하기 위함.
	virtual HRESULT Spawn_Monster() { return S_OK; }

protected:
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

	_float m_fCurTime = 0.f;
	_float m_fSpawnTime = 0.f;

public:
	virtual void Free() override;
};
END

