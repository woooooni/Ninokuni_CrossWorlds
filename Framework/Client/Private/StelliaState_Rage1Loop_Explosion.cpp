#include "stdafx.h"
#include "StelliaState_Rage1Loop_Explosion.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "Decal.h"

CStelliaState_Rage1Loop_Explosion::CStelliaState_Rage1Loop_Explosion(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage1Loop_Explosion::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fRotAngle = 21.f;
	m_fDist = 10.f;
	m_fStelliaCenterOffSetDist = 5.f;

#ifdef _DEBUG
	if (FAILED(Ready_DebugDraw()))
		return E_FAIL;
#endif

	return S_OK;
}

void CStelliaState_Rage1Loop_Explosion::Enter_State(void* pArg)
{
	m_vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
	
	m_vLeftRot = XMVector3Rotate(m_vLook, XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(-m_fRotAngle), 0.0f));
	m_vRightRot = XMVector3Rotate(m_vLook, XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(m_fRotAngle), 0.0f));

	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill04"));

	m_pStellia->Set_StelliaHit(false);

	// Effect Create
	CTransform* pTransformCom = m_pStellia->Get_Component<CTransform>(L"Com_Transform");
	if (pTransformCom == nullptr)
		return;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Rage01Explosion"), pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_Rage1Loop_Explosion::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);
	__super::Rage1_Tick(fTimeDelta);

	Vec4 vDirToPlayer = (Vec4)m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position() - ((Vec4)m_pTransformCom->Get_Position() + m_vLook * m_fStelliaCenterOffSetDist);

	// 스텔리아의 룩과 플레이어의 현재 위치의 라디안 각도.
	_float fAngleToPlayer = XMVectorGetX(XMVector3AngleBetweenNormals(m_vLook, XMVector3Normalize(vDirToPlayer)));

	if (m_pModelCom->Get_CurrAnimationFrame() >= 65 && m_pModelCom->Get_CurrAnimationFrame() <= 70)
	{
		_float fLength = vDirToPlayer.Length();
		// 플레이어가 안전지대 범위 내에 없다.
		if (XMConvertToDegrees(fAngleToPlayer) >= m_fRotAngle || fLength > m_fDist)
		{
			if (CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Hp() > 0)
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Decrease_HP(999);
		}
	}
 
	// 안전 범위 내에 있을 때에만 플레이어가 스텔리아 공격 가능
	if (XMConvertToDegrees(fAngleToPlayer) < m_fRotAngle && vDirToPlayer.Length() < m_fDist)
		m_pStellia->Set_StelliaHit(true);
	else
		m_pStellia->Set_StelliaHit(false);

	//
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		if (m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_SKILLAROUND))
			m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1LOOP_SPINTAIL);
		else
			m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1LOOP_JUMPSTAMP);
	}
}

void CStelliaState_Rage1Loop_Explosion::Exit_State()
{
}

#ifdef _DEBUG
HRESULT CStelliaState_Rage1Loop_Explosion::Render()
{
	Render_DebugDraw();

	return S_OK;
}

HRESULT CStelliaState_Rage1Loop_Explosion::Ready_DebugDraw()
{
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(GI->Get_Context());
	m_pEffect = new BasicEffect(GI->Get_Device());
	
	m_pEffect->SetVertexColorEnabled(true);
	
	const void* pShaderByteCodes = nullptr;
	size_t			iLength = 0;
	
	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);
	
	if (FAILED(GI->Get_Device()->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;
	
	_float	fRadius = 0.5f;
	Vec3	vOrigin = { 0.f, fRadius * 0.5f, 0.f };
	m_pSphere = new BoundingSphere(vOrigin, fRadius);
	
	if (nullptr == m_pSphere)
		return E_FAIL;
	
	return S_OK;
}

HRESULT CStelliaState_Rage1Loop_Explosion::Render_DebugDraw()
{
	m_vLeftRot.Normalize();
	m_vRightRot.Normalize();

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));
	
	m_pEffect->Apply(GI->Get_Context());
	
	GI->Get_Context()->IASetInputLayout(m_pInputLayout);
	
	m_pBatch->Begin();
	{
		Vec4 vOffsetPos = Vec4((Vec4)m_pTransformCom->Get_Position() + m_vLook * (m_fStelliaCenterOffSetDist)).xyz();
		
		m_pSphere->Center = Vec4((Vec4)m_pTransformCom->Get_Position() + m_vLook * (m_fStelliaCenterOffSetDist)).xyz();
		DX::Draw(m_pBatch, *m_pSphere, Colors::Yellow);

		m_pSphere->Center = Vec4(vOffsetPos + (m_vLeftRot * m_fDist)).xyz();
		DX::Draw(m_pBatch, *m_pSphere, Colors::Yellow);

		m_pSphere->Center = Vec4(vOffsetPos + (m_vRightRot * m_fDist)).xyz();
		DX::Draw(m_pBatch, *m_pSphere, Colors::Yellow);
	}
	m_pBatch->End();
	
	return S_OK;
}
#endif

CStelliaState_Rage1Loop_Explosion* CStelliaState_Rage1Loop_Explosion::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage1Loop_Explosion* pInstance = new CStelliaState_Rage1Loop_Explosion(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage1Loop_Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage1Loop_Explosion::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Delete(m_pSphere);
	Safe_Release(m_pInputLayout);
#endif
}

