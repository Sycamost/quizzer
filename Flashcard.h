#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Question.h"

class Flashcard : public Question
{
private:
	std::wstring _question;
	std::wstring _answer;
	bool _caseSensitive;
	static const std::wstring _optCaseSensitive;
	void writeChildData(std::wofstream& stream);
public:
	Flashcard(std::wstring question, std::wstring answer, bool caseSensitive, std::vector<std::wstring> tags = std::vector<std::wstring>());
	std::wstring getQuestion();
	std::wstring getAnswer();
	bool isCorrect(std::wstring guess);
	bool isCaseSensitive();
	void setCaseSensitive();
	void setCaseInsensitive();
	static Question* readFlashcard(std::wifstream& stream);
};