#pragma once

#include "Client_Defines.h"
#include "Character_Projectile.h"

BEGIN(Client)

class CCannon_Ball final : public CCharacter_Projectile
{
private:
	explicit CCannon_Ball(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CCannon_Ball(const CCannon_Ball& rhs);
	virtual ~CCannon_Ball() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;

public:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;

private:
	_bool Find_Dest_Position(_float fTimeDelta);

private:
	class CPhysX_Controller* m_pControllerCom = nullptr;

private:
	_float m_fAccEffect = 0.f;
	_float m_fGenEffectTime = 0.5f;

private:
	_float m_fAccMove = 0.f;
	_float m_fArriveTime = 2.f;

private:
	_bool m_bInitLook = false;
	_bool m_bInitDestPosition = false;

	Vec4 m_vStartPosition = {};
	Vec4 m_vDestPosition = {};
	Vec3 m_vInitLook = {};

public:
	static CCannon_Ball* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

