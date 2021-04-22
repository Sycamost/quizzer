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

/// <summary>
/// Sets the state of command/input handling.
/// </summary>
/// <param name="beforeHandlerFunc">This function will be called before asking for user input, and when the state is reset.</param>
/// <param name="inputHandlerFunc">This function will handle (non-command) user input.</param>
/// <param name="...validCommandTypes">These command types precisely will be executed until this function is called again.</param>
template <typename ..._CommandTypee>
void setHandling(const BeforeHandlerFunc& beforeHandlerFunc, const InputHandlerFunc& inputHandlerFunc, const _CommandTypee... validCommandTypes)
{
	static_assert(std::conjunction_v<std::is_same<_CommandTypee, CommandType>...>);
	InputHandler::_beforeHandlerFunc = beforeHandlerFunc;
	InputHandler::_handlerFunc = inputHandlerFunc;
	InputHandler::_beforeHandlerMsg = L"";
	CommandHandler::_validCommandTypes = easy_list::list({ validCommandTypes... });
	InputHandler::reset();
}

/// <summary>
/// Sets the state of command/input handling. (Use this overload if you want nothing to happen before the user gives input.)
/// </summary>
/// <param name="inputHandlerFunc">This function will handle (non-command) user input.</param>
/// <param name="...validCommandTypes">These command types precisely will be executed until this function is called again.</param>
template <typename ..._CommandTypee>
void setHandling(const InputHandlerFunc& inputHandlerFunc, const _CommandTypee... validCommandTypes)
{
	setHandling([]() -> BeforeHandlerReturns {}, inputHandlerFunc, validCommandTypes...);
}

/// <summary>
/// Sets the state of command/input handling.
/// </summary>
/// <param name="beforeHandlerMsg">This message will be displayed before asking for user input, and when the state is reset.</param>
/// <param name="inputHandlerFunc">This function will handle (non-command) user input.</param>
/// <param name="...validCommandTypes">These command types precisely will be executed until this function is called again.</param>
template <typename ..._CommandTypee>
void setHandling(const std::wstring beforeHandlerMsg, const InputHandlerFunc& inputHandlerFunc, const _CommandTypee... validCommandTypes)
{
	static_assert(std::conjunction_v<std::is_same<_CommandTypee, CommandType>...>);
	InputHandler::_beforeHandlerFunc = nullptr;
	InputHandler::_handlerFunc = inputHandlerFunc;
	InputHandler::_beforeHandlerMsg = beforeHandlerMsg;
	CommandHandler::_validCommandTypes = easy_list::list({ validCommandTypes... });
	InputHandler::reset();
}

void setHandlingDefault();

class CommandHandler {
	template <typename ..._CommandTypee>
	friend void setHandling(const BeforeHandlerFunc& beforeHandlerFunc, const InputHandlerFunc& inputHandlerFunc, const _CommandTypee... validCommandTypes);
	template <typename ..._CommandTypee>
	friend void setHandling(const std::wstring beforeHandlerMsg, const InputHandlerFunc& inputHandlerFunc, const _CommandTypee... validCommandTypes);
	template <typename ..._CommandTypee>
	friend void setHandling(const InputHandlerFunc& inputHandlerFunc, const _CommandTypee... validCommandTypes);
	friend void setHandlingDefault();
private:
	using Returns = CommandHandlerReturns;
	static easy_list::list<CommandType> _validCommandTypes;
public:
	static Returns call(std::wstring input);
};

class InputHandler {
	template <typename ..._CommandTypee>
	friend void setHandling(const BeforeHandlerFunc& beforeHandlerFunc, const InputHandlerFunc& inputHandlerFunc, const _CommandTypee... validCommandTypes);
	template <typename ..._CommandTypee>
	friend void setHandling(const std::wstring beforeHandlerMsg, const InputHandlerFunc& inputHandlerFunc, const _CommandTypee... validCommandTypes);
	template <typename ..._CommandTypee>
	friend void setHandling(const InputHandlerFunc& inputHandlerFunc, const _CommandTypee... validCommandTypes);
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