#include <algorithm>
#include <iostream>
#include "Question.h"
#include "QuestionWriter.h"
#include "QuestionTypeInfo.h"
#include "Option.h"
#include "Command.h"
#include "Write.h"
#include "Handlers.h"
#include "util.h"

easy_list::list<std::wstring> QuestionWriter::_tags = easy_list::list<std::wstring>();

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
	askForTag();
}

void QuestionWriter::askForTag()
{
	std::wcout << L"Tag " << _tags.size() + 1 << L":\t";
	setHandling(&tagsInputHandlerFunc, CommandType::CANCEL);
}

InputHandlerReturns QuestionWriter::tagsInputHandlerFunc(std::wstring input)
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

easy_list::list<Question*> QuestionWriter::writeToFile()
{
	std::wofstream file;
	try
	{
		file.open(QuestionTypeInfo::get(_type)->getFileAddress(), std::ios::app);
		if (!file.is_open())
			throw std::exception("File did not open correctly.");
	}
	catch (std::exception e)
	{
		std::wcout << L"Oops! Something went wrong when trying to access the destination file. We got the following error message:\n"
			<< e.what() << L"\n";
		return easy_list::list<Question*>();
	}

	for (Question* question : _newQuestions)
		question->write(file);

	file.close();

	return _newQuestions;
}

void QuestionWriter::resetLastStep()
{
	// Try resetting the last child data step. If that fails, reset the last tag step.
	if (!_resetLastChildDataStep())
	{
		askForTag();
	}
}
