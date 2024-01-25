#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CHierarchyNode;
class CModel;
class CShader;
class CTransform;
class CRenderer;
END

BEGIN(Client)

class CWeapon abstract : public CGameObject
{
protected:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObejctTag, _uint iObjectType);
	virtual ~CWeapon() = default;

public:
	HRESULT	Set_Owner(CGameObject* pOwner);
	class CGameObject* Get_Owner() { return m_pOwner; }

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) {};
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) {};
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) {};

public:
	void Generate_Trail(const wstring& strDiffuseTextureName, const wstring& strAlphaTextureName, const _float4& vColor, _uint iVertexCount);
	void Stop_Trail();

protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

protected:
	virtual HRESULT Ready_Trails() PURE;

public:
	void Set_WeaponModelCom(class CModel* pModel) { m_pModelCom = pModel; }
	class CModel* Get_WeaponModelCom() { return m_pModelCom; }

public:
	HRESULT Appear_Weapon();
	HRESULT Disappear_Weapon();

public:
	virtual void Set_SocketWorld(Matrix matSocketWorld) { memcpy(&m_matSocketWorld, &matSocketWorld, sizeof(Matrix)); } /* 주인 모델의 애니메이션 갱신이 이루어진 뒤 호출 */

protected:
	class CGameObject* m_pOwner = { nullptr };
	class CModel* m_pModelCom = { nullptr };
	class CShader* m_pShaderCom = { nullptr };
	class CRenderer* m_pRendererCom = { nullptr };
	class CTransform* m_pTransformCom = { nullptr };
	class CTexture* m_pDissolveTextureCom = { nullptr };

protected:
	class CTrail* m_pTrail = nullptr;

protected:
	Matrix m_matSocketWorld;

protected:
	_bool m_bDisappear = false;
	_float m_fAccDissolve = 0.f;


public:
	virtual void Free() override;
};

END