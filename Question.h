#pragma once
#include <easy_list.h>
#include <string>
#include <fstream>
#include <map>
#include "QuestionType.h"

class Question
{
private:
	static easy_list::list<Question*> _questionList;
protected:
	QuestionType _type;
	easy_list::list<std::wstring> _tags;
	static easy_list::list<std::wstring> readTags(std::wifstream& stream);
	virtual void writeChildData(std::wofstream& stream) = 0;
public:
	Question(QuestionType type, easy_list::list<std::wstring> tags = easy_list::list<std::wstring>());
	virtual std::wstring getQuestion() = 0;
	virtual std::wstring getAnswer() = 0;
	virtual bool isCorrect(std::wstring guess) = 0;
	void write(std::wofstream& stream);
	easy_list::list<std::wstring> getTags();
	QuestionType getType();
};