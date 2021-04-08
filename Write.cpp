#include "Write.h"
#include "CmdHandler.h"
#include "util.h"
#include "WriteFlashcard.h"

const DECLARE_CMD_FUNC(startWriting) {

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