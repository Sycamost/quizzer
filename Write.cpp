#include "Write.h"
#include "WriteQuestion.h"
#include "CmdHandler.h"
#include "util.h"
#include "WriteFlashcard.h"
#include "globals.h"

DECLARE_CMD_FUNC(startWriting) {

	if (args.size() == 0)
		return CmdHandler::Returns::TOO_FEW_ARGS;

	std::wstring type = toUpper(args[0]);

	if (type == L"FLASHCARD")
	{
		startWritingFlashcards();
		return CmdHandler::Returns::SUCCESS;
	}
	// ... other types

	return CmdHandler::Returns::INVALID_ARGS;
};

DECLARE_CMD_FUNC(cancelCurrentWrite) {
	std::wcout << L"\nAre you sure you want to cancel writing the current question? [Y/N]\n";
	if (getUserYesNo())
	{
		WriteFlashcard::setValue(WriteFlashcard::Stage::NEW_CARD);
		return CmdHandler::Returns::SUCCESS;
	}
	WriteFlashcard::setValue(WriteFlashcard::Stage::FRONT);
	return CmdHandler::Returns::SUCCESS;
};

DECLARE_CMD_FUNC(setCaseSensitive) {
	
};

CmdHandler::Returns writeCmdHandler(std::wstring userInput)
{
	using CmdHandler::Returns;
	typedef WriteQuestion::Stage Stage;
	WriteQuestion writer = WriteQuestion::getCurrentInstance();
	Stage stage = WriteQuestion::getCurrentStage();

	Command* cmd = Command::read(userInput);
	if (cmd != NULL && cmd->getCommandInfo().type == CommandType::CANCEL)
	{
		std::wcout << L"Are you sure you want to cancel writing the current question? [Y/N]\n";
		if (getUserYesNo())
		{
			writer.cancel();
			return CmdHandler::Returns::SUCCESS;
		}
		writer.resetLastStep();
		return CmdHandler::Returns::SUCCESS;
	}

	if (stage == Stage::INPUT_DATA)
	{
		WriteQuestion::getCurrentInstance().inputData(userInput);
		return Returns::SUCCESS;
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
		WriteQuestion::setCurrentStage(Stage::INPUT_TAGS);
		return Returns::SUCCESS;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting write session...\n";
	finishWriting();
	return Returns::CMD_NOT_RECOGNISED;
}