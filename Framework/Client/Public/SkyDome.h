#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CPipeLine;
class CTransform;
END


BEGIN(Client)
class CSkyDome final : public CGameObject 
{
private:
	enum LEVEL_SKY { EVERMORE_SKY, WINTER_SKY, WITCH_SKY, SKY_END };

protected:
	explicit CSkyDome(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CSkyDome(const CSkyDome& rhs);
	virtual ~CSkyDome() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Ready_Components();

public:
	void Set_ApexColor(Vec4 vColor) { m_vApexColor = vColor; }
	void Set_CenterColor(Vec4 vColor) { m_vCenterColor = vColor; }

	Vec4 Get_ApexColor() { return m_vApexColor; }
	Vec4 Get_CenterColor() { return m_vCenterColor; }

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CTexture* m_pTextureCom[LEVEL_SKY::SKY_END] = {};


private:
	Vec4 m_vApexColor = Vec4(0.173f, 0.631f, 0.851f, 1.0f);								// ¹Ø È¤Àº À­ºÎºÐ »ö±ò.
	Vec4 m_vCenterColor = Vec4(183.0f / 255.0f, 255.0f/ 255.0f , 255.0f / 255.0f,1.0f);	// ¼¾ÅÍ ÃàºÎÅÍ ¾î¶² »ö±ò.

	//Vec4 m_vApexColor = Vec4(0.671f, 0.059f, 0.0f, 1.0f);								// ¹Ø È¤Àº À­ºÎºÐ »ö±ò.
	//Vec4 m_vCenterColor = Vec4(0.329f, 0.173f, 0.157f, 1.0f);	// ¼¾ÅÍ ÃàºÎÅÍ ¾î¶² »ö±ò.
public:
	static CSkyDome* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END

