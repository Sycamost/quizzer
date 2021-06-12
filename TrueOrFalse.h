#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include "Question.h"

class TrueOrFalse : public Question
{
private:
	std::wstring _prompt;
	bool _isTrue;
	void writeChildData(std::wofstream& stream);
public:
	TrueOrFalse(std::wstring prompt, bool isTrue, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	std::wstring getQuestion();
	std::wstring getAnswer();
	bool isCorrect(std::wstring guess);
};