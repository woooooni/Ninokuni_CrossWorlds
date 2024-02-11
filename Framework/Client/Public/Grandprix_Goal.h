#pragma once
#include "Boss.h"

BEGIN(Client)
class CGrandprix_Goal final : public CGameObject
{
private:
	CGrandprix_Goal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGrandprix_Goal(const CGrandprix_Goal& rhs);
	virtual ~CGrandprix_Goal() = default;

public:
	void Set_Active(_bool bActive) { m_bActive = bActive; }
	_bool Is_Active() { return m_bActive; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	virtual HRESULT Render_Instance(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);
	virtual HRESULT Render_Instance_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();

private:
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CTexture* m_pDissoveTexture = { nullptr };

private:
	_float m_fSpeed = { 0.f };
	_bool m_bActive = { false };

public:
	static CGrandprix_Goal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

