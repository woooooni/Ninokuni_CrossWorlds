#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CRenderer;
class CTransform;
class CRigidBody;
END

BEGIN(Client)
class CVehicleFlying_Projectile abstract : public CGameObject
{
public:
	typedef struct tagFlyingProjectileDesc
	{
		_bool bPool = false;
		class CVehicle_Flying* pOwner = { nullptr };

	} GRANDPRIX_PROJECTILE_DESC;

protected:
	CVehicleFlying_Projectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjTag, OBJ_TYPE eObjectType);
	CVehicleFlying_Projectile(const CVehicleFlying_Projectile& rhs);
	virtual ~CVehicleFlying_Projectile() = default;

public:
	void Set_Owner(class CVehicle_Flying* pOwner);
	class CVehicle_Flying* Get_Owner() { return m_pOwner; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;

public:
	void Set_CameraTarget(_bool bTarget) { m_bCameraTarget = bTarget; }

protected:
	CModel* m_pModelCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CRigidBody* m_pRigidBodyCom = { nullptr };

protected:
	class CVehicle_Flying* m_pOwner = { nullptr };

protected:
	_float m_fAccDeletionTime = 0.f;
	_float m_fDeletionTime = 1.f;
	_float m_fMoveSpeed = 20.f;

protected:
	_bool m_bCameraTarget = false;

public:
	virtual void Free() override;
};
END

