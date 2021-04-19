#include "InputHandler.h"
#include "Command.h"

InputHandler::Handler getDefaultInputHandler();

InputHandler::Handler InputHandler::_handler { getDefaultInputHandler() };

void InputHandler::set(Returns(*handler)(std::wstring))
{
	_handler = handler;
}

void InputHandler::setDefault()
{
	_handler = getDefaultInputHandler();
}

InputHandler::Returns InputHandler::call(std::wstring input)
{
	return _handler(input);
}

InputHandler::Handler getDefaultInputHandler()
{
	static InputHandler::Handler defaultInputHandler = [](std::wstring input) -> InputHandler::Returns
	{
		if (input.empty())
			return InputHandler::Returns::SUCCESS;

		Command* cmd = Command::read(input);
		if (cmd == nullptr)
			return InputHandler::Returns::CMD_NOT_RECOGNISED;

		CommandType cmdType = cmd->getCommandInfo().getType();
		if (cmdType == CommandType::QUIT ||
			cmdType == CommandType::EXIT ||
			cmdType == CommandType::WRITE ||
			cmdType == CommandType::PLAY)
		{
			return cmd->doCommandFunc();
		}

		return InputHandler::Returns::INVALID_STATE;
	};
	return defaultInputHandler;
}