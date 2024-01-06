#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

class CCamera_CutScene_Boss final : public CCamera
{
	enum BOSS_TYPE			{ GLANIX, WITCH, STELLIA, BOSS_TYPEEND };

	enum PROGRESS_GLANIX	{ APPEAR, CHANGE_PAGE, DEAD, PROGRESS_GLANIX_END };

private:
	CCamera_CutScene_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_CutScene_Boss(const CCamera_CutScene_Boss& rhs);
	virtual ~CCamera_CutScene_Boss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Tick_Blending(const _float fDeltaTime) override;

public:
	HRESULT Start_CutScene(const _uint& iBossType, const _uint& iProgressType);

public:
	virtual Vec4 Get_LookAt() override;

private:
	virtual HRESULT Ready_Components() override;

private:
	BOSS_TYPE		m_eCurBossType = BOSS_TYPE::BOSS_TYPEEND;
	_uint			m_iCurProgressType = 99;

public:
	static CCamera_CutScene_Boss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END