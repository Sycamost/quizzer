#pragma once
#include <string>

namespace CmdHandler {

	enum class Returns {
		SUCCESS,
		CMD_NOT_RECOGNISED,
		QUIT_APP
	};

	void setHandler(Returns(*handler)(std::wstring));
	void setHandlerDefault();
	bool isYes(std::wstring);
}