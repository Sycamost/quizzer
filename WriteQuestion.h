#pragma once
#include <fstream>

class WriteQuestion
{
public:
	enum class Stage {
		CONTINUE,
		INPUT_DATA,
		INPUT_TAGS
	};
	static WriteQuestion& getCurrentInstance();
	Stage getStage();
	void setStage(Stage stage);
	void inputData(std::wstring);
private:
	static WriteQuestion _currentInstance;
	Stage _stage;
	typedef void (*VoidFuncWstr)(std::wstring);
	VoidFuncWstr _inputData;
};