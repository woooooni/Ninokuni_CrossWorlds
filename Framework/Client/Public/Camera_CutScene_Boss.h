#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

class CCamera_CutScene_Boss final : public CCamera
{
public:

	enum BOSS_TYPE							{ GLANIX, WITCH, STELLIA, BOSS_TYPEEND };

#pragma region GRANIX 

	enum GLANIX_CUTSCENE_TYPE				{ APPEAR, PAGE, DEAD, GLANIX_CUTSCENE_TYPEEND};

	enum GLANIX_CUTSCENE_APPEAR_PROGRESS	{ IDEL, ROAR, JUMP, LAND, GLANIX_CUTSCENE_APPEAR_PROGRESSEND};

	typedef struct tagGlanixCutSceneAppearDesc
	{
		/* 진행 상황에 따른 오프셋을 Pair(Target Offset, Look At Offset) 배열로 관리한다. */
		pair<Vec4, Vec4> Offsets[GLANIX_CUTSCENE_APPEAR_PROGRESSEND]
		{
			{Vec4{-4.5f, 6.f, -6.f, 1.f}, Vec4{0.f, 1.f, 0.f, 1.f}}, //	IDEL : 뒷 모습 
			{Vec4{0.f, 2.0f, 7.f, 1.f}, Vec4{0.f, 0.75f, 0.f, 1.f}}, //	ROAR : 앞모습 
			{Vec4{0.f, 2.0f, 7.f, 1.f}, Vec4{0.f, 0.75f, 0.f, 1.f}}, //	JUMP : 뒷모습 
			{Vec4{0.f, 2.f, 7.f, 1.f}, Vec4{-0.5f, 0.5f, 0.f, 1.f}}	 //	LAND : 착륙 지점에서 앞 모습 (위에서 아래로 뷰)
		};

	}GLANIX_CUTSCENE_APPEAR_DESC;

	enum GLANIX_CUTSCENE_PAGE_PROGRESS		{ };

	enum GLANIX_CUTSCENE_DEAC_PROGRESS		{ };

#pragma endregion


private:
	CCamera_CutScene_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_CutScene_Boss(const CCamera_CutScene_Boss& rhs);
	virtual ~CCamera_CutScene_Boss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override { return S_OK; }

public:
	virtual void Tick_Blending(const _float fDeltaTime) override;

public:
	HRESULT Start_CutScene(const _uint& iBossType, const _uint& iCutSceneType, CGameObject* pBoss);
	HRESULT Finish_CutScene();
	void	Send_Signal() { m_bSignal = true; }
	
	const _bool& Is_Playing() const { return m_bPlaying; }
	virtual Vec4 Get_LookAt() override { return Vec4(); }


private:
	virtual HRESULT Ready_Components() override;

private:
	HRESULT Start_CutScene_Granix(const _uint& iCutSceneType, CGameObject* pBoss);

	void Tick_CutScene_Granix(const _float fDeltaTime);

	void Clear_Data();

private:
	_int			m_iCurBossType = -1;
	_int			m_iCurCutSceneType = -1;
	_int			m_iCurProgressType = -1;

	_bool			m_bPlaying = false;

	_bool			m_bSignal = false;

	GLANIX_CUTSCENE_APPEAR_DESC m_tGlanixCutSceneAppearDesc;

public:
	static CCamera_CutScene_Boss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END