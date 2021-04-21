#pragma once
#include <fstream>
#include "Question.h"
#include "QuestionType.h"
#include "Handlers.h"

class QuestionWriter
{
public:
	QuestionWriter(
		QuestionType type,
		void(*writeChildData)(),
		bool(*resetLastChildDataStep)(),
		Question* (*constructCurrent)(easy_list::list<std::wstring> tags)
	);
	void writeQuestion();
	static void writeTags();
	easy_list::list<Question*> writeToFile();
	void resetLastStep();

private:

	static easy_list::list<std::wstring> _tags;
	static InputHandlerReturns tagsInputHandlerFunc(std::wstring input);
	static void askForTag();

	QuestionType _type;
	void(*_writeChildData)();
	bool(*_resetLastChildDataStep)();
	Question* (*_constructCurrent)(easy_list::list<std::wstring> tags);
	easy_list::list<Question*> _newQuestions;
};