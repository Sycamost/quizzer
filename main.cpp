#include <iostream>
#include <easy_list.h>
#include "InputHandler.h"
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

	while (true)
	{
		std::wstring userInput = getInputLine();
		InputHandler::Returns ret = InputHandler::call(userInput);

		if (ret == InputHandler::Returns::SUCCESS)
			continue;

		if (ret == InputHandler::Returns::CMD_NOT_RECOGNISED)
		{
			std::wcout << L"Command not recognised. Please enter a valid command.\n";
			continue;
		}

		if (ret == InputHandler::Returns::QUIT_APP)
		{
			break;
		}

		if (ret == InputHandler::Returns::TOO_FEW_ARGS)
		{
			std::wcout << L"Too few arguments were provided for that command.\n";
			continue;
		}

		if (ret == InputHandler::Returns::INVALID_ARGS)
		{
			std::wcout << L"Invalid arguments were provided for that command.\n";
			continue;
		}

		if (ret == InputHandler::Returns::INVALID_STATE)
		{
			std::wcout << L"That command is invalid right now.\n";
			continue;
		}
	}

	return 0;
}

extern DECLARE_CMD_FUNC(cmdFuncQuit) {

	if (inputYesNo(L"Are you sure you want to exit the app?"))
	{
		std::wcout << L"Exiting app...\n";
		return InputHandler::Returns::QUIT_APP;
	}

	std::wcout << L"Not exiting app.\n";
	return InputHandler::Returns::SUCCESS;;
};