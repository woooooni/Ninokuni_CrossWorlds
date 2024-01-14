#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CCharacter_Projectile abstract : public CGameObject
{

public:
	typedef struct tagCharacterProjectileDesc
	{
		CGameObject* pOwner = nullptr;
	} CHARACTER_PROJECTILE_DESC;

protected:
	CCharacter_Projectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjTag);
	CCharacter_Projectile(const CCharacter_Projectile& rhs);
	virtual ~CCharacter_Projectile() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;


protected:
	class CModel* m_pModelCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

protected:
	class CGameObject* m_pOwner = nullptr;

protected:
	_float m_fAccDeletionTime = 0.f;
	_float m_fDeletionTime = 1.f;
	_float m_fMoveSpeed = 20.f;

public:
	virtual void Free() override;
};

END

