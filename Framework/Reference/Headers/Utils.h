#pragma once

#include "Base.h"

class ENGINE_DLL CUtils : public CBase
{

public:
	static wstring ToWString(string value);
	static string ToString(wstring value);

	static wstring PathToWString(wstring strPath);
	static _float3 ToEulerAngles(_vector Quaternion);

	static _int Random_Int(_int fMin, _int fMax);
	static _float Random_Float(_float fMin, _float fMax);

	static bool StartsWith(string str, string comp);
	static bool StartsWith(wstring str, wstring comp);

	static void Replace(OUT string& str, string comp, string rep);
	static void Replace(OUT wstring& str, wstring comp, wstring rep);

	static wstring TCharToWString(TCHAR* value);
	static string TCharToString(TCHAR* value);

	static _bool Equal_TChar_Char(TCHAR* val1, const char* val2);

	static char* WCharToChar(const wchar_t* value);

	static const char** Get_WStrings_To_ConstChar(const wstring* wstrArray, const _uint iSize);
	static void Release_WString_To_ConstChar(const char** szArray, const _uint iSize);
};

