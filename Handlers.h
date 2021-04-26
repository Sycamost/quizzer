#pragma once
#include <type_traits>
#include <string>
#include <iostream>
#include "HandlerReturns.h"
#include "Command.h"

#define DECLARE_INPUT_HANDLER_FUNC(funcName) InputHandlerReturns funcName(std::wstring input)
#define DEFINE_INPUT_HANDLER_FUNC(funcName) InputHandlerReturns funcName(std::wstring input)
#define DECLARE_BEFORE_HANDLER_FUNC(funcName) BeforeHandlerReturns funcName()
#define DEFINE_BEFORE_HANDLER_FUNC(funcName) BeforeHandlerReturns funcName()

using BeforeHandlerFunc = BeforeHandlerReturns(*)();
using InputHandlerFunc = InputHandlerReturns(*)(std::wstring);

void setInputHandling(const BeforeHandlerFunc& beforeHandlerFunc, const InputHandlerFunc& inputHandlerFunc);
void setInputHandling(const InputHandlerFunc& inputHandlerFunc);
void setInputHandling(const std::wstring beforeHandlerMsg, const InputHandlerFunc& inputHandlerFunc);
void setCommandHandling(const easy_list::list<CommandType> validCommandTypes);
void setHandlingDefault();

class CommandHandler {
	friend void setInputHandling(const BeforeHandlerFunc& beforeHandlerFunc, const InputHandlerFunc& inputHandlerFunc);
	friend void setInputHandling(const std::wstring beforeHandlerMsg, const InputHandlerFunc& inputHandlerFunc);
	friend void setInputHandling(const InputHandlerFunc& inputHandlerFunc);
	friend void setCommandHandling(const easy_list::list<CommandType> validCommandTypes);
	friend void setHandlingDefault();
private:
	using Returns = CommandHandlerReturns;
	static easy_list::list<CommandType> _validCommandTypes;
public:
	static Returns call(std::wstring input);
};

class InputHandler {
	friend void setInputHandling(const BeforeHandlerFunc& beforeHandlerFunc, const InputHandlerFunc& inputHandlerFunc);
	friend void setInputHandling(const std::wstring beforeHandlerMsg, const InputHandlerFunc& inputHandlerFunc);
	friend void setInputHandling(const InputHandlerFunc& inputHandlerFunc);
	friend void setCommandHandling(const easy_list::list<CommandType> validCommandTypes);
	friend void setHandlingDefault();
private:
	using Returns = InputHandlerReturns;
	using Func = InputHandlerFunc;
	static Func _handlerFunc;
	static BeforeHandlerFunc _beforeHandlerFunc;
	static std::wstring _beforeHandlerMsg;
public:
	static DECLARE_INPUT_HANDLER_FUNC(call);
	static DECLARE_BEFORE_HANDLER_FUNC(reset);
};