#include "..\Public\Animation.h"
#include "Channel.h"
#include "Model.h"
#include "HierarchyNode.h"
#include "Shader.h"
#include "Utils.h"
#include "GameInstance.h"

#include "GameObject.h"

#include "Collider.h"
#include "Collider_OBB.h"
#include "Camera.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_fDuration(rhs.m_fDuration)
	, m_Channels(rhs.m_Channels)
	, m_iNumChannels(rhs.m_iNumChannels)
	, m_fTickPerSecond(rhs.m_fTickPerSecond)
	, m_strName(rhs.m_strName)
	, m_fOriginSpeed(rhs.m_fOriginSpeed)
	, m_bLoop(rhs.m_bLoop)
	, m_SpeedDescs(rhs.m_SpeedDescs)
	, m_SoundEvents(rhs.m_SoundEvents)
	, m_ColliderEvents(rhs.m_ColliderEvents)
	, m_CameraEvents(rhs.m_CameraEvents)
{
	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);
}

HRESULT CAnimation::Initialize_Prototype(aiAnimation* pAIAnimation)
{
	m_strName = CUtils::ToWString(pAIAnimation->mName.C_Str());
	m_fDuration = pAIAnimation->mDuration;
	m_fTickPerSecond = pAIAnimation->mTicksPerSecond;

	m_iNumChannels = pAIAnimation->mNumChannels;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i]);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	return S_OK;
}

HRESULT CAnimation::Initialize(CModel* pModel)
{
	m_pModel = pModel;
	
	//if (99 != GI->Get_CurrentLevel()) return S_OK;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_ChannelKeyFrames.push_back(0);

		CHierarchyNode* pNode = pModel->Get_HierarchyNode(m_Channels[i]->Get_Name());

		if (nullptr == pNode)
			return E_FAIL;

		m_HierarchyNodes.push_back(pNode);
		Safe_AddRef(pNode);
	}

	return S_OK;
}

void CAnimation::Update_Animation_Data(_float fTickPerSecond, const TWEEN_DESC& tDesc)
{
	Update_Animation_Speed(fTickPerSecond, tDesc);
	Update_Animation_Event(fTickPerSecond, tDesc);
}

void CAnimation::Clear_AnimationEvent()
{
	/* 사운드 이벤트 초기화 */
	for (auto& SoundEvent : m_SoundEvents)
		SoundEvent.second.bExecuted = false;

	/* 콜라이더 이벤트 초기화 */
	for (auto& ColliderEvent : m_ColliderEvents)
	{
		ColliderEvent.second.bExecuted = false;

		/* 어택 타입의 경우 트위닝시 종료 프레임에 도달하지 않으면 안꺼지는 것을 방지하기 위해 강제로 꺼준다. */
		if (CCollider::DETECTION_TYPE::ATTACK == ColliderEvent.second.iDetectionType)
		{
			vector<class CCollider*>& pColliders = m_pModel->Get_Owner()->Get_Collider(ColliderEvent.second.iDetectionType);

			for (size_t i = 0; i < pColliders.size(); i++)
			{
				pColliders[i]->Set_Active(false);
			}
		}
	}

	/* 카메라 이벤트 초기화 */
	for (auto& CameraEvent : m_CameraEvents)
		CameraEvent.second.bTag1 = false;
}

void CAnimation::Clear_AnimationSpeed()
{
	/* 키프레임별 속도 조절 관련 */
	m_iCurSpeedDescIndex = 0;
	m_fCurSpeedDescEndFrame = 0.f;

	m_tLiveSpeedDesc.fCurValue = m_fOriginSpeed;
	m_tLiveSpeedDesc.bActive = false;
}

_float CAnimation::Get_LiveSpeed()
{
	if(m_SpeedDescs.empty())
		return m_fOriginSpeed;

	if(m_tLiveSpeedDesc.fCurValue <= 0.01f)
		return m_fOriginSpeed;

	return m_tLiveSpeedDesc.fCurValue;
}

void CAnimation::Add_SpeedDesc(ANIM_SPEED_DESC desc)
{
	m_SpeedDescs.push_back(desc);

	/* Sorting */

	std::sort(m_SpeedDescs.begin(), m_SpeedDescs.end(),
		[](const ANIM_SPEED_DESC& desc1, const ANIM_SPEED_DESC& desc2)
		{
			if (desc1.fStartFrame < desc2.fStartFrame)
				return true;
			else if (desc1.fStartFrame > desc2.fStartFrame)
				return false;
			return desc1.fEndFrame < desc2.fEndFrame;
		});

	Sort_SpeedDesces();
}

