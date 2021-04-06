#pragma once
#include <vector>
#include <string>
#include <fstream>

class Question
{
private:
	static std::vector<Question*> _questionList;
protected:
	std::vector<std::wstring> _tags;
public:
	Question(std::vector<std::wstring> tags = std::vector<std::wstring>());
	virtual std::wstring getQuestion() = 0;
	virtual std::wstring getAnswer() = 0;
	virtual bool isCorrect(std::wstring guess) = 0;
	virtual void write(std::wofstream& stream) = 0;
	static std::vector<Question*> getQuestionList();
	static void readQuestionList();
	static void appendQuestionToList(Question* question);
	static void appendQuestionsToList(std::vector<Question*> questions);
	std::vector<std::wstring> getTags();
};