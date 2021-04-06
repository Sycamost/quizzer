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
public:
	Flashcard(std::wstring question, std::wstring answer, std::vector<std::wstring> tags = std::vector<std::wstring>());
	std::wstring getQuestion();
	std::wstring getAnswer();
	bool isCorrect(std::wstring guess);
	void write(std::wofstream& stream);
	bool isCaseSensitive();
	void setCaseSensitive();
	void setCaseInsensitive();
	static std::vector<Question*> readFlashcardList();
};