#include <algorithm>
#include <iostream>
#include <functional>
#include "Question.h"
#include "QuestionWriter.h"
#include "QuestionTypeInfo.h"
#include "Option.h"
#include "Command.h"
#include "Write.h"
#include "Handlers.h"
#include "util.h"

easy_list::list<std::wstring> QuestionWriter::_tags{ easy_list::list<std::wstring>() };

QuestionWriter::QuestionWriter(
	QuestionType type,
	void(*writeChildData)(),
	bool(*resetLastChildDataStep)(),
	Question* (*constructCurrent)(easy_list::list<std::wstring> tags))
	:
	_type(type),
	_writeChildData(writeChildData),
	_resetLastChildDataStep(resetLastChildDataStep),
	_constructCurrent(constructCurrent)
{}

void QuestionWriter::writeQuestion()
{
	_writeChildData();
}

void QuestionWriter::writeTags()
{
	_tags = easy_list::list<std::wstring>();
	askForTag();
}

void QuestionWriter::askForTag()
{
	std::wstring msg = L"Tag " + std::to_wstring(_tags.size() + 1) + L":\t";
	setHandling(msg, &tagsInputHandlerFunc, CommandType::CANCEL);
}

DEFINE_INPUT_HANDLER_FUNC(QuestionWriter::tagsInputHandlerFunc)
{
	if (input.empty())
	{
		Write::nextQuestion();
		return InputHandlerReturns::SUCCESS;
	}

	_tags.push_back(input);
	askForTag();
	return InputHandlerReturns::SUCCESS;
}

void QuestionWriter::resetLastStep()
{
	// Try resetting the last child data step. If that fails, reset the last tag step.
	if (!_resetLastChildDataStep())
	{
		askForTag();
	}
}

Question* QuestionWriter::constructCurrent()
{
	return _constructCurrent(_tags);
}
