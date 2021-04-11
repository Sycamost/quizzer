#pragma once
#include <fstream>
#include "Question.h"
#include "QuestionType.h"
#include "QuestionList.h"
#include "Option.h"

class QuestionReader
{
public:
	QuestionReader(
		void(*readChildData)(std::wstring line),
		Question*(*constructCurrent)(std::vector<Option> options, std::vector<std::wstring> tags));
	Question* read(std::wifstream& stream);
private:
	enum class Stage {
		CHILD_DATA,
		TAGS
	};
	void (*_readChildData)(std::wstring line);
	Question* (*_constructCurrent)(std::vector<Option> options, std::vector<std::wstring> tags);
};