#include "CmdWrite.h"
#include "WriteQuestion.h"
#include "CmdHandler.h"
#include "util.h"
#include "WriteFlashcard.h"
#include "globals.h"

CmdHandler::Returns writeCmdHandler(std::wstring userInput)
{
	using CmdHandler::Returns;
	typedef WriteQuestion::Stage Stage;
	Stage stage = WriteQuestion::getStage();

	Command* cmd = Command::read(userInput);
	if (cmd != NULL)
	{
		if (cmd->getCommandInfo().type == CommandType::CANCEL)
		{
			std::wcout << L"Are you sure you want to cancel writing the current "
				<< questionTypeDisplay.at(WriteQuestion::getCurrentType())
				<< L"? [Y/N]\n";

			if (getUserYesNo())
			{
				WriteQuestion::cancel();
				return CmdHandler::Returns::SUCCESS;
			}

			WriteQuestion::resetLastStep();
			return CmdHandler::Returns::SUCCESS;
		}

		return CmdHandler::Returns::CMD_NOT_RECOGNISED;
	}

	if (stage == Stage::INPUT_DATA)
	{
		WriteQuestion::inputData(userInput);
		return CmdHandler::Returns::SUCCESS;
	}

	else if (stage == Stage::INPUT_TAGS)
	{
		if (userInput == L"")
		{
			WriteQuestion::pushCurrent();
			return Returns::SUCCESS;
		}

		if (userInput.find(L' ') != std::wstring::npos)
		{
			std::wcout << L"Tags must be one word only.\n";
			WriteQuestion::resetLastStep();
			return Returns::SUCCESS;
		}

		WriteQuestion::pushTag(userInput);
		WriteQuestion::setStage(Stage::INPUT_TAGS);
		return Returns::SUCCESS;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting write session...\n";
	WriteQuestion::finishWriting();
	return Returns::CMD_NOT_RECOGNISED;
}

DECLARE_CMD_FUNC(startWriting) {

	if (args.size() == 0)
		return CmdHandler::Returns::TOO_FEW_ARGS;

	auto typeIter = questionCodeType.find(toUpper(args[0]));
	if (typeIter == questionCodeType.end())
		return CmdHandler::Returns::INVALID_ARGS;

	CmdHandler::setHandler(writeCmdHandler);
	WriteQuestion::startWriting(typeIter->second);
	return CmdHandler::Returns::SUCCESS;
};

DECLARE_CMD_FUNC(cancelCurrentWrite) {
	std::wcout << L"\nAre you sure you want to cancel writing the current question? [Y/N]\n";
	if (getUserYesNo())
	{
		WriteQuestion::setStage(WriteQuestion::Stage::NEXT_QUESTION);
		return CmdHandler::Returns::SUCCESS;
	}
	WriteQuestion::resetLastStep();
	return CmdHandler::Returns::SUCCESS;
};

DECLARE_CMD_FUNC(setCaseSensitive) {
	throw new std::exception("setCaseSensitive() not implemented yet!");
};