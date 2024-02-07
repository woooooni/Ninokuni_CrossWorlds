#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Free final : public CCamera
{
private:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	const _float& Get_MoveSpeed() const { return m_fMoveSpeed; }
	const _float& Get_RotateSpeed() const { return m_fRotateSpeed; }

	void Set_MoveSpeed(const _float& fSpeed) { m_fMoveSpeed = fSpeed; }
	void Set_RotateSpeed(const _float& fSpeed) { m_fRotateSpeed = fSpeed; }

private:
	void Move(const _float fTimeDelta);
	void Rotate(const _float fTimeDelta);

private:
	virtual HRESULT Ready_Components() override;

private:
	_float m_fMoveSpeed		= 30.f;
	_float m_fRotateSpeed	= 0.5f;

public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END