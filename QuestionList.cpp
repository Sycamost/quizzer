#include <easy_list.h>
#include "QuestionList.h"
#include "Question.h"

namespace QuestionList
{
	easy_list::list<Question*> questionList = easy_list::list<Question*>();
}

const easy_list::list<Question*> QuestionList::get()
{
	return questionList;
}

void QuestionList::append(Question* question)
{
	questionList.push_back(question);
}

void QuestionList::append(easy_list::list<Question*> questions)
{
	questionList.insert(questionList.end(), questions.begin(), questions.end());
}

void QuestionList::clear()
{
	questionList = easy_list::list<Question*>();
}