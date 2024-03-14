#include "stdafx.h"
#include "UI_Milepost.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Camera_Manager.h"
#include "Camera.h"
#include "UI_Manager.h"

CUI_Milepost::CUI_Milepost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MILEPOST eType)
	: CUI(pDevice, pContext, L"UI_Milepost")
	, m_eType(eType)
{
}

CUI_Milepost::CUI_Milepost(const CUI_Milepost& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Milepost::Set_Active(_bool bActive)
{
//	if (false == m_bGoal)
//		return;

	if (false == bActive)
	{
		if (m_bEvent)
			m_bEvent = false;

		m_bGoal = false;
	}

	m_bActive = bActive;
}

HRESULT CUI_Milepost::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Milepost::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	//Set_TargetPosition(_float4(-69.5f, -2.7f, -10.f, 1.f));

	m_bActive = true;

	return S_OK;
}

void CUI_Milepost::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (MILEPOST_ARROW == m_eType)
			Rotation_Arrow();

		__super::Tick(fTimeDelta);
	}
}

void CUI_Milepost::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != CUI_Manager::GetInstance()->Get_Character())
		{
			if (true == m_bGoal) // 목표가 설정되어 있다면
			{
				// 카메라 범위
				_float4 vCamPos = GI->Get_CamPosition();
				_vector vCameraPos = XMLoadFloat4(&vCamPos);
				_vector vTargetToCamera = XMLoadFloat4(&m_vTargetPos) - vCameraPos;
				_float fDistance = XMVectorGetX(XMVector3Length(vTargetToCamera));

				vTargetToCamera = XMVector3Normalize(vTargetToCamera);
				CCamera* pCurCamera = CCamera_Manager::GetInstance()->Get_CurCamera();
				if (nullptr == pCurCamera)
					return;
				CTransform* pCameraTrans = pCurCamera->Get_Transform();
				if (nullptr == pCameraTrans)
					return;
				_vector vCamLook = XMVector3Normalize(pCameraTrans->Get_State(CTransform::STATE_LOOK));
				_float fAngle = XMVectorGetX(XMVector3Dot(vTargetToCamera, vCamLook));

				if (3.f < fDistance) // 거리가 3보다 크다면
				{
					if (fAngle >= XMConvertToRadians(0.f) && fAngle <= XMConvertToRadians(180.f))
					{
						_matrix matTemp = XMMatrixIdentity();
						matTemp.r[3] = XMLoadFloat4(&m_vTargetPos);
						_float4x4 matTargetWorld;
						XMStoreFloat4x4(&matTargetWorld, matTemp);

						_float4x4 matWorld;
						matWorld = matTargetWorld;
						_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
						_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

						_float4x4 matWindow;
						XMStoreFloat4x4(&matWindow, XMLoadFloat4x4(&matWorld) * matView * matProj);

						_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];

						vWindowPos.x /= vWindowPos.z;
						vWindowPos.y /= vWindowPos.z;

						m_vCurrentPos.x = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
						m_vCurrentPos.y = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);

						//만약 화면(1600:900) 안에 들어온다면 그린다.
						if ((0.f + m_tInfo.fCX < m_vCurrentPos.x && _float(g_iWinSizeX) - m_tInfo.fCX > m_vCurrentPos.x) &&
							(0.f + m_tInfo.fCY < m_vCurrentPos.y && _float(g_iWinSizeY) - m_tInfo.fCY > m_vCurrentPos.y))
							m_bRender = true;
						else
							m_bRender = false;

						m_tInfo.fX = m_vCurrentPos.x;
						m_tInfo.fY = m_vCurrentPos.y;

						m_pTransformCom->Set_State(CTransform::STATE_POSITION,
							XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
					}

					// 깃발에 미터 텍스트를 추가한다.
					if (m_eType == UI_MILEPOST::MILEPOST_FLAG)
					{
						CTransform* pPlayerTransform = CUI_Manager::GetInstance()->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
						if (nullptr == pPlayerTransform)
							return;
						_vector vTemp = XMLoadFloat4(&m_vTargetPos) - (pPlayerTransform->Get_Position());
						_float fTotarget = XMVectorGetX(XMVector3Length(vTemp));

						if (fTotarget < 3.f)
							CUI_Manager::GetInstance()->Off_Milepost();

						if (true == m_bRender)
						{
							wstring strDistance = to_wstring(_uint(fTotarget)) + TEXT("M");
							_int iLength = strDistance.length() - 1;
							_float fOffset = iLength * 3.5f;
							_float2 vFontPos = _float2((m_vCurrentPos.x - 6.f) - fOffset, m_vCurrentPos.y + 4.f);

							CRenderer::TEXT_DESC TextDesc = {};
							TextDesc.strText = strDistance;
							TextDesc.strFontTag = L"Default_Bold";
							TextDesc.vScale = { 0.25f, 0.25f };
							TextDesc.vColor = _float4(0.655f, 0.475f, 0.325f, 1.f);
							TextDesc.vPosition = _float2(vFontPos.x - 1.f, vFontPos.y);
							m_pRendererCom->Add_Text(TextDesc);
							TextDesc.vPosition = _float2(vFontPos.x + 1.f, vFontPos.y);
							m_pRendererCom->Add_Text(TextDesc);
							TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y - 1.f);
							m_pRendererCom->Add_Text(TextDesc);
							TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y + 1.f);
							m_pRendererCom->Add_Text(TextDesc);

							TextDesc.vColor = _float4(1.f, 0.973f, 0.588f, 1.f);
							TextDesc.vPosition = vFontPos;
							m_pRendererCom->Add_Text(TextDesc);
						}
					}
				}
				else // 거리가 3보다 작다면
				{
					CUI_Manager::GetInstance()->Off_Milepost();
				}
			}
		}
		
		if (true == m_bGoal && true == m_bRender)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}

}

