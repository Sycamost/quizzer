#include <iostream>
#include <easy_list.h>
#include "Handlers.h"
#include "Command.h"
#include "HandlerReturns.h"

DEFINE_INPUT_HANDLER_FUNC(defaultInputHandlerFunc) {
	std::wcout << L"Please enter a command. Commands are preceded by a backslash <\\>, e.g. <\\play>.\n";
	return InputHandlerReturns::SUCCESS;
}
#define DEFAULT_VALID_COMMAND_TYPES CommandType::EXIT, CommandType::QUIT, CommandType::PLAY, CommandType::WRITE
InputHandlerFunc InputHandler::_handlerFunc{ &defaultInputHandlerFunc };
easy_list::list<CommandType> CommandHandler::_validCommandTypes{ easy_list::list<CommandType>({ DEFAULT_VALID_COMMAND_TYPES }) };

CommandHandlerReturns CommandHandler::call(std::wstring input)
{
	Command* command = Command::read(input);

	if (command == nullptr)
		return CommandHandlerReturns::CMD_NOT_RECOGNISED;

	if (_validCommandTypes.contains(command->getCommandInfo().getType()))
		return command->doCommandFunc();

	return CommandHandlerReturns::INVALID_STATE;
}

DEFINE_INPUT_HANDLER_FUNC(InputHandler::call)
{
	return _handlerFunc(input);
}

void setHandlingDefault()
{
	setHandling(&defaultInputHandlerFunc, DEFAULT_VALID_COMMAND_TYPES);
}
