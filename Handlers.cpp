#include <iostream>
#include <easy_list.h>
#include "Handlers.h"
#include "Command.h"
#include "HandlerReturns.h"

DEFINE_INPUT_HANDLER_FUNC(defaultInputHandlerFunc) {
	std::wcout << L"Please enter a command. Commands are preceded by a backslash <\\>, e.g. <\\play>.\n";
	return InputHandlerReturns::SUCCESS;
}
DEFINE_BEFORE_HANDLER_FUNC(defaultBeforeHandlerFunc) {}
#define DEFAULT_VALID_COMMAND_TYPES CommandType::QUIT, CommandType::PLAY, CommandType::WRITE
InputHandlerFunc InputHandler::_handlerFunc{ &defaultInputHandlerFunc };
BeforeHandlerFunc InputHandler::_beforeHandlerFunc{ &defaultBeforeHandlerFunc };
std::wstring InputHandler::_beforeHandlerMsg{ L"" };
easy_list::list<CommandType> CommandHandler::_validCommandTypes{ easy_list::list<CommandType>({ DEFAULT_VALID_COMMAND_TYPES }) };

/// <summary>
/// Sets the state of input handling.
/// </summary>
/// <param name="beforeHandlerFunc">This function will be called before asking for user input, and when the state is reset.</param>
/// <param name="inputHandlerFunc">This function will handle (non-command) user input.</param>
void setInputHandling(const BeforeHandlerFunc& beforeHandlerFunc, const InputHandlerFunc& inputHandlerFunc)
{
	InputHandler::_beforeHandlerFunc = beforeHandlerFunc;
	InputHandler::_handlerFunc = inputHandlerFunc;
	InputHandler::_beforeHandlerMsg = L"";
	InputHandler::reset();
}

/// <summary>
/// Sets the state of input handling. (Use this overload if you want nothing to happen before the user gives input.)
/// </summary>
/// <param name="inputHandlerFunc">This function will handle (non-command) user input.</param>
void setInputHandling(const InputHandlerFunc& inputHandlerFunc)
{
	setInputHandling([]() -> BeforeHandlerReturns {}, inputHandlerFunc);
}

/// <summary>
/// Sets the state of input handling.
/// </summary>
/// <param name="beforeHandlerMsg">This message will be displayed before asking for user input, and when the state is reset.</param>
/// <param name="inputHandlerFunc">This function will handle (non-command) user input.</param>
void setInputHandling(const std::wstring beforeHandlerMsg, const InputHandlerFunc& inputHandlerFunc)
{
	InputHandler::_beforeHandlerFunc = nullptr;
	InputHandler::_handlerFunc = inputHandlerFunc;
	InputHandler::_beforeHandlerMsg = beforeHandlerMsg;
	InputHandler::reset();
}

/// <summary>
/// Sets the state of command handling.
/// </summary>
/// <param name="validCommandTypes">These command types precisely will be executed until this function is called again.</param>
void setCommandHandling(const easy_list::list<CommandType> validCommandTypes)
{
	CommandHandler::_validCommandTypes = validCommandTypes;
}

void setHandlingDefault()
{
	setInputHandling(&defaultBeforeHandlerFunc, &defaultInputHandlerFunc);
	setCommandHandling({ DEFAULT_VALID_COMMAND_TYPES });
}

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

DEFINE_BEFORE_HANDLER_FUNC(InputHandler::reset)
{
	if (_beforeHandlerFunc != nullptr)
		return _beforeHandlerFunc();
	std::wcout << _beforeHandlerMsg;
}