void CAnimation::Delete_SpeedDesc(const _uint& iIndex)
{
	if (m_SpeedDescs.size() <= iIndex)
		return;

	_int iCount = 0;
	for (vector<ANIM_SPEED_DESC>::iterator iter = m_SpeedDescs.begin(); iter != m_SpeedDescs.end();)
	{
		if (iIndex == iCount)
		{
			m_SpeedDescs.erase(iter);
			break;
		}
		else
		{
			++iter;
			++iCount;
		}
	}

	Sort_SpeedDesces();
}

void CAnimation::Change_SpeedDesc(const _uint& iIndex, const Vec4& vDesc)
{
	if (m_SpeedDescs.size() <= iIndex)
		return;

	m_SpeedDescs[iIndex].fStartFrame = vDesc.x;
	m_SpeedDescs[iIndex].fEndFrame = vDesc.y;
	m_SpeedDescs[iIndex].fStartSpeed = vDesc.z;
	m_SpeedDescs[iIndex].fEndSpeed = vDesc.w;
}

void CAnimation::Sort_SpeedDesces()
{
	std::sort(m_SpeedDescs.begin(), m_SpeedDescs.end(),
		[](const ANIM_SPEED_DESC& desc1, const ANIM_SPEED_DESC& desc2)
		{
			if (desc1.fStartFrame < desc2.fStartFrame)
				return true;
			else if (desc1.fStartFrame > desc2.fStartFrame)
				return false;
			return desc1.fEndFrame < desc2.fEndFrame;
		});

	m_iCurSpeedDescIndex = 0;
}

void CAnimation::Change_EventKeyFrame(const _uint& iIndex, const _float fFrame, const ANIM_EVENT_TYPE& eEventType)
{
	switch (eEventType)
	{
	case Engine::SOUND:
	{
		if (m_SoundEvents.size() <= iIndex)
			return;

		m_SoundEvents[iIndex].first = fFrame;
	}
		break;
	case Engine::EFFECT:
	{

	}
		break;
	case Engine::CAMERA:
	{
		if (m_CameraEvents.size() <= iIndex)
			return;

		m_CameraEvents[iIndex].first = fFrame;
	}
		break;
	case Engine::COLLIDER:
	{
		if (m_ColliderEvents.size() <= iIndex)
			return;

		m_ColliderEvents[iIndex].first = fFrame;
	}
		break;
	case Engine::ANIM_EVENT_TYPE_END:
		break;
	default:
		break;
	}
}

void CAnimation::Add_SoundEvent(const _float& fFrame, const ANIM_EVENT_SOUND_DESC& desc)
{
	m_SoundEvents.push_back(pair(fFrame, desc));

	Sort_SoundEvents();
}

void CAnimation::Del_SoundEvent(const _uint iIndex)
{
	if (m_SoundEvents.size() <= iIndex)
		return;

	_int iCount = 0;
	for (vector<pair<_float, ANIM_EVENT_SOUND_DESC>>::iterator iter = m_SoundEvents.begin(); iter != m_SoundEvents.end();)
	{
		if (iIndex == iCount)
		{
			m_SoundEvents.erase(iter);
			break;
		}
		else
		{
			++iter;
			++iCount;
		}
	}

	Sort_SoundEvents();
}

void CAnimation::Del_All_SoundEvent()
{
	m_SoundEvents.clear();
	m_SoundEvents.shrink_to_fit();
}

void CAnimation::Change_SoundEvent(const _uint iIndex, const ANIM_EVENT_SOUND_DESC& desc)
{
	if (m_SoundEvents.size() <= iIndex)
		return;

	m_SoundEvents[iIndex].second.strSoundKey = desc.strSoundKey;
	m_SoundEvents[iIndex].second.iChannelID = desc.iChannelID;
	m_SoundEvents[iIndex].second.fVolume = desc.fVolume;
	m_SoundEvents[iIndex].second.bStop = desc.bStop;
}

void CAnimation::Sort_SoundEvents()
{
	if (m_SoundEvents.empty())
		return;

	std::sort(m_SoundEvents.begin(), m_SoundEvents.end(),
		[](const auto& lhs, const auto& rhs) {
			return lhs.first < rhs.first;
		});
}

void CAnimation::Add_ColliderEvent(const _float& fFrame, const ANIM_EVENT_COLLIDER_DESC& desc)
{
	m_ColliderEvents.push_back(pair(fFrame, desc));

	Sort_ColliderEvents();
}

