#pragma once
#include <string>

std::wstring getUserInputLine(std::wistream &stream = std::wcin);
bool getUserYesNo();
std::wstring toUpper(std::wstring wstr);
bool isYes(std::wstring wstr);