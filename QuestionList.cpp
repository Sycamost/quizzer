#include <vector>
#include "QuestionList.h"

std::vector<Question*> questionList = std::vector<Question*>();

const std::vector<Question*> getQuestionList()
{
	return questionList;
}

void appendQuestionToList(Question* question)
{
	questionList.push_back(question);
}

void appendQuestionsToList(std::vector<Question*> questions)
{
	questionList.insert(questionList.end(), questions.begin(), questions.end());
}

void clearQuestionList()
{
	questionList = std::vector<Question*>();
}