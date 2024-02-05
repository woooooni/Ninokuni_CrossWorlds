#pragma once
#include "Boss.h"

BEGIN(Client)
class CGrandprix_ItemBox final : public CGameObject
{
public:
	enum ITEMBOX_TYPE { ITEMBOX_SPEEDUP, ITEMBOX_SIZEUP, ITEMBOX_BOMB, ITEMBOX_SLOW, ITEMBOX_END };

private:
	CGrandprix_ItemBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CGrandprix_ItemBox(const CGrandprix_ItemBox& rhs);
	virtual ~CGrandprix_ItemBox() = default;

public:
	void Set_MinMaxPosition(Vec4 vOriginPos);
	void Set_Active(_bool bActive) { m_bActive = bActive; }
	_bool Is_Active() { return m_bActive; }
	const ITEMBOX_TYPE& Get_ItemType() { return m_eItemType; }

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

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();
	void Update_Position(_float fTimeDelta);
	void Update_Rotation(_float fTimeDelta);

private:
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	ITEMBOX_TYPE m_eItemType = { ITEMBOX_END };

	Vec3 m_vRotationAngle = { 0.f, 0.f, 0.f };
	_float m_vMaxHeight = { 0.f };
	_float m_vMinHeight = { 0.f };
	_float m_fSpeed = { 0.f };
	_bool m_bReverse = { false };

private:
	_bool m_bActive = { false };

public:
	static CGrandprix_ItemBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	friend class CGlanix_IcePillar_Controller;
};
END

