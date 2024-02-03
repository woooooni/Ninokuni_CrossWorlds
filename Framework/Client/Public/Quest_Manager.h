#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "MainQuest.h"
#include "SubQuest.h"

BEGIN(Client)

class CQuest_Manager final : public CBase
{
	DECLARE_SINGLETON(CQuest_Manager)
public:
	enum QUESTEVENT_TYPE { QUESTEVENT_FIND_ANIMAL, QUESTEVENT_MONSTER_KILL, QUESTEVENT_BOSS_KILL,
		QUESTEVENT_INVASION, QUESTEVENT_TOWERDEFENCE, QUESTEVENT_FINALBATTLE, QUESTEVENT_ENDING, QUESTEVENT_GAMEEND,
		QUESTEVENT_END };

private:
	CQuest_Manager();
	virtual ~CQuest_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);
	void    LateTick(_float fTimeDelta);

public:
	void	Set_SubQuestRunning(CSubQuest::SUBQUEST_NAME eSQName, _bool bIsRunning);

	void	Set_Running(_bool bIsRunning) { m_bIsRunning = bIsRunning; }
	_bool	Get_IsReserve() { return m_bIsReserve; }

	void			Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	CGameObject*	Get_Target() { return m_pTarget; }

	// 퀘스트 완료 스택.
	_int	Get_QuestClearStack() { return m_iQuestClearStack; }
	void    Set_QuestClearStack(_int iStack) { m_iQuestClearStack += iStack; }
	void    Clear_QuestClearStack() { m_iQuestClearStack = 0; }	

	// 퀘스트 이벤트 타입
	QUESTEVENT_TYPE Get_CurQuestEvent() { return m_eCurQuestType; }
	void			Set_CurQuestEvent(QUESTEVENT_TYPE eType) { m_eCurQuestType = eType; }

	// 몬스터 처치 관련
	_int	Get_MonsterKillCount() { return m_iMonsterKillCount; }
	void	Set_MonsterKillCount(_int iCount) { m_iMonsterKillCount += iCount; }
	void	Clear_MonsterKillCount() { m_iMonsterKillCount = 0; }

	// 보스 처치 관련
	_bool	Get_IsBossKill() { return m_bIsBossKill; }
	void	Set_IsBossKill(_bool IsBossKill) { m_bIsBossKill = IsBossKill; }

	const Vec4& Get_OriginSkyCenter() const { return m_vOriginSkyCenterColor; }
	const Vec4& Get_OriginSkyApexColor() const { return m_vOriginSkyApexColor; }

	void Set_OriginSkyCenterColor(Vec4 vColor) { m_vOriginSkyCenterColor = vColor; }
	void Set_OriginSkyApexColor(Vec4 vColor) { m_vOriginSkyApexColor = vColor; }

	void Ready_InvasionLight(const wstring& strLightFilePath);
private:
	CMainQuest*  m_pMainQuest = nullptr;
	CSubQuest*   m_pSubQuest = nullptr;

	CGameObject* m_pTarget = nullptr;
	
	_bool		m_bIsRunning = false;
	_bool		m_bIsReserve = false;

	_int		m_iQuestClearStack = 0;

	QUESTEVENT_TYPE m_eCurQuestType = QUESTEVENT_END;
	_int		m_iMonsterKillCount = 0;
	_bool		m_bIsBossKill = false;

private:
	Vec4 m_vOriginSkyCenterColor;
	Vec4 m_vOriginSkyApexColor;


	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
public:
	virtual void Free() override;
};

END

