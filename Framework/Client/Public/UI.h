#pragma once
#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CRenderer;
class CShader;
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI abstract : public CGameObject
{
public:
	enum UI_MOUSESTATE { MOUSE_ENTER, MOUSE_ON, MOUSE_EXIT, MOUSE_DRAG_START, MOUSE_DRAG, MOUSE_DRAG_END, MOUSE_END };

public:
	typedef struct tagUI_Info
	{
		_float fX	= 0.f; _float fY	= 0.f; _float fCX	= 0.f; _float fCY	= 0.f;
		CUI* pParent = nullptr; void* pDesc = nullptr;

	} UI_INFO;
	
protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	_float2 Get_ProjectionPosition(CTransform* pTransfrom);
	_float2 Get_ProjectionPosition(_float4 vPosition);
	const UI_INFO& Get_UI_Info() { return m_tInfo; }
	void Set_UI_Info(const UI_INFO& tInfo) { m_tInfo = tInfo; }

	_bool Has_EventOccurred() { return m_bEvent; }

	_bool Get_Active() { return m_bActive; }
	virtual void Set_Active(_bool bActive);

	virtual void Set_ChildAlpha(float fAlpha) { m_fAlpha = fAlpha; } // 구분을 위해서 Child를 붙임
	virtual void Set_ParentsAlpha(_float fAlpha); // 자식에게 부모의 Alpha값을 입힌다.

	void Set_ChildActive(_bool bActive);
	void Set_ChildPosition(_float3 vPosition);
	void Set_ElementalTypeToChild(ELEMENTAL_TYPE eElementalType); // 자식에게도 플레이어 정보를 공유하기 위한 함수
	void Set_PlayerTypeToChild(CHARACTER_TYPE eCharacterType);

	virtual void Set_CharacterType(CHARACTER_TYPE eType) { m_eCurPlayerType = eType; }
	virtual void Set_ElementalType(ELEMENTAL_TYPE eType) { m_eElementalType = eType; }

public:
	virtual HRESULT Initialize_Prototype() override { return S_OK; }
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Make_Child(_float fX, _float fY, _float fCX, _float fCY, const wstring& strPrototypeTag,
		const wstring& strObjectTag = TEXT(""), void* pChildArg = nullptr);
	CUI* Get_Child(const wstring& strPrototypeTag);
	void Delete_AllChild();

public:
	void Set_Text(const wstring& strText) { m_strText = strText; }

protected:
	virtual void On_MouseEnter(_float fTimeDelta) {}
	virtual void On_Mouse(_float fTimeDelta);
	virtual void On_MouseExit(_float fTimeDelta);
	virtual void On_MouseDragEnter(_float fTimeDelta) {}
	virtual void On_MouseDrag(_float fTimeDelta) {}
	virtual void On_MouseDragExit(_float fTimeDelta) {}

	virtual void Debug_Input(_float fTimeDelta);

protected:
	virtual HRESULT Ready_Components();

protected:
	class CShader* m_pShaderCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;
	class CTexture* m_pTextureCom = nullptr;
	class CVIBuffer_Rect* m_pVIBufferCom = nullptr;

protected:
	CHARACTER_TYPE m_eCurPlayerType = { CHARACTER_TYPE::SWORD_MAN };
	ELEMENTAL_TYPE m_eElementalType = { ELEMENTAL_TYPE::FIRE };

	list<CUI*> m_pChild;

	UI_INFO m_tInfo = { 0 };
	UI_MOUSESTATE m_eMouseState = UI_MOUSESTATE::MOUSE_END;

	_float4x4 m_ViewMatrix;
	_float4x4 m_ProjMatrix;

	wstring m_strText = L"";

	_float m_fAlpha = { 1.f };
	_bool m_bActive = { true };
	_bool m_bEvent = { false };
	_bool m_bUseMouse = { false };
	_float m_fEventTimeAcc = { 0.f };

public:
	virtual void Free() override;
};
END
