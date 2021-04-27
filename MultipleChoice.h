#pragma once
#include <string>
#include <easy_list.h>
#include <fstream>
#include "Question.h"

class MultipleChoice : public Question
{
private:
	std::wstring _question;
	std::wstring _correctAnswer;
	easy_list::list<std::wstring> _wrongAnswers;
	easy_list::list<size_t> _shuffledIndexes;
	void writeChildData(std::wofstream& stream);
	void shuffle();
	std::wstring getOption(size_t unshuffledIndex);
public:
	MultipleChoice(std::wstring question, std::wstring correctAnswer, easy_list::list<std::wstring> wrongAnswers, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	std::wstring getQuestion();
	std::wstring getAnswer();
	bool isCorrect(std::wstring guess);
};