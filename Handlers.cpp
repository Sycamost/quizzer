#include <iostream>
#include "Handlers.h"
#include "Command.h"

CommandHandlerFunc getDefaultCommandHandlerFunc();
InputHandlerFunc getDefaultInputHandlerFunc();

CommandHandlerFunc CommandHandler::_handlerFunc { getDefaultCommandHandlerFunc() };
InputHandlerFunc InputHandler::_handlerFunc{ getDefaultInputHandlerFunc() };

void CommandHandler::set(Func handler)
{
	_handlerFunc = handler;
}
void InputHandler::set(Func handler)
{
	_handlerFunc = handler;
}

CommandHandlerReturns CommandHandler::call(Command command)
{
	return _handlerFunc(command);
}
InputHandlerReturns InputHandler::call(std::wstring input)
{
	return _handlerFunc(input);
}

CommandHandlerFunc getDefaultCommandHandlerFunc()
{
	static DEFINE_CMD_HANDLER_FUNC(defaultCommandHandlerFunc) {

		CommandType commandType = command.getCommandInfo().getType();
		if (commandType == CommandType::QUIT ||
			commandType == CommandType::EXIT ||
			commandType == CommandType::WRITE ||
			commandType == CommandType::PLAY)
		{
			return command.doCommandFunc();
		}

		return CommandHandlerReturns::INVALID_STATE;
	};
	return defaultCommandHandlerFunc;
}

InputHandlerFunc getDefaultInputHandlerFunc()
{
	static DEFINE_INPUT_HANDLER_FUNC(defaultInputHandlerFunc) {
		std::wcout << L"Please enter a command. Commands are preceded by a backslash <\\>, e.g. <\play>.\n";
		return InputHandlerReturns::SUCCESS;
	};
	return defaultInputHandlerFunc;
}

void setHandlers(CommandHandlerFunc commandHandlerFunc, InputHandlerFunc inputHandlerFunc)
{
	CommandHandler::set(commandHandlerFunc);
	InputHandler::set(inputHandlerFunc);
}

void setHandlersDefault()
{
	CommandHandler::set(getDefaultCommandHandlerFunc());
	InputHandler::set(getDefaultInputHandlerFunc());
}
