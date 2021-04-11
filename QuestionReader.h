#pragma once
#include <fstream>
#include "Question.h"
#include "QuestionList.h"

class QuestionReader
{
public:
	Question* read(std::wifstream& stream);
};