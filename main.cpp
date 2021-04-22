#include <iostream>
#include <easy_list.h>
#include "Handlers.h"
#include "util.h"
#include "globals.h"
#include "Question.h"
#include "QuestionList.h"
#include "Command.h"
#include "Read.h"

int main() {

	std::wcout << L"Reading questions from file...\n";
	read();
	std::wcout << L"Read " << QuestionList::get().size() << L" questions from file.\n\n";
	std::wcout << L"Welcome to Quizzer!\n\n";

	bool doContinue = true;
	while (doContinue)
	{
		std::wstring userInput = getInputLine();

		// Command
		if (!userInput.empty() && userInput[0] == L'\\')
		{

			CommandHandlerReturns ret = CommandHandler::call(userInput);

			switch (ret)
			{
				case CommandHandlerReturns::SUCCESS:
					break;

				case CommandHandlerReturns::QUIT_APP:
					doContinue = false;
					break;

				case CommandHandlerReturns::RESET_INPUT:
					InputHandler::reset();
					break;

				case CommandHandlerReturns::TOO_FEW_ARGS:
					std::wcout << L"Too few arguments were provided for that command.\n";
					InputHandler::reset();
					break;

				case CommandHandlerReturns::INVALID_ARGS:
					std::wcout << L"Invalid arguments were provided for that command.\n";
					InputHandler::reset();
					break;

				case CommandHandlerReturns::INVALID_STATE:
					std::wcout << L"That command is invalid right now.\n";
					InputHandler::reset();
					break;

				case CommandHandlerReturns::CMD_NOT_RECOGNISED:
				default:
					std::wcout << L"Command not recognised. Please enter a valid command.\n";
					InputHandler::reset();
					break;
			}

			continue;
		}

		// Input
		auto ret = InputHandler::call(userInput);
		/* Will we ever find a use for input handler return values?
		switch (ret)
		{
		case InputHandlerReturns::SUCCESS:
		default:
			break;
		}
		*/
	}

	return 0;
}

extern DEFINE_CMD_FUNC(cmdFuncQuit) {

	if (inputYesNo(L"Are you sure you want to exit the app?"))
	{
		std::wcout << L"Exiting app...\n";
		return CommandHandlerReturns::QUIT_APP;
	}

	std::wcout << L"Not exiting app.\n";
	return CommandHandlerReturns::SUCCESS;;
};