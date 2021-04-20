#pragma once
#include <easy_list.h>
#include "Question.h"

namespace QuestionList
{
	const easy_list::list<Question*> get();
	void append(Question* question);
	void append(easy_list::list<Question*> questions);
	void clear();
}