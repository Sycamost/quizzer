#pragma once
#include <string>
#include <iostream>
#include <vector>

std::wstring getInputLine(std::wistream &stream = std::wcin);
bool getUserYesNo(bool doPrintResult = true);
std::wstring toUpper(std::wstring wstr);
std::wstring toLower(std::wstring wstr);
std::wstring indent(std::wstring wstr, int numTabs);
bool isYes(std::wstring wstr);
std::vector<std::wstring> splitByWord(std::wstring wstr);

template <typename T1, typename T2> std::vector<T2> convv(std::vector<T1> v)
{
	std::vector<T2> result = std::vector<T2>();
	result.reserve(v.size());
	for (int i = 0; i < v.size(); i++)
		result.push_back((T2)v[i]);
	return result;
}

template <typename T> std::vector<T> slice(std::vector<T> v, int start = 0, int end = -1)
{
	std::vector<T> result = std::vector<T>();

	if (end < 0)
		end = v.size();

	for (int i = start; i < end; i++)
		result.push_back(v[i]);

	return result;
}

template <typename T> bool shareAnyElems(std::vector<T> v1, std::vector<T> v2)
{

	for (unsigned int i = 0; i < v1.size(); i++)
	{
		for (unsigned int j = 0; j < v2.size(); j++)
		{
			if (v1[i] == v2[j])
				return true;
		}
	}
	return false;
}