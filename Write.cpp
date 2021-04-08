#include "Write.h"
#include "CmdHandler.h"
#include "util.h"
#include "WriteFlashcard.h"

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