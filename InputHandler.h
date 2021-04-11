#pragma once
#include <string>

class InputHandler {
public:
	enum class Returns {
		SUCCESS,
		CMD_NOT_RECOGNISED,
		QUIT_APP,
		TOO_FEW_ARGS,
		INVALID_ARGS,
		INVALID_STATE
	};
	typedef Returns(*Handler)(std::wstring input);
	static void set(Returns(*handler)(std::wstring));
	static void setDefault();
	static Returns call(std::wstring input);
private:
	static Handler _handler;
};