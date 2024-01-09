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

class CUI_Dummy_Weapon final : public CGameObject
{
protected:
	CUI_Dummy_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObejctTag);
	virtual ~CUI_Dummy_Weapon() = default;

public:
	HRESULT	Set_Owner(CGameObject* pOwner, CHARACTER_TYPE eType);
	class CGameObject* Get_Owner() { return m_pOwner; }

	void Set_Active(_bool bActive) { m_bActive = bActive; }
	_bool Get_Active() { return m_bActive; }

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	void Set_WeaponModelCom(class CModel* pModel) { m_pModelCom = pModel; }
	class CModel* Get_WeaponModelCom() { return m_pModelCom; }

public:
	virtual void Set_SocketWorld(Matrix matSocketWorld) { memcpy(&m_matSocketWorld, &matSocketWorld, sizeof(Matrix)); } /* 주인 모델의 애니메이션 갱신이 이루어진 뒤 호출 */

protected:
	class CGameObject* m_pOwner = { nullptr };
	class CModel* m_pModelCom = { nullptr };
	class CShader* m_pShaderCom = { nullptr };
	class CRenderer* m_pRendererCom = { nullptr };
	class CTransform* m_pTransformCom = { nullptr };

protected:
	Matrix m_matSocketWorld;

protected:
	_bool m_bActive = { false };
	_vector m_vCamPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_matrix m_vCamMatrix = XMMatrixIdentity();
	_float4x4 m_ViewMatrix;
	CHARACTER_TYPE m_eCurCharacter = { CHARACTER_TYPE::SWORD_MAN };


public:
	static CUI_Dummy_Weapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END