void CAnimation::Del_ColliderEvent(const _uint iIndex)
{
	if (m_ColliderEvents.size() <= iIndex)
		return;

	_int iCount = 0;
	for (vector<pair<_float, ANIM_EVENT_COLLIDER_DESC>>::iterator iter = m_ColliderEvents.begin(); iter != m_ColliderEvents.end();)
	{
		if (iIndex == iCount)
		{
			m_ColliderEvents.erase(iter);
			break;
		}
		else
		{
			++iter;
			++iCount;
		}
	}

	Sort_ColliderEvents();
}

void CAnimation::Del_All_ColliderEvent()
{
	m_ColliderEvents.clear();
	m_ColliderEvents.shrink_to_fit();
}

void CAnimation::Change_ColliderEvent(const _uint iIndex, const ANIM_EVENT_COLLIDER_DESC& desc)
{
	if (m_ColliderEvents.size() <= iIndex)
		return;

	m_ColliderEvents[iIndex].second.bOnOff = desc.bOnOff;
	m_ColliderEvents[iIndex].second.vOffset = desc.vOffset;
	m_ColliderEvents[iIndex].second.vExtents = desc.vExtents;
	m_ColliderEvents[iIndex].second.iDetectionType = desc.iDetectionType;
	m_ColliderEvents[iIndex].second.iAttackType = desc.iAttackType;
}

void CAnimation::Sort_ColliderEvents()
{
	if (m_ColliderEvents.empty())
		return;

	std::sort(m_ColliderEvents.begin(), m_ColliderEvents.end(),
		[](const auto& lhs, const auto& rhs) {
			return lhs.first < rhs.first;
		});
}

void CAnimation::Add_CameraEvent(const _float& fFrame, const CAMERA_EVENT_DESC& desc)
{
	m_CameraEvents.push_back(pair(fFrame, desc));

	Sort_CameraEvents();
}

void CAnimation::Del_CameraEvent(const _uint iIndex)
{
	if (m_CameraEvents.size() <= iIndex)
		return;

	_int iCount = 0;
	for (vector<pair<_float, CAMERA_EVENT_DESC>>::iterator iter = m_CameraEvents.begin(); iter != m_CameraEvents.end();)
	{
		if (iIndex == iCount)
		{
			m_CameraEvents.erase(iter);
			break;
		}
		else
		{
			++iter;
			++iCount;
		}
	}

	Sort_CameraEvents();
}

void CAnimation::Del_All_CameraEvent()
{
	m_CameraEvents.clear();
	m_CameraEvents.shrink_to_fit();
}

void CAnimation::Change_CameraEvent(const _uint iIndex, const CAMERA_EVENT_DESC& desc)
{
	if (m_CameraEvents.size() <= iIndex)
		return;

	m_CameraEvents[iIndex].second.fTag1 = desc.fTag1;
	m_CameraEvents[iIndex].second.fTag2 = desc.fTag2;
	m_CameraEvents[iIndex].second.fTag3 = desc.fTag3;

	m_CameraEvents[iIndex].second.iTag1 = desc.iTag1;
	m_CameraEvents[iIndex].second.iTag2 = desc.iTag2;
}

void CAnimation::Sort_CameraEvents()
{
	if (m_CameraEvents.empty())
		return;

	std::sort(m_CameraEvents.begin(), m_CameraEvents.end(),
		[](const auto& lhs, const auto& rhs) {
			return lhs.first < rhs.first;
		});
}

CChannel* CAnimation::Get_Channel(const wstring& strChannelName)
{
	for (auto& iter : m_Channels)
	{
		if (strChannelName == iter->Get_Name())
			return iter;
	}

	return nullptr;
}

HRESULT CAnimation::Calculate_Animation(const _uint& iFrame)
{
	for (auto& pChannel : m_Channels)
	{
		for (auto& iCurrentKeyFrame : m_ChannelKeyFrames)
			iCurrentKeyFrame = 0;
	}

	_uint iChannelIndex = 0;
	for (auto& pChannel : m_Channels)
	{
		m_ChannelKeyFrames[iChannelIndex] = pChannel->Update_Transformation(iFrame, m_ChannelKeyFrames[iChannelIndex], m_HierarchyNodes[iChannelIndex]);
		++iChannelIndex;
	}

	return S_OK;
}

HRESULT CAnimation::Clear_Channels()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

	// 툴레벨이 아니라면 어차피 하이어러키 내부 데이터는 없음 
	
	//for (auto& pHierarchyNode : m_HierarchyNodes)
	//	Safe_Release(pHierarchyNode);

	//m_HierarchyNodes.clear();

	return S_OK;
}

