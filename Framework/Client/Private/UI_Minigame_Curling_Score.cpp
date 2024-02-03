#include "stdafx.h"
#include "UI_Minigame_Curling_Score.h"
#include "GameInstance.h"

#include "CurlingGame_Group.h"

CUI_Minigame_Curling_Score::CUI_Minigame_Curling_Score(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType)
	: CUI_Minigame_Curling_Base(pDevice, pContext, wstrTag)
	, m_bPlayerType(bPlayerType)
{
}

CUI_Minigame_Curling_Score::CUI_Minigame_Curling_Score(const CUI_Minigame_Curling_Score& rhs)
	: CUI_Minigame_Curling_Base(rhs)
	, m_bPlayerType(rhs.m_bPlayerType)
{
}

HRESULT CUI_Minigame_Curling_Score::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Score::Initialize(void* pArg)
{
	CUI::UI_INFO UIDesc = {};
	{
		UIDesc.fCX = UIDesc.fCY = UIDesc.fX = UIDesc.fY = 1.f;
	}

	if (FAILED(__super::Initialize(&UIDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Default()))
		return E_FAIL;

	return S_OK;
}

void CUI_Minigame_Curling_Score::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);
}

void CUI_Minigame_Curling_Score::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Minigame_Curling_Score::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	vector<CUI_Minigame_Curling_Score::BIND_DESC> Descs = Get_BindDesc();

	for (auto& desc : Descs)
	{
		/* Bind */
		{
			m_pTransformCom->Set_Scale(XMVectorSet(desc.tUIDesc.fCX, desc.tUIDesc.fCY, 1.f, 0.f));

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(desc.tUIDesc.fX, desc.tUIDesc.fY, 0.f, 1.f));

			if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
				return E_FAIL;

			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", desc.iTextureIndex)))
				return E_FAIL;
		}

		/* Render */
		{
			m_iPass = 1; // PS_MAIN_ALPHA

			m_pShaderCom->Begin(m_iPass);

			m_pVIBufferCom->Render();
		}
	}

	return S_OK;
}

void CUI_Minigame_Curling_Score::Send_Message_Int(const _int& iValue)
{
	m_iCurScore = iValue;
}

HRESULT CUI_Minigame_Curling_Score::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_ICELAND, TEXT("Prototype_Component_Texture_UI_Minigame_Curling_Count"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Score::Ready_Default()
{
	if (FAILED(__super::Ready_Default()))
		return E_FAIL;

	const Vec2 vOriginSize	= { 256.f, 256.f };
	const Vec2 vSizeMag		= { 0.5f, 0.5f };
	const Vec2 vPosDelta	= { g_vWinCenter.x * 0.815f, g_vWinCenter.y * 0.475f };

	if(m_bPlayerType)
		m_tUIDesc_OriginLeft.fX = g_vWinCenter.x + vPosDelta.x;
	else
		m_tUIDesc_OriginLeft.fX = g_vWinCenter.x - vPosDelta.x;

	m_tUIDesc_OriginLeft.fY = g_vWinCenter.y + vPosDelta.y;
	m_tUIDesc_OriginLeft.fCX = vOriginSize.x * vSizeMag.x;
	m_tUIDesc_OriginLeft.fCY = vOriginSize.y * vSizeMag.y;

	return S_OK;
}

HRESULT CUI_Minigame_Curling_Score::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

const vector<CUI_Minigame_Curling_Score::BIND_DESC> CUI_Minigame_Curling_Score::Get_BindDesc()
{
	// 현재 점수를 통해, 자릿수를 분석한 뒤 하나의 숫자에 해당하는 텍스처 인덱스와 Ui Info 정보를 벡터에 저장한다. 

	vector<CUI_Minigame_Curling_Score::BIND_DESC> Descs;

	vector<_uint> iNums;
	{
		_uint iRemainScore = m_iCurScore;

		while (iRemainScore > 0)
		{
			iNums.push_back(iRemainScore % 10);
			iRemainScore /= 10;
		}

		if (iNums.empty())
			iNums.push_back(iRemainScore);

		reverse(iNums.begin(), iNums.end());
	}

	for (size_t i = 0; i < iNums.size(); i++)
	{
		CUI_Minigame_Curling_Score::BIND_DESC desc;
	
		desc.iTextureIndex = iNums[i];

		memcpy(&desc.tUIDesc, &m_tUIDesc_OriginLeft, sizeof(CUI::UI_INFO));

		if (m_bPlayerType) 
			desc.tUIDesc.fX - ((iNums.size() - i) * m_iInterdigitSpacing);
		else 
			desc.tUIDesc.fX + (i * m_iInterdigitSpacing); 

		Descs.push_back(desc);
	}

	return Descs;
}

CUI_Minigame_Curling_Score* CUI_Minigame_Curling_Score::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType)
{
	CUI_Minigame_Curling_Score* pInstance = new CUI_Minigame_Curling_Score(pDevice, pContext, wstrTag, bPlayerType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_Curling_Score");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_Curling_Score::Clone(void* pArg)
{
	CUI_Minigame_Curling_Score* pInstance = new CUI_Minigame_Curling_Score(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_Curling_Score");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Curling_Score::Free()
{
	__super::Free();
}
