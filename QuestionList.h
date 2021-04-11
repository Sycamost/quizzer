#pragma once
#include <vector>
#include "Question.h"

namespace QuestionList
{
	const std::vector<Question*> get();
	void append(Question* question);
	void append(std::vector<Question*> questions);
	void clear();
}