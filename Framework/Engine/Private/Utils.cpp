#include "../Public/Utils.h"
#include <random>
#include <tchar.h>

bool CUtils::StartsWith(string str, string comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

bool CUtils::StartsWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && index == 0)
		return true;

	return false;
}

void CUtils::Replace(OUT string& str, string comp, string rep)
{
	string temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

void CUtils::Replace(OUT wstring& str, wstring comp, wstring rep)
{
	wstring temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

Matrix CUtils::To_RightHanded(Matrix matLeftHanded)
{
	Matrix matrix = Matrix::Identity;
	matrix.Forward(Vec3(0.f, 0.f, -1.f));

	return matrix * matLeftHanded;;
}

Matrix CUtils::To_LeftHanded(Matrix matRightHanded)
{
	Matrix matrix = Matrix::Identity;
	matrix.Forward(Vec3(0.f, 0.f, -1.f));

	return matrix * matRightHanded;;
}

Vec4 CUtils::To_RightHanded(Vec4 vLeftHanded)
{
	Matrix matrix = Matrix::Identity;
	matrix.Forward(Vec3(0.f, 0.f, -1.f));

	Vec4 vTransformVector = XMVector3TransformCoord(vLeftHanded, matrix);
	return vTransformVector;
}

Vec4 CUtils::To_LeftHanded(Vec4 vRightHanded)
{
	Matrix matrix = Matrix::Identity;
	matrix.Forward(Vec3(0.f, 0.f, -1.f));

	Vec4 vTransformVector = XMVector3TransformCoord(vRightHanded, matrix);
	return vTransformVector;
}

Vec3 CUtils::To_RightHanded(Vec3 vLeftHanded)
{
	Matrix matrix = Matrix::Identity;
	matrix.Forward(Vec3(0.f, 0.f, -1.f));

	Vec3 vTransformVector = XMVector3TransformCoord(vLeftHanded, matrix);
	return vTransformVector;
}

Vec3 CUtils::To_LeftHanded(Vec3 vRightHanded)
{
	Matrix matrix = Matrix::Identity;
	matrix.Forward(Vec3(0.f, 0.f, -1.f));

	Vec3 vTransformVector = XMVector3TransformCoord(vRightHanded, matrix);
	return vTransformVector;
}

PxTransform CUtils::To_PxTransform(Matrix matrix)
{
	Vec3 vScale, vPos;
	Quaternion vQuat;

	matrix.Decompose(vScale, vQuat, vPos);


	return PxTransform(PxVec3(vPos.x, vPos.y, vPos.z), PxQuat(vQuat.x, vQuat.y, vQuat.z, vQuat.w));
}

Matrix CUtils::To_Matrix(PxTransform pxTransform)
{
	Matrix result = Matrix::Identity;
	result.Right(Vec3(pxTransform.q.getBasisVector0().x, pxTransform.q.getBasisVector0().y, pxTransform.q.getBasisVector0().z));
	result.Up(Vec3(pxTransform.q.getBasisVector1().x, pxTransform.q.getBasisVector1().y, pxTransform.q.getBasisVector1().z));
	result.Forward(Vec3(pxTransform.q.getBasisVector2().x, pxTransform.q.getBasisVector2().y, pxTransform.q.getBasisVector2().z));
	result.Translation(Vec3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z));

	return result;
}

wstring CUtils::TCharToWString(TCHAR* value)
{
	if (nullptr == value)
		return wstring();

	return std::wstring(value, value + static_cast<int>(_tcslen(value)));
}

string CUtils::TCharToString(TCHAR* value)
{
	if (nullptr == value)
		return string();

	wstring wstr = TCharToWString(value);

	return ToString(wstr);
}

_bool CUtils::Equal_TChar_Char(TCHAR* val1, const char* val2)
{
	size_t str1Len = strlen(val2) + 1;
	wchar_t* wstr1 = new wchar_t[str1Len];
	size_t convertedChars;
	mbstowcs_s(&convertedChars, wstr1, str1Len, val2, _TRUNCATE);

	int result = _tcscmp(wstr1, val1);

	delete[] wstr1;

	return result == 0;
}

char* CUtils::WCharToChar(const wchar_t* value)
{
	/* 사용후 반드시 메모리 해제 필요 */

	int strSize		= WideCharToMultiByte(CP_ACP, 0, value, -1, NULL, 0, NULL, NULL);
	char* pStr		= new char[WideCharToMultiByte(CP_ACP, 0, value, -1, NULL, 0, NULL, NULL)];
	
	WideCharToMultiByte(CP_ACP, 0, value, -1, pStr, strSize, 0, 0);

	return pStr;
}

const char** CUtils::Get_WStrings_To_ConstChar(const wstring* wstrArray, const _uint iSize)
{
	const char** items = new const char* [iSize];

	for (size_t i = 0; i < iSize; i++)
	{
		std::wstring wideStr = wstrArray[i];
		std::string narrowStr(wideStr.begin(), wideStr.end());

		items[i] = _strdup(narrowStr.c_str());
	}

	return items;
}

void CUtils::Release_WString_To_ConstChar(const char** szArray, const _uint iSize)
{
	for (int n = 0; n < iSize; n++)
		free((void*)szArray[n]);

	delete[] szArray;
}

_bool CUtils::Is_Compare(const char* szLeft, const char* szRight)
{
	if (nullptr == szLeft || nullptr == szRight)
		return false;


	return !strcmp(szLeft, szRight);
}

filesystem::path CUtils::FindFile(const filesystem::path& currentPath, const std::string& targetFileName)
{
	for (const auto& entry : filesystem::directory_iterator(currentPath))
	{
		if (filesystem::is_directory(entry.status()))
		{
			filesystem::path foundPath = FindFile(entry.path(), targetFileName);
			if (!foundPath.empty()) {
				return foundPath;
			}
		}
		else if (filesystem::is_regular_file(entry.status()) && entry.path().filename() == targetFileName) {
			return entry.path();
		}
	}

	return "";
}

filesystem::path CUtils::RemovePrefix(const filesystem::path& fullPath, const filesystem::path& prefix)
{
	auto relativePath = fullPath.lexically_relative(prefix);

	// 반환된 경로의 시작이 "..\\"일 경우, 상위 디렉토리로 이동하는 부분을 제거
	while (relativePath.has_root_directory() && relativePath.root_directory() == "..\\") 
	{
		relativePath = relativePath.lexically_normal();
	}

	return relativePath;
}

wstring CUtils::PathToWString(wstring strPath)
{
	Replace(strPath, L"\\", L"/");
	
	return strPath;
}

_float3 CUtils::ToEulerAngles(_vector Quaternion)
{
	
	_float3 vAngles;
	ZeroMemory(&vAngles, sizeof(_float3));

	_float4 vQuaternion;

	XMStoreFloat4(&vQuaternion, Quaternion);


	// roll (x-axis rotation)
	_float sinr_cosp = 2 * (vQuaternion.w * vQuaternion.x + vQuaternion.y * vQuaternion.z);
	_float cosr_cosp = 1 - 2 * (vQuaternion.x * vQuaternion.x + vQuaternion.y * vQuaternion.y);
	vAngles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	_float sinp = std::sqrt(1 + 2 * (vQuaternion.w * vQuaternion.y - vQuaternion.x * vQuaternion.z));
	_float cosp = std::sqrt(1 - 2 * (vQuaternion.w * vQuaternion.y - vQuaternion.x * vQuaternion.z));
	vAngles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	_float siny_cosp = 2 * (vQuaternion.w * vQuaternion.z + vQuaternion.x * vQuaternion.y);
	_float cosy_cosp = 1 - 2 * (vQuaternion.y * vQuaternion.y + vQuaternion.z * vQuaternion.z);
	vAngles.z = std::atan2(siny_cosp, cosy_cosp);

	return vAngles;
	
}

_int CUtils::Random_Int(_int fMin, _int fMax)
{
	_int iRealMin = min(fMin, fMax);
	_int iRealMax = max(fMin, fMax);


	std::random_device Random_Device;

	std::mt19937 Generater(Random_Device());

	std::uniform_int_distribution<_int> Distribution(iRealMin, iRealMax);
	return Distribution(Generater);
}

_float CUtils::Random_Float(_float fMin, _float fMax)
{	
	_float fRealMin = min(fMin, fMax);
	_float fRealMax = max(fMin, fMax);

	std::random_device Random_Device;

	std::mt19937 Generater(Random_Device());

	std::uniform_real_distribution<_float> Distribution(fRealMin, fRealMax);
	return Distribution(Generater);
}

wstring CUtils::PopEof_WString(const wstring& str)
{
	wstring strName = L"";

	size_t NamePosition = str.find_last_of(L'\0');

	if (NamePosition != std::wstring::npos && NamePosition > 0)
		strName = str.substr(0, NamePosition);

	return strName;
}

wstring CUtils::Utf8_To_Wstring(const string& strUtf8)
{
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, nullptr, 0);
	if (requiredSize == 0) {
		MSG_BOX("Fail UTF-8 To Wstring");
		return L"";
	}

	std::wstring strUtf16(requiredSize, L'\0');
	if (MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, &strUtf16[0], requiredSize) == 0) {
		MSG_BOX("Fail UTF-8 To Wstring");
		return L"";
	}

	return strUtf16;
}

std::wstring CUtils::ToWString(string value)
{
	return wstring(value.begin(), value.end());
}

std::string CUtils::ToString(wstring value)
{
	return string(value.begin(), value.end());
}