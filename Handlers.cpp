#include <iostream>
#include <easy_list.h>
#include "Handlers.h"
#include "Command.h"
#include "HandlerReturns.h"

InputHandlerFunc getDefaultInputHandlerFunc()
{
	static DEFINE_INPUT_HANDLER_FUNC(defaultInputHandlerFunc) {
		std::wcout << L"Please enter a command. Commands are preceded by a backslash <\\>, e.g. <play>.\n";
		return InputHandlerReturns::SUCCESS;
	};
	return defaultInputHandlerFunc;
}
#define DEFAULT_VALID_COMMAND_TYPES CommandType::EXIT, CommandType::QUIT, CommandType::PLAY, CommandType::WRITE
InputHandlerFunc InputHandler::_handlerFunc{ getDefaultInputHandlerFunc() };
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

InputHandlerReturns InputHandler::call(std::wstring input)
{
	return _handlerFunc(input);
}

void setHandlingDefault()
{
	setHandling(getDefaultInputHandlerFunc(), DEFAULT_VALID_COMMAND_TYPES);
}
