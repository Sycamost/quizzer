#include <algorithm>
#include <iostream>
#include "Question.h"
#include "QuestionWriter.h"
#include "Write.h"
#include "Command.h"
#include "InputHandler.h"
#include "util.h"

QuestionTypeInfo Write::_typeInfo{ *getQuestionTypeInfo(QuestionType::FLASHCARD) };
Write::Stage Write::_stage{ Write::Stage::NEXT_QUESTION };

InputHandler::Handler Write::getWriteHandler()
{
	static InputHandler::Handler writeHandler = [](std::wstring input) -> InputHandler::Returns
	{
		Command* command = Command::read(input);
		if (command != nullptr)
		{
			CommandInfo cmdInfo = command->getCommandInfo();

			if (cmdInfo.type == CommandType::CANCEL)
				return command->doCommandFunc();

			return InputHandler::Returns::INVALID_STATE;
		}

		if (_stage == Stage::INPUT)
		{
			_typeInfo.writer.processInput(input);
			return InputHandler::Returns::SUCCESS;
		}

		std::wcout << L"\nSomething went wrong interpreting that input. Exiting write session...\n";
		finishWriting();
		return InputHandler::Returns::CMD_NOT_RECOGNISED;
	};
	return writeHandler;
}

void Write::nextQuestion()
{
	setStage(Stage::NEXT_QUESTION);
}

DECLARE_CMD_FUNC(Write::cmdFuncWrite) {

	if (args.size() == 0)
		return InputHandler::Returns::TOO_FEW_ARGS;

	const QuestionTypeInfo* questionTypeInfo = getQuestionTypeInfoFromCode(args[0]);
	if (questionTypeInfo == nullptr)
		return InputHandler::Returns::INVALID_ARGS;

	InputHandler::set(getWriteHandler());
	Write::startWriting(*questionTypeInfo);
	return InputHandler::Returns::SUCCESS;
};

DECLARE_CMD_FUNC(Write::cmdFuncCancel) {
	std::wcout << L"\n";
	if (inputYesNo(L"Are you sure you want to cancel writing the current question?"))
	{
		setStage(Stage::NEXT_QUESTION);
		return InputHandler::Returns::SUCCESS;
	}
	resetLastStep();
	return InputHandler::Returns::SUCCESS;
};

void Write::setStage(Stage stage)
{
	if (stage == Stage::NEXT_QUESTION)
	{
		_stage = stage;

		std::wstring message =
			L"Would you like to write another "
			+ _typeInfo.displaySingular
			+ L"?";

		if (inputYesNo(message))
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
	QuestionList::append(_typeInfo.writer.writeToFile());
	InputHandler::setDefault();
}
