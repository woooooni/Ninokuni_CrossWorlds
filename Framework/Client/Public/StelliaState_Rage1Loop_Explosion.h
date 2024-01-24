#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage1Loop_Explosion final : public CStelliaState_Base
{
private:
	CStelliaState_Rage1Loop_Explosion(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage1Loop_Explosion() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

#ifdef _DEBUG
public:
	virtual HRESULT Render() override;

private:
	HRESULT Ready_DebugDraw();
	HRESULT Render_DebugDraw();
#endif


private:
	Vec4 m_vLook = {};
	Vec4 m_vLeftRot = {};
	Vec4 m_vRightRot = {};

	_float m_fRotAngle = 0.f;
	_float m_fDist = 0.f;

#pragma region Debug Draw 
	BasicEffect* m_pEffect = nullptr;
	BoundingSphere* m_pSphere = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
#pragma endregion

public:
	static CStelliaState_Rage1Loop_Explosion* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

