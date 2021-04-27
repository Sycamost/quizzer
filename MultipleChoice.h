#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include "Question.h"

class MultipleChoice : public Question
{
private:
	std::wstring _question;
	easy_list::list<std::wstring> _answers;
	size_t _correctAnswerIndex;
	void writeChildData(std::wofstream& stream);
	void shuffle();
public:
	MultipleChoice(std::wstring question, std::wstring correctAnswer, easy_list::list<std::wstring> wrongAnswers, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	std::wstring getQuestion();
	std::wstring getAnswer();
	bool isCorrect(std::wstring guess);
};