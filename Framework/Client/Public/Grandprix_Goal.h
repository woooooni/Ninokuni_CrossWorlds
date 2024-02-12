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
	void Set_Active(_bool bActive);
	_bool Is_Active() { return m_bActive; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_ShadowDepth();

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

	_bool m_bStartDissolve = false;
	_float4 m_vDissolveColor = _float4(0.715f, 0.995f, 1.f, 1.f);
	_float  m_fDissolveTotal = 10.f;
	_float  m_fDissolveDuration = 5.f;
	_float  m_fDissolveWeight = 0.f;
	_float  m_fDissolveSpeed = 3.f;

public:
	static CGrandprix_Goal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

