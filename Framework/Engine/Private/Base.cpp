#include "..\Public\Base.h"

CBase::CBase()
{
	int			iData = 10;

}

unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;	
}

unsigned long CBase::Release()
{
	/* 삭제하낟. */
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
	{
		/* 감소하낟. */
		if (1 < m_dwRefCnt--)
			Free_Clone();

		return m_dwRefCnt;
	}
}

void CBase::Free_Clone()
{
	// 복사본 객체가 삭제될 때 호출된다.
}

void CBase::Free()
{
}
