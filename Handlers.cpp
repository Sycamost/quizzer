#include <iostream>
#include <easy_list.h>
#include "Handlers.h"
#include "Command.h"
#include "HandlerReturns.h"

InputHandlerFunc getDefaultInputHandlerFunc();
InputHandlerFunc InputHandler::_handlerFunc{ getDefaultInputHandlerFunc() };
easy_list::list<CommandType> CommandHandler::_validCommandTypes{ easy_list::list<CommandType>() };

CommandHandlerReturns CommandHandler::call(std::wstring input)
{
	Command* command = Command::read(input);

	if (command == nullptr)
		return CommandHandlerReturns::CMD_NOT_RECOGNISED;

	if (_validCommandTypes.contains(command->getCommandInfo().getType()))
		return command->doCommandFunc();

	return CommandHandlerReturns::INVALID_STATE;
}

void InputHandler::set(Func handler)
{
	_handlerFunc = handler;
}
InputHandlerReturns InputHandler::call(std::wstring input)
{
	return _handlerFunc(input);
}

InputHandlerFunc getDefaultInputHandlerFunc()
{
	static DEFINE_INPUT_HANDLER_FUNC(defaultInputHandlerFunc) {
		std::wcout << L"Please enter a command. Commands are preceded by a backslash <\\>, e.g. <play>.\n";
		return InputHandlerReturns::SUCCESS;
	};
	return defaultInputHandlerFunc;
}

template <typename ..._CommandTypee>
void setHandling(InputHandlerFunc inputHandlerFunc, _CommandTypee... validCommandTypes)
{
	static_assert(std::conjunction_v<std::is_same<_CommandTypee, CommandType>...>);
	InputHandler::set(inputHandlerFunc);
	CommandHandler::_validCommandTypes = easy_list::list({ validCommandTypes... });
}

void setHandlingDefault()
{
	setHandling(
		getDefaultInputHandlerFunc(),
		CommandType::EXIT,
		CommandType::QUIT,
		CommandType::PLAY,
		CommandType::WRITE
	);
}