void CAnimation::Update_Animation_Speed(_float fTickPerSecond, const TWEEN_DESC& tDesc)
{
	/* Checke Speed */
	if (m_SpeedDescs.empty())
		return;

	if (m_tLiveSpeedDesc.bActive)
		m_tLiveSpeedDesc.Update(fTickPerSecond);

	if (m_tLiveSpeedDesc.fCurValue < 0.1f)
		int k = 0; 

	/* 트윈중이라면 다음 애니메이션 키프레임 기준으로 동작하도록 한다. */
	const _float fCurFrame = (0 <= tDesc.next.iAnimIndex) ? tDesc.next.iCurFrame : tDesc.cur.iCurFrame;

	for (_uint i = m_iCurSpeedDescIndex; i < m_SpeedDescs.size(); i++)
	{
		if (m_SpeedDescs[i].fStartFrame < fCurFrame && !m_tLiveSpeedDesc.bActive)
		{
			if (abs(m_tLiveSpeedDesc.fCurValue - m_SpeedDescs[i].fEndSpeed) < 0.05f)
				continue;

			if (m_SpeedDescs[m_iCurSpeedDescIndex].fStartFrame <= 0.f &&
				m_SpeedDescs[m_iCurSpeedDescIndex].fEndFrame <= 0.f &&
				m_SpeedDescs[m_iCurSpeedDescIndex].fStartSpeed <= 0.f &&
				m_SpeedDescs[m_iCurSpeedDescIndex].fEndSpeed <= 0.f)
				continue;

			if (m_SpeedDescs[m_iCurSpeedDescIndex].fStartFrame >= m_SpeedDescs[m_iCurSpeedDescIndex].fEndFrame)
				continue;

			m_iCurSpeedDescIndex = i;

			m_tLiveSpeedDesc.Start(
				m_SpeedDescs[m_iCurSpeedDescIndex].fStartSpeed,
				m_SpeedDescs[m_iCurSpeedDescIndex].fEndSpeed,
				Calculate_LerpTime(m_SpeedDescs[m_iCurSpeedDescIndex], tDesc, fTickPerSecond),
				LERP_MODE::SMOOTH_STEP);
		}
	}
}   

void CAnimation::Update_Animation_Event(_float fTickPerSecond, const TWEEN_DESC& tDesc)
{
	if (nullptr == m_pModel || nullptr == m_pModel->Get_Owner())
		return;

	/* 트윈중이라면 다음 애니메이션 키프레임 기준으로 동작하도록 한다. */
	const _float fCurFrame = (0 <= tDesc.next.iAnimIndex) ? tDesc.next.iCurFrame : tDesc.cur.iCurFrame;

	/* 사운드 이벤트 */
	
	const _float fCamDist = m_pModel->Get_Owner()->Get_CamDistance();

	for (auto& SoundEvent : m_SoundEvents)
	{
		if (SoundEvent.first <= fCurFrame && !SoundEvent.second.bExecuted)
		{
			SoundEvent.second.bExecuted = true;
			 
			GI->Play_Sound(CUtils::ToWString(SoundEvent.second.strSoundKey), 
				CHANNELID(SoundEvent.second.iChannelID), 
				SoundEvent.second.fVolume, 
				SoundEvent.second.bStop,
				fCamDist);
		}
	}

	/* 콜라이더 이벤트 */
	for (auto& ColliderEvent : m_ColliderEvents)
	{
		if (ColliderEvent.first <= fCurFrame && !ColliderEvent.second.bExecuted)
		{
			ColliderEvent.second.bExecuted = true;

			if (ColliderEvent.second.bOnOff)
			{
				if (!m_pModel->Get_Owner()->Get_Collider(ColliderEvent.second.iDetectionType).empty())
				{
					CCollider* pCollider = m_pModel->Get_Owner()->Get_Collider(ColliderEvent.second.iDetectionType).front();

					pCollider->Set_Active(ColliderEvent.second.bOnOff);
					pCollider->Set_Offset(ColliderEvent.second.vOffset);
					pCollider->Set_Radius(ColliderEvent.second.vExtents.x);
					pCollider->Set_Extents(ColliderEvent.second.vExtents);
					pCollider->Set_AttackType(CCollider::ATTACK_TYPE(ColliderEvent.second.iAttackType));
				}
			}
			else
			{
				if (!m_pModel->Get_Owner()->Get_Collider(ColliderEvent.second.iDetectionType).empty())
				{
					m_pModel->Get_Owner()->Get_Collider(ColliderEvent.second.iDetectionType).front()->Set_Active(ColliderEvent.second.bOnOff);
				}
			}
		}
	}

	/* 카메라 이벤트 */
	for (auto& CameraEvent : m_CameraEvents)
	{
		if (CameraEvent.first <= fCurFrame && !CameraEvent.second.bTag1)
		{
			CameraEvent.second.bTag1 = true;
			
			switch (CameraEvent.second.iTag2)
			{
			case CAMERA_EVENT_TYPE::FOV :
			{
				CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Lerp_Fov(CameraEvent.second.fTag1, CameraEvent.second.fTag2, (LERP_MODE)CameraEvent.second.iTag1);
			}
				break;
			case CAMERA_EVENT_TYPE::DISTANCE :
			{
				//CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Lerp_Distance(
				//	CameraEvent.second.fTag1, CameraEvent.second.fTag2, (LERP_MODE)CameraEvent.second.iTag1);
			}
				break;
			case CAMERA_EVENT_TYPE::SHAKE :
			{
				CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(CameraEvent.second.fTag1, CameraEvent.second.fTag2, CameraEvent.second.fTag3);
			}
				break;
			default:
				break;
			}
		}
	}
}

