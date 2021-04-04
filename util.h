#pragma once
#include <string>
#include <iostream>
#include <vector>

std::wstring getUserInputLine(std::wistream &stream = std::wcin);
bool getUserYesNo();
std::wstring toUpper(std::wstring wstr);
std::wstring toLower(std::wstring wstr);
std::wstring indent(std::wstring wstr, int numTabs);
bool isYes(std::wstring wstr);

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