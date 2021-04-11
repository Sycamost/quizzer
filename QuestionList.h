#pragma once
#include <vector>
#include "Question.h"

const std::vector<Question*> getQuestionList();
void appendQuestionToList(Question* question);
void appendQuestionsToList(std::vector<Question*> questions);
void clearQuestionList();