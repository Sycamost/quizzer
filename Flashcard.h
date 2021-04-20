#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include "Question.h"

class Flashcard : public Question
{
private:
	std::wstring _question;
	std::wstring _answer;
	bool _caseSensitive;
	void writeChildData(std::wofstream& stream);
public:
	Flashcard(std::wstring question, std::wstring answer, bool caseSensitive, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	std::wstring getQuestion();
	std::wstring getAnswer();
	bool isCorrect(std::wstring guess);
	bool isCaseSensitive();
	void setCaseSensitive();
	void setCaseInsensitive();
	static Question* readFlashcard(std::wifstream& stream);
};