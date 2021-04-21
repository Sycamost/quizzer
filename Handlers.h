#pragma once
#include <string>
#include "HandlerReturns.h"
#include "Command.h"

#define DECLARE_INPUT_HANDLER_FUNC(funcName) const InputHandlerFunc funcName
#define DEFINE_INPUT_HANDLER_FUNC(funcName) const InputHandlerFunc funcName = [](std::wstring input) -> InputHandlerReturns

using InputHandlerFunc = InputHandlerReturns(*)(std::wstring);

class CommandHandler {
private:
	using Returns = CommandHandlerReturns;
public:
	static CommandHandlerReturns call(std::wstring input);
private:
	template <typename ..._CommandTypee, std::enable_if_t<std::conjunction_v<std::is_same<_CommandTypee, CommandType>...>, bool> = true>
	friend void setHandling(InputHandlerFunc inputHandlerFunc, _CommandTypee... validCommandTypes);
	friend void setHandlingDefault();
	static easy_list::list<CommandType> _validCommandTypes;
};

class InputHandler {
private:
	using Returns = InputHandlerReturns;
	using Func = InputHandlerFunc;
public:
	static Returns call(std::wstring input);
private:
	static Func _handlerFunc;
	static void set(Func func);
	template <typename ..._CommandTypee, std::enable_if_t<std::conjunction_v<std::is_same<_CommandTypee, CommandType>...>, bool> = true>
	friend void setHandling(InputHandlerFunc inputHandlerFunc, _CommandTypee... validCommandTypes);
	friend void setHandlingDefault();
};

template <typename ..._CommandTypee, std::enable_if_t<std::conjunction_v<std::is_same<_CommandTypee, CommandType>...>, bool> = true>
void setHandling(InputHandlerFunc inputHandlerFunc, _CommandTypee... validCommandTypes);
void setHandlingDefault();