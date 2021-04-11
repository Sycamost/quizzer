#include <vector>
#include "QuestionList.h"
#include "Question.h"

namespace QuestionList
{
	std::vector<Question*> questionList = std::vector<Question*>();
}

const std::vector<Question*> QuestionList::get()
{
	return questionList;
}

void QuestionList::append(Question* question)
{
	questionList.push_back(question);
}

void QuestionList::append(std::vector<Question*> questions)
{
	questionList.insert(questionList.end(), questions.begin(), questions.end());
}

void QuestionList::clear()
{
	questionList = std::vector<Question*>();
}