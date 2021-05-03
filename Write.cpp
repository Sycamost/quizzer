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

	setCommandHandling({ CommandType::CANCEL, CommandType::QUIT_WRITE });
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

DEFINE_CMD_FUNC(Write::cmdFuncQuitWrite)
{
	std::wcout << L"\n";
	if (inputYesNo(L"Are you sure you want to cancel writing the current question and quit the app?"))
	{
		finishWriting();
		return CommandHandlerReturns::QUIT_APP;
	}
	return CommandHandlerReturns::RESET_INPUT;
}

void Write::startWriting(const QuestionTypeInfo qti)
{
	std::wcout << "Writing new " << qti.getDisplayPlural() << L"...\n\n";
	_typeInfo = qti;
	_typeInfo.getWriter()->writeQuestion();
}

void ensureFileEndsInEmptyLine(std::string fileAddress)
{
	// We need the file to end in an empty line, which consists of two consecutive new-line characters.
	std::wfstream file(fileAddress, std::ios_base::in | std::ios_base::out | std::ios_base::app);

	auto newlinelen = 1;
	auto buflen = newlinelen + 1;
	auto newline = easy_list::list<wchar_t>(newlinelen, L'\n');
	auto buf = easy_list::list<wchar_t>(buflen, L'\0');
	auto endoff = -1;
	
	// One new-line character
	file.seekg(endoff - newlinelen, std::ios_base::end);
	file.get(buf.begin()._Ptr, buflen + 1, L'\0');
	if (buf.slice(0, newlinelen) != newline)
		file << L'\n';

	// The second new-line character
	file.seekg(endoff - newlinelen * 2, std::ios_base::end);
	file.get(buf.begin()._Ptr, buflen + 1, L'\0');
	if (buf.slice(0, newlinelen) != newline)
		file << L'\n';

	file.close();
}

easy_list::list<Question*> Write::writeToFile()
{
	std::wofstream file;
	try
	{
		std::string fileAddress = _typeInfo.getFileAddress();
		ensureFileEndsInEmptyLine(fileAddress);
		file.open(fileAddress, std::ios_base::app);
		if (!file.is_open())
			throw std::exception((fileAddress + " did not open correctly.").c_str());
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
