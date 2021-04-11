#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include "QuestionType.h"

class Question
{
private:
	static std::vector<Question*> _questionList;
protected:
	QuestionType _type;
	std::vector<std::wstring> _tags;
	std::vector<std::wstring> readTags(std::wifstream& stream);
	virtual void writeChildData(std::wofstream& stream) = 0;
public:
	Question(QuestionType type, std::vector<std::wstring> tags = std::vector<std::wstring>());
	virtual std::wstring getQuestion() = 0;
	virtual std::wstring getAnswer() = 0;
	virtual bool isCorrect(std::wstring guess) = 0;
	void write(std::wofstream& stream);
	static std::vector<Question*> getQuestionList();
	static void readQuestionList();
	static void appendQuestionToList(Question* question);
	static void appendQuestionsToList(std::vector<Question*> questions);
	std::vector<std::wstring> getTags();
	QuestionType getType();
};