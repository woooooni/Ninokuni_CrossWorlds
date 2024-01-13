#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CSwordMan_AuraBlade final : public CGameObject
{
	
private:
	explicit CSwordMan_AuraBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CSwordMan_AuraBlade(const CSwordMan_AuraBlade& rhs);
	virtual ~CSwordMan_AuraBlade() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	virtual HRESULT Ready_Components() override;

private:
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

private:
	_float m_fAccDeletionTime = 0.f;
	_float m_fDeletionTime = 1.f;
	_float m_fMoveSpeed = 20.f;


public:
	static CSwordMan_AuraBlade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

