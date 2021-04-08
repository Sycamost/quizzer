#pragma once
#include <fstream>

class WriteQuestion
{
public:
	enum class Stage {
		INPUT_DATA,
		INPUT_TAGS
	};
	static const WriteQuestion& getCurrentInstance();
	static const Stage getCurrentStage();
	static void setCurrentStage(Stage stage);
	static void pushTag(std::wstring tag);
	static const void inputData(std::wstring);
	static const void cancel();
	static const void resetLastStep();
	static const void pushCurrent();
private:
	const Stage getStage();
	static void setStage(Stage stage);
	static WriteQuestion _currentInstance;
	static std::vector<std::wstring> _currentTags;
	Stage _stage;
	typedef void (*VoidFuncWstr)(std::wstring);
	typedef void (*VoidFunc)();
	static VoidFuncWstr _inputData;
	static VoidFunc _cancel;
	static VoidFunc _resetLastStep;
	static VoidFunc _pushCurrent;
};