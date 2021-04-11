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
	Command* command = Command::read(userInput);
	if (command == nullptr)
		return InputHandler::Returns::CMD_NOT_RECOGNISED;
	return command->doCommandFunc();
}