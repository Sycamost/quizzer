#include <algorithm>
#include <iostream>
#include "Question.h"
#include "QuestionWriter.h"
#include "Write.h"
#include "Command.h"
#include "WriteFlashcard.h"
#include "CmdHandler.h"
#include "util.h"

QuestionTypeInfo Write::_typeInfo = *getQuestionTypeInfo(QuestionType::FLASHCARD);
Write::Stage Write::_stage = Write::Stage::NEXT_QUESTION;

CmdHandler::Returns Write::cmdHandler(std::wstring userInput)
{
	Command* cmd = Command::read(userInput);
	if (cmd != nullptr)
		return cmd->doCommandFunc();

	if (_stage == Stage::INPUT)
	{
		_typeInfo.writer.processInput(userInput);
		return CmdHandler::Returns::SUCCESS;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting write session...\n";
	finishWriting();
	return CmdHandler::Returns::CMD_NOT_RECOGNISED;
}

void Write::nextQuestion()
{
	setStage(Stage::NEXT_QUESTION);
}

DECLARE_CMD_FUNC(Write::cmdFuncWrite) {

	if (args.size() == 0)
		return CmdHandler::Returns::TOO_FEW_ARGS;

	const QuestionTypeInfo* questionTypeInfo = getQuestionTypeInfoFromCode(args[0]);
	if (questionTypeInfo == nullptr)
		return CmdHandler::Returns::INVALID_ARGS;

	CmdHandler::setHandler(cmdHandler);
	Write::startWriting(*questionTypeInfo);
	return CmdHandler::Returns::SUCCESS;
};

DECLARE_CMD_FUNC(Write::cmdFuncCancel) {
	std::wcout << L"\nAre you sure you want to cancel writing the current question? [Y/N]\n";
	if (getUserYesNo())
	{
		setStage(Stage::NEXT_QUESTION);
		return CmdHandler::Returns::SUCCESS;
	}
	resetLastStep();
	return CmdHandler::Returns::SUCCESS;
};

void Write::setStage(Stage stage)
{
	if (stage == Stage::NEXT_QUESTION)
	{
		_stage = stage;

		std::wcout << L"Would you like to write another "
			<< _typeInfo.displaySingular
			<< L"? [Y/N]\n";

		if (getUserYesNo())
		{
			setStage(Stage::INPUT);
			return;
		}

		finishWriting();
		return;
	}

	if (stage == Stage::INPUT)
	{
		_stage = stage;
		_typeInfo.writer.startInput();
		return;
	}

	return;
}

void Write::startWriting(const QuestionTypeInfo qti)
{
	std::wcout << "Writing new " << qti.displayPlural << L"...\n\n";
	_typeInfo = qti;
	_stage = Stage::INPUT;
	std::wcout << _typeInfo.writer.getStartWritingMessage() << L"\n";
	_typeInfo.writer.startInput();
}

void Write::resetLastStep()
{
	switch (_stage)
	{
	case Stage::NEXT_QUESTION:
		setStage(Stage::NEXT_QUESTION);
		break;

	case Stage::INPUT:
		_typeInfo.writer.resetLastStep();
		break;

	default:
		break;
	}
}

void Write::finishWriting()
{
	std::wcout << L"\nFinished writing new " << _typeInfo.displayPlural << L".\n";
	Question::appendQuestionsToList(_typeInfo.writer.writeToFile());
	CmdHandler::setHandlerDefault();
}
