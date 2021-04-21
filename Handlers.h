#pragma once
#include <type_traits>
#include <string>
#include "HandlerReturns.h"
#include "Command.h"

#define DECLARE_INPUT_HANDLER_FUNC(funcName) const InputHandlerFunc funcName
#define DEFINE_INPUT_HANDLER_FUNC(funcName) const InputHandlerFunc funcName = [](std::wstring input) -> InputHandlerReturns

using InputHandlerFunc = InputHandlerReturns(*)(std::wstring);

template <typename ..._CommandTypee>
void setHandling(InputHandlerFunc inputHandlerFunc, _CommandTypee... validCommandTypes)
{
	static_assert(std::conjunction_v<std::is_same<_CommandTypee, CommandType>...>);
	InputHandler::_handlerFunc = inputHandlerFunc;
	CommandHandler::_validCommandTypes = easy_list::list({ validCommandTypes... });
}
void setHandlingDefault();

class CommandHandler {
	template <typename ..._CommandTypee>
	friend void setHandling<_CommandTypee...>(InputHandlerFunc inputHandlerFunc, _CommandTypee... validCommandTypes);
	friend void setHandlingDefault();
private:
	using Returns = CommandHandlerReturns;
	static easy_list::list<CommandType> _validCommandTypes;
public:
	static Returns call(std::wstring input);
};

class InputHandler {
	template <typename ..._CommandTypee>
	friend void setHandling<_CommandTypee...>(InputHandlerFunc inputHandlerFunc, _CommandTypee... validCommandTypes);
	friend void setHandlingDefault();
private:
	using Returns = InputHandlerReturns;
	using Func = InputHandlerFunc;
	static Func _handlerFunc;
public:
	static Returns call(std::wstring input);
};