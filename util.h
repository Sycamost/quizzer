#pragma once
#include <string>
#include <iostream>

std::wstring getUserInputLine(std::wistream &stream = std::wcin);
bool getUserYesNo();
std::wstring toUpper(std::wstring wstr);
std::wstring toLower(std::wstring wstr);
bool isYes(std::wstring wstr);
template<typename T> bool shareAnyElems(std::vector<T> v1, std::vector<T> v2);