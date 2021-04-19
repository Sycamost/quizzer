#pragma once
#include <vector>
#include <easy_list.h>
#include "Question.h"

namespace QuestionList
{
	const easy_list::list<Question*> get();
	void append(Question* question);
	void append(std::vector<Question*> questions);
	void clear();
}