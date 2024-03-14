#pragma once

#include "Glanix.h"

BEGIN(Client)
class CGlanix_Phoenix final : public CGameObject
{
public:
	enum GLANIX_PHOENIX_STATE { PHOENIX_SPAWN, PHOENIX_IDLE, PHOENIX_OUT, PHOENIX_END };

private:
	CGlanix_Phoenix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CGlanix_Phoenix(const CGlanix_Phoenix& rhs);
	virtual ~CGlanix_Phoenix() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;


public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	_bool	Get_IsOut() { return m_bIsOut; }
	_float4 Get_OriginPos() { return m_pOriginPos; }

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Colliders();

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CPhysX_Controller* m_pControllerCom = { nullptr };
	CStateMachine* m_pStateCom = nullptr;


private:
	_float4 m_pOriginPos = {};
	_bool m_bIsOut = false;
	_int* m_pSlowStack = nullptr;

public:
	static CGlanix_Phoenix* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

