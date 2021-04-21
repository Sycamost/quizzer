#include <algorithm>
#include <iostream>
#include "Question.h"
#include "QuestionWriter.h"
#include "Write.h"
#include "Command.h"
#include "Handlers.h"
#include "util.h"

QuestionTypeInfo Write::_typeInfo{ QuestionTypeInfo::getList()->front() };

void Write::nextQuestion()
{
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
};

DEFINE_CMD_FUNC(Write::cmdFuncCancel) {
	std::wcout << L"\n";
	if (inputYesNo(L"Are you sure you want to cancel writing the current question?"))
	{
		_typeInfo.getWriter()->writeQuestion();
		return CommandHandlerReturns::SUCCESS;
	}
	_typeInfo.getWriter()->resetLastStep();
	return CommandHandlerReturns::SUCCESS;
};

void Write::startWriting(const QuestionTypeInfo qti)
{
	std::wcout << "Writing new " << qti.getDisplayPlural() << L"...\n\n";
	_typeInfo = qti;
	_typeInfo.getWriter()->writeQuestion();
}

void Write::finishWriting()
{
	std::wcout << L"\nFinished writing new " << _typeInfo.getDisplayPlural() << L".\n";
	QuestionList::append(_typeInfo.getWriter()->writeToFile());
	setHandlingDefault();
}