const _float CAnimation::Calculate_LerpTime(const ANIM_SPEED_DESC tSpeedDesc, const TWEEN_DESC tTweenDesc, const _float fTickPerSecond)
{
	_float fTimeAcc = 0.f;
	_float fFrameAcc = 0.f;

	_float fCurFrame = tSpeedDesc.fStartFrame;
	_float fEndFrame = tSpeedDesc.fEndFrame;

	/* 예외 처리 조건 필수 ! */

	/* 시작 프레임에서 종료 프레임까지 디폴트 시간으로 소요되는 총 시간 */

	const _float fCurSpeed = (m_tLiveSpeedDesc.fCurValue <= 0.01f) ? m_fOriginSpeed : m_tLiveSpeedDesc.fCurValue;

	while (1)
	{
		fTimeAcc += fTickPerSecond;
		fFrameAcc += fTickPerSecond;

		const _float fTimePerFrame = 1 / (m_fTickPerSecond * fCurSpeed);

		if (fTimePerFrame <= fFrameAcc)
		{
			fCurFrame++;
			fFrameAcc = 0.f;
			if (fCurFrame == tSpeedDesc.fEndFrame)
			{
				break;
			}
		}

		/* 혹시 모를 예외 처리*/
		if (100 < fTimeAcc)
			return 0.f;
	}


	fFrameAcc = 0.f;

	fCurFrame = tSpeedDesc.fStartFrame;
	fEndFrame = tSpeedDesc.fEndFrame;

	LERP_FLOAT_DESC tLerpDesc;
	tLerpDesc.Start(
		m_SpeedDescs[m_iCurSpeedDescIndex].fStartSpeed,
		m_SpeedDescs[m_iCurSpeedDescIndex].fEndSpeed,
		fTimeAcc, LERP_MODE::SMOOTH_STEP);

	fTimeAcc = 0.f;

	/* 시작 프레임에서 종료 프레임까지 럴프 시간 소요되는 총 시간 */
	while (1)
	{
		fTimeAcc += fTickPerSecond;
		fFrameAcc += fTickPerSecond;

		const _float fTimePerFrame = 1 / (m_fTickPerSecond * tLerpDesc.fCurValue);

		if (fTimePerFrame <= fFrameAcc)
		{
			fCurFrame++;
			fFrameAcc = 0.f;
			if (fCurFrame == tSpeedDesc.fEndFrame)
			{
				return fTimeAcc;
			}
		}

		if (tLerpDesc.bActive)
		{
			tLerpDesc.Update(fTickPerSecond);
			if(!tLerpDesc.bActive)
				return fTimeAcc;
		}

		/* 혹시 모를 예외 처리*/
		if (100 < fTimeAcc)
			return 0.f;
	}
	
	
	return 0.f;
}

CAnimation* CAnimation::Create(aiAnimation* pAIAnimation)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize_Prototype(pAIAnimation)))
	{
		MSG_BOX("Failed To Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Create_Bin()
{
	CAnimation* pInstance = new CAnimation();
	return pInstance;
}

CAnimation* CAnimation::Clone(CModel* pModel)
{
	CAnimation* pInstance = new CAnimation(*this);

	if (FAILED(pInstance->Initialize(pModel)))
	{
		MSG_BOX("Failed To Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();


	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);

	m_HierarchyNodes.clear();
}