HRESULT CUI_Milepost::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Milepost::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UI_MILEPOST::MILEPOST_FLAG:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_TargetPosition_Flag"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MILEPOST::MILEPOST_ARROW:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_TargetPosition_Arrow"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CUI_Milepost::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Milepost::Bind_ShaderResources()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

void CUI_Milepost::Rotation_Arrow()
{
	if (false == m_bGoal)
		return;

	CCamera* pCurCamera = CCamera_Manager::GetInstance()->Get_CurCamera();
	if (nullptr == pCurCamera)
		return;

	CTransform* pCameraTrans = pCurCamera->Get_Transform();
	if (nullptr == pCameraTrans)
		return;

	CTransform* pPlayerTransform = CUI_Manager::GetInstance()->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pPlayerTransform)
		return;

//	Vec3 vScale = m_pTransformCom->Get_Scale();							// 1. MilePost의 스케일을 저장해놓는다.
//	Vec2 vProjTargetPos = Get_ProjectionPosition(m_vTargetPos);			// 2. 타겟포지션의 윈도우 좌표를 구한다.
//	Vec2 vProjPlayerPos = Get_ProjectionPosition(pPlayerTransform);		// 3. 플레이어포지션의 윈도우 좌표를 구한다.
//
//	if ((0.f <= vProjTargetPos.x && _float(g_iWinSizeX) >= vProjTargetPos.y)
//		&& (0.f <= vProjTargetPos.y && _float(g_iWinSizeY) >= vProjTargetPos.y))
//	{
//		// 화면 안에 TargetPosition이 있다면, 화살표를 노출하지 않는다.
//		m_fAlpha = 0.f;
//	}
//	else
//	{
//		m_fAlpha = 1.f;
//
//		// 5. 윈도우 상의 좌표에서 플레이어 포지션에서 타겟포지션으로 향하는 방향을 구한다.
//		Vec2 vDir = XMVector2Normalize(vProjTargetPos - vProjPlayerPos);
//		Vec4 vDirVector = XMVector3Normalize(XMLoadFloat4(&m_vTargetPos) - pPlayerTransform->Get_Position());
//
//		// 6.	(1)월드상의 플레이어 포지션에서 타겟포지션으로 향하는 방향벡터와,
//		//		(2) 카메라의 룩벡터를 내적한다. 만약 -값이 나오면, vDir의 Y값을 역으로 만든다.
//		_float fDot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(XMLoadFloat4(&m_vTargetPos) - pPlayerTransform->Get_Position()), XMVector3Normalize(pCameraTrans->Get_Look())));
//
//		Vec3 vRight, vUp, vLook;
//		vUp = XMVector3Normalize(XMVectorSet(vDir.x, vDir.y, 0.f, 0.f));
//		if (fDot < 0.f)
//			vUp.y *= -1.f;
//		vLook = XMVector3Normalize(XMVectorSet(0.f, 0.f, 1.f, 0.f));
//		vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));
//
//		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight * vScale.x);
//		m_pTransformCom->Set_State(CTransform::STATE_UP, vUp * vScale.y);
//		m_pTransformCom->Set_State(CTransform::STATE_LOOK, vLook * vScale.z);
//	}

	// - - - - - - - - - - - - - - - - - - - 

//	_vector vStandard = XMVectorSet(0.f, 0.f, 1.f, 0.f);
//	Vec4 vDirVector = XMVector3Normalize(XMLoadFloat4(&m_vTargetPos) - pPlayerTransform->Get_Position());
//
//	// 두 벡터 사이의 각도 계산
//	_float fDot = XMVectorGetX(XMVector3Dot(vStandard, vDirVector));
//	_float fDegree = XMConvertToDegrees(acos(fDot));
//
//	// 두 벡터의 외적을 통해 회전 방향 결정
//	if (XMVectorGetY(XMVector3Cross(vStandard, vDirVector)) > 0.f)
//		fDegree *= -1.f;
//
//	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fDegree));


	Vec3 vTargetDir = XMVector3Normalize(XMLoadFloat4(&m_vTargetPos) - pPlayerTransform->Get_Position());
	Vec3 vPlayerLook = XMVector3Normalize(pPlayerTransform->Get_Look());
	_float fAngle = XMConvertToDegrees(acos(XMVectorGetX(XMVector3Dot(vPlayerLook, vTargetDir))));

	Vec3 vCross = XMVector3Cross(vPlayerLook, vTargetDir);
	if (XMVectorGetY(vCross) > 0.f)
		fAngle *= -1.f;

	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fAngle));
}

CUI_Milepost* CUI_Milepost::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MILEPOST eType)
{
	CUI_Milepost* pInstance = new CUI_Milepost(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Milepost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Milepost::Clone(void* pArg)
{
	CUI_Milepost* pInstance = new CUI_Milepost(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Milepost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Milepost::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
