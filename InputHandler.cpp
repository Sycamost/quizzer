#include "InputHandler.h"
#include "Command.h"

InputHandler::Returns defaultInputHandler(std::wstring input);

InputHandler::Returns(*InputHandler::_handler)(std::wstring) { &defaultInputHandler };

void InputHandler::set(Returns(*handler)(std::wstring))
{
	_handler = handler;
}

void InputHandler::setDefault()
{
	_handler = &defaultInputHandler;
}

InputHandler::Returns InputHandler::call(std::wstring input)
{
	return _handler(input);
}

InputHandler::Returns defaultInputHandler(std::wstring userInput)
{
	if (userInput.empty())
		return InputHandler::Returns::SUCCESS;

	Command* cmd = Command::read(userInput);
	if (cmd == nullptr)
		return InputHandler::Returns::CMD_NOT_RECOGNISED;

	CommandType cmdType = cmd->getCommandInfo().type;
	if (cmdType == CommandType::QUIT ||
		cmdType == CommandType::EXIT ||
		cmdType == CommandType::WRITE ||
		cmdType == CommandType::PLAY)
	{
		return cmd->doCommandFunc();
	}

	return InputHandler::Returns::INVALID_STATE;
}