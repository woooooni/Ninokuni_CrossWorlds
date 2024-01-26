#pragma once
#include "Boss.h"

BEGIN(Client)
class CQuest_DestSpot final : public CGameObject
{
private:
	CQuest_DestSpot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CQuest_DestSpot(const CQuest_DestSpot& rhs);
	virtual ~CQuest_DestSpot() = default;

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
	_bool	Get_IsCol() { return m_bIsCol; }
	void	Set_ReadyDelete(_bool bIsDelete) { m_bIsDelete = bIsDelete; }

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();

private:
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;

	class CVfx* pEffectObject = nullptr;
	class CUI_Minimap_Icon* m_pMinimapIcon = { nullptr };

	_bool m_bIsCol = false;
	_bool m_bIsDelete = false;

public:
	static CQuest_DestSpot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

