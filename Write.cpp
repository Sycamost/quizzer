#include <algorithm>
#include <iostream>
#include "Question.h"
#include "QuestionWriter.h"
#include "Write.h"
#include "Command.h"
#include "Handlers.h"
#include "util.h"

QuestionTypeInfo Write::_typeInfo{ QuestionTypeInfo::getList()->front() };
easy_list::list<Question*> Write::_newQuestions{ easy_list::list<Question*>() };

void Write::nextQuestion()
{
	Question* question = _typeInfo.getWriter()->constructCurrent();
	if (question == nullptr)
		std::wcout << L"Sorry, that question couldn't be constructed properly for some reason.\n";
	else
		_newQuestions.push_back(question);

	std::wstring message =
		L"Would you like to write another "
		+ _typeInfo.getDisplaySingular()
		+ L"?";

	if (inputYesNo(L"Would you like to write another " + _typeInfo.getDisplaySingular() + L"?"))
	{
		_typeInfo.getWriter()->writeQuestion();
		return;
	}

	finishWriting();
	return;
}

DEFINE_CMD_FUNC(Write::cmdFuncWrite) {

	if (args.size() == 0)
		return CommandHandlerReturns::TOO_FEW_ARGS;

	auto questionTypeInfo = QuestionTypeInfo::get(args[0]);
	if (questionTypeInfo == QuestionTypeInfo::getList()->npos())
		return CommandHandlerReturns::INVALID_ARGS;

	Write::startWriting(*questionTypeInfo);
	return CommandHandlerReturns::SUCCESS;
}

DEFINE_CMD_FUNC(Write::cmdFuncCancel) {
	std::wcout << L"\n";
	if (inputYesNo(L"Are you sure you want to cancel writing the current question?"))
	{
		nextQuestion();
		return CommandHandlerReturns::SUCCESS;
	}
	_typeInfo.getWriter()->resetLastStep();
	return CommandHandlerReturns::SUCCESS;
}

void Write::startWriting(const QuestionTypeInfo qti)
{
	std::wcout << "Writing new " << qti.getDisplayPlural() << L"...\n\n";
	_typeInfo = qti;
	_typeInfo.getWriter()->writeQuestion();
}

easy_list::list<Question*> Write::writeToFile()
{
	std::wofstream file;
	try
	{
		file.open(_typeInfo.getFileAddress(), std::ios::app);
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

void Write::finishWriting()
{
	std::wcout << L"\nFinished writing new " << _typeInfo.getDisplayPlural() << L".\n";
	QuestionList::append(writeToFile());
	setHandlingDefault();
}
