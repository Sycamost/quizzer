#pragma once
#include <fstream>
#include <easy_list.h>
#include "Question.h"
#include "QuestionType.h"
#include "QuestionList.h"
#include "Option.h"

class QuestionReader
{
public:
	QuestionReader(
		void(*clearChildData)(),
		void(*readChildData)(std::wstring line),
		Question*(*constructCurrent)(easy_list::list<Option> options, std::vector<std::wstring> tags));
	Question* read(std::wifstream& stream);
private:
	enum class Stage {
		CHILD_DATA,
		TAGS
	};
	void (*_clearChildData)();
	void (*_readChildData)(std::wstring line);
	Question* (*_constructCurrent)(easy_list::list<Option> options, std::vector<std::wstring> tags);
};