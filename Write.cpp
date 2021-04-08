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
	WriteQuestion& writer = WriteQuestion::getCurrentInstance();
	using CmdHandler::Returns;
	typedef WriteQuestion::Stage Stage;

	Command* cmd = Command::read(userInput);
	if (cmd != NULL && cmd->

	if (writer.getStage() == Stage::INPUT_DATA)
	{
		WriteQuestion::getCurrentInstance().inputData(userInput);
		return Returns::SUCCESS;
	}

	else if (WriteQuestion::getCurrentInstance().getStage() == Stage::INPUT_DATA)
	{
		if (userInputUpper == Globals::cmdCancel)
		{
			std::wcout << L"\nAre you sure you want to cancel adding the current card? [Y/N]\n";
			if (getUserYesNo())
			{
				WriteFlashcard::setValue(Stage::NEW_CARD);
				return Returns::SUCCESS;
			}
			WriteFlashcard::setValue(Stage::TAGS);
			return Returns::SUCCESS;
		}

		if (userInput == L"")
		{
			newFlashcards.push_back(new Flashcard(front, back, false, tags));
			WriteFlashcard::setValue(WriteFlashcard::Stage::NEW_CARD);
			return Returns::SUCCESS;
		}

		if (userInput.find(L' ') != std::wstring::npos)
		{
			std::wcout << L"Tags must be one word only.\n";
			WriteFlashcard::setValue(WriteFlashcard::Stage::TAGS);
			return Returns::SUCCESS;
		}

		tags.push_back(userInput);
		WriteFlashcard::setValue(WriteFlashcard::Stage::TAGS);
		return Returns::SUCCESS;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting write session...\n";
	finishWriting();
	return Returns::CMD_NOT_RECOGNISED;
}