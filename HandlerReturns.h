#pragma once

enum class CommandHandlerReturns {
	SUCCESS,
	CMD_NOT_RECOGNISED,
	QUIT_APP,
	TOO_FEW_ARGS,
	INVALID_ARGS,
	INVALID_STATE
};

enum class InputHandlerReturns {
	SUCCESS,
	INVALID_INPUT
};