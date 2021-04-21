#pragma once
#include <string>
#include "Command.h"

#define DECLARE_CMD_HANDLER_FUNC(funcName) const CommandHandlerFunc funcName
#define DEFINE_CMD_HANDLER_FUNC(funcName) const CommandHandlerFunc funcName = [](Command command) -> CommandHandlerReturns
#define DECLARE_INPUT_HANDLER_FUNC(funcName) const InputHandlerFunc funcName
#define DEFINE_INPUT_HANDLER_FUNC(funcName) const InputHandlerFunc funcName = [](std::wstring input) -> InputHandlerReturns

enum class CommandHandlerReturns {
	SUCCESS,
	CMD_NOT_RECOGNISED,
	QUIT_APP,
	TOO_FEW_ARGS,
	INVALID_ARGS,
	INVALID_STATE
};
using CommandHandlerFunc = CommandHandlerReturns(*)(Command);
enum class InputHandlerReturns {
	SUCCESS,
	INVALID_INPUT
};
using InputHandlerFunc = InputHandlerReturns(*)(std::wstring);

class CommandHandler {
private:
	using Returns = CommandHandlerReturns;
	using Func = CommandHandlerFunc;
public:
	static Returns call(Command command);
private:
	static Func _handlerFunc;
	static void set(Func func);
	friend void setHandlers(CommandHandlerFunc commandHandlerFunc, InputHandlerFunc inputHandlerFunc);
	friend void setHandlersDefault();
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
	friend void setHandlers(CommandHandlerFunc commandHandlerFunc, InputHandlerFunc inputHandlerFunc);
	friend void setHandlersDefault();
};

void setHandlers(CommandHandlerFunc commandHandlerFunc, InputHandlerFunc inputHandlerFunc);
void setHandlersDefault();